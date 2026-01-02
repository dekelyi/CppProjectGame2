#include <string>
#include <vector>
#include <filesystem>

#include "prelude.h"
#include "Collectible.h"
#include "Door.h"
#include "Obstacle.h"
#include "Spring.h"
#include "Switch.h"
#include "Riddle.h"
#include "LevelParser.h"

MapObject* ObjectData::into_map_object(GameView* game, GameRoom* room, const LevelParser& parser) const {
	switch (type) {
	case ObjType::PLAYER_1:
	case ObjType::PLAYER_2: {
		auto p = (type == ObjType::PLAYER_1) ? game->player1 : game->player2;
		p->setPosition(position);
		room->add_object(p);
		return p;
	}
	case ObjType::DOOR: {
		int required_keys;
		try {
			required_keys = stoi(properties.at("condition-keys"));
		}
		catch (...) {
			required_keys = 0;
		}
		DoorDest DEST = (id == -1) ? DoorDest::NEXT : DoorDest::PREV;
		auto door = new Door(position, size, DEST, required_keys);
		return door;
	}
	case ObjType::OBSTCALE:
		return new Obstacle(position, size);
	case ObjType::SPRING:
		return new Spring(position, size);
	case ObjType::SWITCH_OFF:
		if (properties.count("do-open-door")) {
			string door_id = properties.at("do-open-door");
			auto cond = new Condition(Condition::SWITCH, 1);
			Door* door = (door_id == "N") ? room->p_doors.exit_point : room->p_doors.entry_point;
			door->conditions.push_back(cond);
			auto cb = SwitchDoor(room, door, cond);
			return new Switch(position, cb);
		}
		return new Switch(position);
	case ObjType::TORCH:
		int area;
		try { area = stoi(properties.at("area")); }
		catch (...) { area = 5; }
		return new Torch(position, area);
	case  ObjType::KEY:
		return new Key(position, size);
	case ObjType::RIDDLE: {
		try {
			RiddleData rdata = parser.riddle_parser.riddles.at(id);
			return new Riddle(position, size, rdata);
		}
		catch (...) {
			return nullptr;
		}
	}
	case ObjType::BOMB: 
		return new Bomb(position);
	default:
		return new MapObject(position, size, (char)type);
	}
}

void LevelParser::build_room(GameView* game) {
	auto get_prop = [&](string key, int _default) -> int {
		string& val = this->room_properties[key];
		try {
			return stoi(val);
		}
		catch (...) {
			return _default;
		}
	};

	auto room = game->add_room(get_prop("width", SIZE_X), get_prop("height", SIZE_Y));
	for (auto& obj_data : objects) {
		ObjectData& od = obj_data;
		MapObject* obj = obj_data.into_map_object(game, room, *this);
		if (obj) room->add_object(obj);
	}
	for (const auto& [key, value] : room_properties) {
		if (key == "msg-on-enter") {
			room->msg->text = value;
			room->msg->ticks_left = 10;
		}
		else if (key == "is_dark") {
			if (value == "true" || value == "1") {
				room->p_torch.is_dark = true;
			}
		};
	}
}

void LevelParser::parse() {
	while (!file.eof()) {
		string line;
		getline(file, line);

		if (line.empty() || line.starts_with("//")) continue;
		if (line[0] == ':') {
			if (line[1] == ':') parse_object_data(line.substr(2));
			else parse_data(line.substr(1));
		}
		else if (line[0] == 'L') legend_position = current_y_pos;
		else {
			parse_objects(line);
			current_y_pos++;
		};

	}
}

void LevelParser::parse_objects(const string& line) {
	size_t index = line.find_first_not_of(' ');
	size_t x_offset = 0;
	while (index != string::npos) {
		Parameterized p = parse_parameterized(line.substr(index));
		ObjType type = static_cast<ObjType>(p.symbol);
		size_t id = get_id_from_parameterized(p);
		V pos = V((int)(index - x_offset), current_y_pos);
		S _size = (p.length == 1) ? S1 : S((int)p.length, S::SH);
		if (!try_extend_vertical_object(type, (int)id, pos, _size)) {
			ObjectData obj = { type, pos, _size, (int)id,{} };
			objects.push_back(obj);
		}
		x_offset += p.size() - p.length;
		index = line.find_first_not_of(' ', index + p.size());
	}
}

bool LevelParser::try_extend_vertical_object(ObjType type, int id, V pos, S size) {
	for (auto& obj : objects) {
		if (obj.type != type || (id != 0 && obj.id != id)) continue;

		if ((size == S1 || size.d == S::SV) && (
			obj.size == S1 && obj.position == pos - V(0, 1) ||
			obj.size.d == S::SV && obj.position == pos - V(0, obj.size.length)
			)) {
			if (obj.size == S1) { obj.size = S(2, S::SV); return true; }
			else if (obj.size.d == S::SV) { obj.size.setLength(obj.size.length + 1); return true; }
		}
	}
	return false;
}

void LevelParser::parse_object_data(const string& line) {
	int id;
	try {
		id = stoi(line);
	}
	catch (...) {
		id = get_id_from_parameterized(parse_parameterized(line));
	};
	current_property_id = id;
}

void LevelParser::parse_data(const string& line) {
	string key, value;
	key = line.substr(0, line.find(' '));
	value = line.substr(line.find(' ') + 1);
	if (current_property_id == 0) {
		room_properties[key] = value;
	}
	else {
		for (auto& obj : objects)
			if (obj.id == current_property_id)
				obj.properties[key] = value;
	}
}

int LevelParser::get_id_from_parameterized(const Parameterized& p) {
	if (p.params.size() < 1) return 0;
	try {
		return stoi(p.params[0]);
	}
	catch (...) {
		if (p.symbol == 'D' && p.params[0] == "N")
			return -1;
		else if (p.symbol == 'D' && p.params[0] == "P")
			return -2;
		else {
			return 0;
		}
	}
}

vector<string> LevelParser::parse_params(const string& str) {
	vector<string> params;
	size_t start = 0;
	size_t end = str.find(',');
	while (end != string::npos) {
		params.push_back(str.substr(start, end - start));
		start = end + 1;
		end = str.find(',', start);
	}
	params.push_back(str.substr(start));
	return params;
}

Parameterized LevelParser::parse_parameterized(const string& str) {
	Parameterized p = { str[0] };
	size_t open_paren = str.find('('),
		next_symbol = str.find_first_not_of(p.symbol, 1);

	if (next_symbol == string::npos)
		p.length = str.size();
	else if (next_symbol < open_paren)
		p.length = next_symbol;
	else {
		size_t close_paren = str.find(')', open_paren);
		if (close_paren == string::npos) {
			p.params = parse_params(str.substr(open_paren + 1));
			return p;
		}
		p.params = parse_params(str.substr(open_paren + 1, close_paren - open_paren - 1));
	}
	return p;
}

void LevelParser::parse_all_levels(GameView* game, const RiddleParser& riddles, const string& path) {
	vector<filesystem::directory_entry> files;
	for (const auto& entry : filesystem::directory_iterator(path)) {
		string filename = entry.path().string();
		if (filename.starts_with(".\\adv-world_") && filename.ends_with(".screen")) {
			string index = filename.substr(12, filename.size() - 19);
			try {
				int n = stoi(index);
				files.push_back(entry);
			}
			catch (...) {}
		}
	}

	if (files.size() == 0) throw runtime_error("No level files found.");

	sort(files.begin(), files.end(), [](const filesystem::directory_entry& a, const filesystem::directory_entry& b) {
		return a.path().string() < b.path().string();
	});

	for (const auto& entry : files) {
		LevelParser parser(entry.path().string(), riddles);
		parser.parse();
		parser.build_room(game);
	}

	if (game->current->p_doors.entry_point) {
		game->player1->setPosition(game->current->p_doors.entry_point->getPosition());
		game->player2->setPosition(game->player1->getDumpPosition(game->current));
		game->current->add_object(game->player1);
		game->current->add_object(game->player2);
		game->current->remove_object(game->current->p_doors.entry_point);
		game->current->p_doors.entry_point = nullptr;
		delete game->current->p_doors.entry_point;
	}
}

void RiddleParser::parse() {
	while (!file.eof()) {
		string line;
		getline(file, line);
		if (line.empty() || line.starts_with('\\')) continue;
		if (line.starts_with("?")) {
			// New riddle
			size_t space_index = line.find(' ');
			int id = stoi(line.substr(1, space_index));
			RiddleData riddle = { line.substr(space_index + 1) };
			riddle.answers.clear();
			riddles[id] = riddle;
			current = &riddles[id];
		}
		else if (line.starts_with("! ") && current) {
			current->answers.push_back(line.substr(2));
			current->correct_answer_index = current->answers.size() - 1;
		}
		else if (current) current->answers.push_back(line);
	}
}

#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "prelude.h"
#include "Vector.h"
#include "GameView.h"
#include "RiddleParser.h"

using namespace std;

struct Parameterized {
	char symbol;
	vector<string> params;
	size_t length = 1;
	size_t size() const { return length + (params.size() > 0 ? 2+params[0].size() : 0); }
};


class LevelParser;
struct ObjectData {
	ObjType type;
	V position;
	S size;
	short id;
	map<string, string> properties;

	MapObject* into_map_object(GameView* game, GameRoom* room, const LevelParser& parser) const;
};

class LevelParser {
	ifstream file;

	int legend_position;
	vector<ObjectData> objects;
	map<string, string> room_properties;

	short current_property_id = 0;
	int current_y_pos = 0;
public:
	const RiddleParser& riddle_parser;
	LevelParser(const string& filename, const RiddleParser& riddles) : riddle_parser(riddles) {
		file.open(filename);
	}

	void build_room(GameView* game) {
		auto room = game->add_room();
		for (auto& obj_data : objects) {
			ObjectData& od = obj_data;
			MapObject* obj = obj_data.into_map_object(game, room, *this);
			room->add_object(obj);
		}
	}

	void parse() {
		while (!file.eof()) {
			string line;
			getline(file, line);

			if (line.empty() || line.starts_with('\\')) continue;
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

	void parse_objects(const string& line) {
		size_t index = line.find_first_not_of(' ');
		size_t x_offset = 0;
		while (index != string::npos) {
			Parameterized p = parse_parameterized(line.substr(index));
			ObjType type = static_cast<ObjType>(p.symbol);
			int id = get_id_from_parameterized(p);
			V pos = V(index - x_offset, current_y_pos);
			S _size = (p.length == 1) ? S1 : S(p.length, S::SH);
			if (!try_extend_vertical_object(type, id, pos, _size)) {
				ObjectData obj = { type, pos, _size, id, {} };
				objects.push_back(obj);
			}
			x_offset += p.size() - p.length;
			index = line.find_first_not_of(' ', index + p.size());
		}
	}

	bool try_extend_vertical_object(ObjType type, int id, V pos, S size) {
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

	void parse_object_data(const string& line) {
		int id;
		try {
			id = stoi(line);
		}
		catch (...) {
			id = get_id_from_parameterized(parse_parameterized(line));
		};
		current_property_id = id;
	}

	void parse_data(const string& line) {
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

	static int get_id_from_parameterized(const Parameterized& p) {
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

	static vector<string> parse_params(const string& str) {
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

	static Parameterized parse_parameterized(const string& str) {
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
};
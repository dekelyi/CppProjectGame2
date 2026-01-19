#include <string>
#include <vector>
#include <filesystem>

#include "ObjTypes.h"
#include "GameView.h"
#include "Collectible.h"
#include "Door.h"
#include "Obstacle.h"
#include "Spring.h"
#include "Switch.h"
#include "Riddle.h"
#include "GameParser.h"

/**
 * Discover, parse and build all level files in `path` following the naming:
 * ".\\adv-world_<N>.screen".
 *
 * - Files are sorted lexicographically before loading.
 * - After all rooms are created, player placement and initialization are adjusted
 *   based on the entry/exit doors.
 *
 * Throws runtime_error when no matching level files are found.
 */
void GameParser::parse_files(const string& path) {
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
        GameParser::LevelParser* parser = new GameParser::LevelParser(entry.path().string(), riddles);
        parser->parse();
        parsers.push_back(parser);
    }
}

void GameParser::init_game(GameView* game) const {
    for (auto& parser : parsers) {
        parser->build_room(game);
    }

    if (game->current->p_doors.entry_point) {
        game->player1->setPosition(game->current->p_doors.entry_point->getPosition());
        game->player2->setPosition(game->player1->getNextPosition(game->current));
        game->current->add_object(game->player1);
        game->current->add_object(game->player2);
        game->current->remove_object(game->current->p_doors.entry_point);
        game->current->p_doors.entry_point = nullptr;
    }
    game->init_rooms();
}


std::vector<MapObject*> GameParser::LevelParser::ObjectData::into_map_objects(GameView* game, GameRoom* room, const GameParser::LevelParser& parser) const {
    switch (type) {
    case ObjType::PLAYER_1:
    case ObjType::PLAYER_2: {
        auto p = (type == ObjType::PLAYER_1) ? game->player1 : game->player2;
        p->setPosition(position);
        return { p };
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
        return { door };
    }
    case ObjType::OBSTCALE:
        return { new Obstacle(position, size) };
    case ObjType::SPRING:
        return { new Spring(position, size) };
    case ObjType::SWITCH_OFF:
        if (properties.count("do-open-door")) {
            string door_id = properties.at("do-open-door");
            auto cond = new Condition(Condition::SWITCH, 1);
            Door* door = (door_id == "N") ? room->p_doors.exit_point : room->p_doors.entry_point;
            door->conditions.push_back(cond);
            auto cb = SwitchDoor(room, door, cond);
            return { new Switch(position, cb) };
        }
        return { new Switch(position) };
    case ObjType::TORCH:
        int area;
        try { area = stoi(properties.at("area")); }
        catch (...) { area = 5; }
        return { new Torch(position, area) };
    case  ObjType::KEY:
        return { new Key(position, size) };
    case ObjType::RIDDLE: {
        try {
            Riddle::RiddleData rdata = parser.riddle_parser.riddles.at(id);
            return { new Riddle(position, size, rdata) };
        }
        catch (...) {
            return {};
        }
    }
    case ObjType::BOMB:
        return { new Bomb(position) };
    case ObjType::WALL: {
        vector<MapObject*> arr;
        for (unsigned x = 0; (int)x < size.getX(); x++)
            for (unsigned y = 0; (int)y < size.getY(); y++)
                arr.push_back(new MapObject(position + V(x, y), S1, (char)type));
        return arr;
    }
    default:
        return { new MapObject(position, size, (char)type) };
    }
}

/**
 * Build a room using parsed `objects` and `room_properties` and add it to `game`.
 *
 * - Uses width/height from room_properties when present (fallback to SIZE_X / SIZE_Y).
 * - Adds each created MapObject to the room.
 * - Applies special room-level properties (e.g. msg-on-enter, is_dark).
 */
void GameParser::LevelParser::build_room(GameView* game) const {
    auto get_prop = [&](string key, int _default) -> int {
        try {
            string val = this->room_properties.at(key);
            return stoi(val);
        }
        catch (...) {
            return _default;
        }
        };

    auto room = game->add_room(get_prop("width", GameView::DEFAULT_X), get_prop("height", GameView::DEFAULT_Y), legend_position);
    for (auto& obj_data : objects) {
        ObjectData od = obj_data;
        std::vector<MapObject*> objs = obj_data.into_map_objects(game, room, *this);
        for (auto obj : objs) room->add_object(obj);
    }
    for (const auto& [key, value] : room_properties) {
        if (key == "msg-on-enter") {
            room->msg->text = value;
            room->msg->ticks_left = GameView::MSG_TICKS;
        }
        else if (key == "is_dark") {
            if (value == "true" || value == "1") {
                room->p_torch.is_dark = true;
            }
        };
    }
}

/**
 * Parse the file associated with this GameParser::LevelParser.
 *
 * See GameParser::LevelParser::parse() declaration for line semantics.
 */
void GameParser::LevelParser::parse() {
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

/**
 * Parse tokens from a single map row and add / extend ObjectData entries.
 *
 * Behavior:
 * - Iterates over visible tokens; for each token calls `parse_parameterized`,
 *   determines type, id and size, and either extends an existing vertical object
 *   or adds a new ObjectData to `objects`.
 */
void GameParser::LevelParser::parse_objects(const string& line) {
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

/**
 * Check if a newly-read tile should extend an existing vertical object.
 *
 * If a match is found the existing object's size is increased and the function
 * returns true. Otherwise returns false.
 */
bool GameParser::LevelParser::try_extend_vertical_object(ObjType type, int id, V pos, S size) {
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

/**
 * Set the current object id context for subsequent property directives.
 *
 * Accepts either a numeric id or a parameterized token string from which the id
 * will be extracted.
 */
void GameParser::LevelParser::parse_object_data(const string& line) {
    int id;
    try {
        id = stoi(line);
    }
    catch (...) {
        id = get_id_from_parameterized(parse_parameterized(line));
    };
    current_property_id = id;
}

/**
 * Parse a key/value directive and apply it either as a room property or to all
 * objects matching the current_property_id.
 */
void GameParser::LevelParser::parse_data(const string& line) {
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

/**
 * Extract the integer id from a Parameterized token.
 *
 * - Returns 0 if there is no parameter or if conversion fails.
 * - Special door tokens: "D(N)" -> -1, "D(P)" -> -2.
 */
int GameParser::LevelParser::get_id_from_parameterized(const Parameterized& p) {
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

/**
 * Split a comma-separated string into individual parameter substrings.
 */
vector<string> GameParser::LevelParser::parse_params(const string& str) {
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

/**
 * Parse a parameterized token from a string.
 *
 * The returned `Parameterized` contains:
 * - symbol: the map character (first char of str)
 * - params: vector of strings extracted between parentheses if present
 * - length: number of characters consumed for this token when parsing the row
 */
GameParser::LevelParser::Parameterized GameParser::LevelParser::parse_parameterized(const string& str) {
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

/**
 * Parse riddles file and populate `riddles`.
 *
 * Format:
 * - Lines starting with '?' start a new riddle: "?<id> <title>";
 * - Lines starting with "! " add an answer and mark it as the correct answer;
 * - Other lines belonging to a riddle are stored as answers;
 * - Blank lines or lines starting with '\\' are ignored.
 */
void GameParser::RiddleParser::parse() {
    while (!file.eof()) {
        string line;
        getline(file, line);
        if (line.empty() || line.starts_with("\\")) continue;
        if (line.starts_with("?")) {
            // New riddle
            size_t space_index = line.find(' ');
            int id = stoi(line.substr(1, space_index));
            Riddle::RiddleData riddle = { line.substr(space_index + 1) };
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
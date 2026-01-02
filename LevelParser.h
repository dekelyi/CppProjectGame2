#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "prelude.h"
#include "Vector.h"
#include "GameView.h"
#include "RiddleParser.h"
#include "Riddle.h"

using namespace std;

struct Parameterized {
	char symbol;
	vector<string> params;
	size_t length = 1;
	inline size_t size() const { return length + (params.size() > 0 ? 2+params[0].size() : 0); }
};


class LevelParser;
struct ObjectData {
	ObjType type;
	V position;
	S size;
	int id;
	map<string, string> properties;

	MapObject* into_map_object(GameView* game, GameRoom* room, const LevelParser& parser) const;
};

class RiddleParser {
	ifstream file;
	RiddleData* current = nullptr;
public:
	map<int, RiddleData> riddles;
	RiddleParser(const string& filename) {
		file.open(filename);
	}

	void parse();
};

class LevelParser {
	ifstream file;

	int legend_position = 0;
	vector<ObjectData> objects;
	map<string, string> room_properties;

	int current_property_id = 0;
	unsigned current_y_pos = 0;

public:
	const RiddleParser& riddle_parser;
	inline LevelParser(const string& filename, const RiddleParser& riddles) : riddle_parser(riddles) {
		file.open(filename);
	}

	void build_room(GameView* game);

	void parse();

	void parse_objects(const string& line);

	bool try_extend_vertical_object(ObjType type, int id, V pos, S size);

	void parse_object_data(const string& line);

	void parse_data(const string& line);

	static int get_id_from_parameterized(const Parameterized& p);

	static vector<string> parse_params(const string& str);

	static Parameterized parse_parameterized(const string& str);

	static void parse_all_levels(GameView* game, const RiddleParser& riddles, const string& path = ".");
};
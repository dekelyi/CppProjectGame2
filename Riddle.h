#pragma once
#include <string>
#include <format>
#include <vector>
#include "ObjTypes.h"
#include "Object.h"
#include "player.h"
#include "Msg.h"

class GameRoom;

using std::string, std::vector;

class Riddle : public MapObject {
public:
	struct RiddleData {
		string question;
		vector<string> answers;
		size_t correct_answer_index = 0;
	};

	const RiddleData data;
	Riddle(V pos, S size, const RiddleData& riddle_data)
		: MapObject(pos, size, (char)ObjType::RIDDLE), data(riddle_data) {
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* obj, Move& move) override;
};


class RiddleMsg : public MsgWithInput {
	GameRoom* room;
	Riddle* riddle;
	Player* player;

public:
	inline RiddleMsg(GameRoom* _room, Riddle* _riddle, Player* p)
		: room(_room), riddle(_riddle), player(p) {
		// build question text
		text = riddle->data.question + "\n";
		for (size_t i = 0; i < riddle->data.answers.size(); i++) {
			text += std::format("  {}. {}\n", i + 1, riddle->data.answers[i]);
		}
	}

	virtual void on_input(string str) override;
};
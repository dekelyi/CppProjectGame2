#pragma once
#include <string>
#include <format>
#include <vector>
#include "ObjTypes.h"
#include "player.h"
#include "../Object.h"
#include "../Msg.h"
#include "../Events.h"

class GameRoom;

using std::string, std::vector;

/**
 * Riddle object placed on the map that opens a modal question when a player interacts.
 */
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


/**
 * Event emitted when a player interacts with a riddle.
 */
class EventRiddleInteracted : public Event {
public:
    const Riddle* riddle;
    const unsigned answer;

    EventRiddleInteracted(const MapObject* _actor, const Riddle* _riddle, const unsigned _answer)
        : Event(_actor), riddle(_riddle), answer(_answer) {}

    virtual string to_string() override {
        return std::format("OBJ {} RIDDLE INTERACTED WITH RIDDLE {}, ANSWER {}, CORRECT: {}",
            actor->getGlyph(), riddle->data.question,
            riddle->data.answers[answer], (answer == riddle->data.correct_answer_index) ? "yes" : "no");
    }
};


/**
 * Modal message used to present a riddle to the player and accept input.
 */
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
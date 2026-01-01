#include <fstream>
#include <map>
#include "Riddle.h"

using namespace std;

class RiddleParser {
	ifstream file;
	RiddleData* current = nullptr;
public:
	map<int, RiddleData> riddles;
	RiddleParser(const string& filename) {
		file.open(filename);
	}

	void parse() {
		while (!file.eof()) {
			string line;
			getline(file, line);
			if (line.empty() || line.starts_with('\\')) continue;
			if (line.starts_with("?")) {
				// New riddle
				int space_index = line.find(' ');
				int id = stoi(line.substr(1, space_index));
				RiddleData riddle = { line.substr(space_index+1)};
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
};
#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "ObjTypes.h"
#include "Vector.h"
#include "GameView.h"
#include "Riddle.h"

using namespace std;

/**
* Parse and build all level files found in `path`, adding them to `game`.
*
* Parameters:
* - game: GameView to receive parsed rooms.
* - riddles: pre-parsed RiddleParser containing riddle data for RIDDLE objects.
* - path: directory to scan for level files (default ".").
*
* Behavior:
* - Scans for files matching the naming convention ".\\adv-world_<N>.screen".
* - Sorts files lexicographically and loads them in order.
* - Throws `runtime_error` if no level files are found.
*/
class GameParser {
    /**
    * Parser of the riddles file that produces a map of RiddleData.
    */
    class RiddleParser {
        ifstream file;
        Riddle::RiddleData* current = nullptr;
    public:
        map<int, Riddle::RiddleData> riddles;

        RiddleParser(const string& filename) {
            file.open(filename);
        }

        /**
        * Parse the riddles file.
        *
        * Expected input format:
        * - Lines beginning with '?' start a new riddle: "?<id> <title...>"
        * - Lines beginning with "! " mark an answer that is considered correct.
        * - Other non-empty lines belonging to the current riddle are treated as answers.
        * - Lines that are empty or start with '\' are ignored as comments.
        *
        * Populates the `riddles` map keyed by riddle id.
        */
        void parse();
    };

    /**
    * Parser of a single level file.
    */
    class LevelParser {
        /**
        * Represent an object with parameters (ID) and a size
        */
        struct Parameterized {
            char symbol;
            vector<string> params;
            size_t length = 1;
            inline size_t size() const { return length + (params.size() > 0 ? 2 + params[0].size() : 0); }
        };

        /**
        * An object in the map
        */
        struct ObjectData {
            ObjType type;
            V position;
            S size;
            int id;
            map<string, string> properties;

            /**
             * Convert this ObjectData instance into one or more runtime MapObject instances.
             *
             * Parameters:
             * - game: pointer to the global GameView (used to reference players and shared resources).
             * - room: pointer to the GameRoom where the objects will be placed.
             * - parser: reference to the LevelParser that parsed this ObjectData (used to lookup riddles).
             *
             * Returns:
             * - A vector of newly created MapObject pointers representing this data entry.
             *   Ownership of the returned pointers is expected to be transferred to the caller/room.
             *
             * Notes:
             * - May return multiple objects for tiled objects (e.g. walls).
             * - May return zero objects when creation fails (e.g. missing riddle).
             */
            vector<MapObject*> into_map_objects(GameView* game, GameRoom* room, const LevelParser& parser) const;
        };

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

        /**
        * Build a GameRoom from the data parsed by this parser and add it to `game`.
        *
        * - Reads `room_properties` for room-level settings (e.g. width, height, msg-on-enter, is_dark).
        * - Instantiates MapObject-derived objects for each parsed `ObjectData` and adds them to the room.
        * - Special-cases player placement and door/switch wiring according to parsed properties.
        *
        * Parameters:
        * - game: the GameView to which the new room will be added.
        */
        void build_room(GameView* game) const;

        /**
        * Parse the entire room file associated with this LevelParser instance.
        *
        * Line conventions:
        * - Empty lines and lines starting with "//" are ignored.
        * - Lines beginning with ':' denote metadata or object property directives.
        *   - '::' lines set the current object id to attach subsequent property directives.
        *   - ':' lines set room-level properties or object properties for the current id.
        * - Lines beginning with 'L' mark the legend (y position).
        * - Other lines represent rows of map symbols and are parsed by `parse_objects`.
        */
        void parse();

        /**
        * Parse a single row of map symbols and append resulting ObjectData entries.
        *
        * Parameters:
        * - line: a string containing the row to parse. Examples: "W W W", "D(N)  P"
        *
        * The method reads consecutive parameterized tokens (see `parse_parameterized`),
        * computes their positions and sizes, and either adds a new ObjectData or tries
        * to extend an existing vertical object of the same type via `try_extend_vertical_object`.
        */
        void parse_objects(const string& line);

        /**
        * Attempt to extend an existing vertically stacked object instead of creating a new one.
        *
        * Parameters:
        * - type: the object type to match.
        * - id: id parameter for the object (0 means no id constraint).
        * - pos: proposed position of the new element.
        * - size: proposed size of the new element.
        *
        * Returns:
        * - true if an existing object was extended (and no new ObjectData should be created),
        *   false otherwise.
        */
        bool try_extend_vertical_object(ObjType type, int id, V pos, S size);

        /**
        * Set the current object id for subsequent `: key value` property directives.
        *
        * Parameters:
        * - line: the remainder of the directive after the leading "::".
        *
        * Behavior:
        * - If `line` contains a numeric id, it becomes the `current_property_id`.
        * - Otherwise it attempts to extract an id from a parameterized token.
        */
        void parse_object_data(const string& line);

        /**
        * Parse a property directive of the form "<key> <value>".
        *
        * Behavior:
        * - If `current_property_id == 0` the key/value is stored as a room property.
        * - Otherwise the key/value is assigned to all objects with matching id.
        *
        * Parameters:
        * - line: the directive string, e.g. "msg-on-enter Welcome to the cave".
        */
        void parse_data(const string& line);

        /**
        * Extract an integer id from a Parameterized entry.
        *
        * Behavior:
        * - Returns 0 when no numeric id exists.
        * - For door parameterization: "D(N)" returns -1, "D(P)" returns -2.
        */
        static int get_id_from_parameterized(const Parameterized& p);

        /**
        * Split a comma-separated parameter substring into individual parameters.
        *
        * Parameters:
        * - str: string containing parameters separated by commas (no surrounding parentheses).
        *
        * Returns:
        * - vector of parameter substrings, empty strings preserved if present.
        */
        static vector<string> parse_params(const string& str);

        /**
        * Parse a parameterized token from a map line.
        *
        * Examples:
        * - "W" -> symbol='W', params empty, length = 1
        * - "###" -> symbol='#', params empty, length = 3
        * - "D(N)" -> symbol='D', params={"N"}, length = 4 (including parentheses)
        *
        * Parameters:
        * - str: substring of the map row starting at a token boundary.
        *
        * Returns:
        * - A `Parameterized` describing the symbol, extracted params and the consumed length.
        */
        static Parameterized parse_parameterized(const string& str);
    };

    RiddleParser riddles;
    vector<LevelParser*> parsers;

    void parse_files(const string& path);
public:
    GameParser(const string& path = ".", const string& riddles_filename = "riddles.txt"): riddles(riddles_filename) {
        riddles.parse();
        parse_files(path);
    }

    ~GameParser() {
        for (auto p : parsers) delete p;
    }

    void init_game(GameView* game) const;
};
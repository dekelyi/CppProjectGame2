#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Vector.h"
#include "Console.h"
#include <functional>

using std::vector, std::string, std::endl, std::cout;

/**
 * A drawable object stored in the MapBuffer grid.
 */
struct DrawingObject {
    char glyph = ' ';
    std::string attr = "";

    bool operator==(const DrawingObject& other) const = default;
    explicit operator bool() const {
        // false when equals default-constructed object
        return *this != DrawingObject();
    }
};

/**
 * `MapBuffer` is a class that encapsulates a 2D buffer for rendering
 * characters with optional attributes (like colors). It manages the 
 * drawing of objects to a console, including support for a legend area.
 */
class MapBuffer {
    vector<vector<DrawingObject>> buffer; /**< 2D vector containing the drawable objects */
public:
    const int legend_y_pos = 0; /**< The Y position of the legend. */
    const unsigned X; /**< The width of the buffer. */
    const unsigned Y; /**< The height of the buffer. */

    /**
     * Construct a MapBuffer of size `x` by `y` with a legend line at `legend_pos`.
     * Allocates internal storage for drawing cells.
     */
    MapBuffer(unsigned x, unsigned y, int legend_pos) : X(x), Y(y), legend_y_pos(legend_pos) {
        buffer = { X, vector<DrawingObject>(Y) };
    }

    /**
     * Return the drawing object at `pos`. Returns a default (empty) object when
     * the position is out of bounds.
     */
    DrawingObject get_at(V pos) const {
        unsigned x = pos.getX(), y = pos.getY();
        if (x < 0 || y < 0 || x >= X || y >= Y)
            return DrawingObject();
        return buffer[x][y];
    }

    /**
     * Check whether a cell at `pos` contains a non-empty drawing object.
     */
    bool is_set(V pos) const {
        unsigned x = pos.getX(), y = pos.getY();
        if (x < 0 || y < 0 || x >= X || y >= Y)
            return false;
        return (bool)buffer[x][y];
    }

    /**
     * Set a single cell at `pos` to `obj`. Out of bounds positions are ignored.
     */
    void set_at(V pos, DrawingObject obj) {
        unsigned x = pos.getX(), y = pos.getY();
        if (x < 0 || y < 0 || x >= X || y >= Y)
            return;
        buffer[x][y] = obj;
    }

    /**
     * Fill a rectangular area starting at `pos` of size `size` with `obj`.
     */
    void set_at(V pos, V size, const DrawingObject& obj) {
        unsigned x = pos.getX(), y = pos.getY(), size_x = size.getX(), size_y = size.getY();
        for (unsigned dy = 0; dy < size_y; dy++)
            for (unsigned dx = 0; dx < size_x; dx++)
                set_at(V(x + dx, y + dy), obj);
    }

    /**
     * Render the buffer to the console. `draw_hud` is a callback used to
     * inject HUD/legend lines while drawing.
     */
    void draw(std::function<void(unsigned)> draw_hud) const {
        string border_h = string(X+2, CH_BLOCK_GREY);
        Console::gotoxy(V(0, 0));
        for (unsigned y = 0; y < Y; y++) {
            if (y == legend_y_pos) draw_hud(y ? y+1 : y);
            if (y == 0) cout << border_h << endl;
            cout << CH_BLOCK_GREY;
            for (unsigned x = 0; x < X; x++) {
                const DrawingObject& obj = buffer[x][y];
                cout << (ConsoleMenu::colors ? obj.attr : "") << obj.glyph << A_RESET;
            }
            cout << CH_BLOCK_GREY << endl;
        }
        cout << border_h << endl;
        if (legend_y_pos >= (int)Y) draw_hud(Y+2);
    }
};
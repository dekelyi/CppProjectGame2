#pragma once
class V {
	int x;
	int y;

public:
		V() : x(-1), y(-1) {}
		V(int _x, int _y) : x(_x), y(_y) {}
		inline int getX() const { return x; }
		inline int getY() const { return y; }
		inline void setX(int _x) { x = _x; }
		inline void setY(int _y) { y = _y; }
		inline V operator+(const V& other) const {
			return V(x + other.x, y + other.y);
		}
		inline V operator-(const V& other) const {
			return V(x - other.x, y - other.y);
		}
		inline bool operator==(const V& other) const {
			return (x == other.x) && (y == other.y);
		}
		inline bool operator!=(const V& other) const {
			return !(*this == other);
		}
};
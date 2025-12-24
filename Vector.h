#pragma once
#include <algorithm>
#include "prelude.h"

// A vector in R^2 represting position/size
class V {
	int x;
	int y;

public:
		V() : x(0), y(0) {}
		V(int _x, int _y) : x(_x), y(_y) {}

		V(const V& v) : x(v.getX()), y(v.getY()) {}

		virtual inline int getX() const { return x; }
		virtual inline int getY() const { return y; }
		inline void setX(int _x) { x = _x; }
		inline void setY(int _y) { y = _y; }

		inline V operator+(const V& other) const {
			return V(getX() + other.getX(), getY()  + other.getY());
		}
		inline V operator-(const V& other) const {
			return V(getX() - other.getX(), getY() - other.getY());
		}
		inline V operator*(unsigned short n) const {
			return V(getX() * n, getY() * n);
		}
		inline int operator*(const V& other) const {
			return (getX() * other.getX()) + (getY() * other.getY());
		}
		inline bool operator==(const V& other) const {
			return (getX() == other.getX()) && (getY() == other.getY());
		}
		inline bool operator!=(const V& other) const {
			return !(*this == other);
		}
		inline bool operator<(const V& other) const {
			return (getX() < other.getX()) && (getY() < other.getY());
		}

		inline bool is_same_direction(const V& other) const {
			return (*this * other) != 0;
		}

		inline bool is_out_of_bounds() const {
			return this->getX() < 0 || this->getY() < 0 || this->getX() >= SIZE_X || this->getY() >= SIZE_Y;
		}
};

// Size vector
class S : public V {
public:
	enum SD {
		SH, // horizontal
		SV, // vertical
		SB // block
	} d;
	int length;

	S(int _l, SD _d) : length(_l), d(_d) {}
	S(SD _d) : S(1, _d) {}

	virtual inline int getX() const override { return (d == SH || d == SB) ? length : 1; }
	virtual inline int getY() const override { return (d == SV || d == SB) ? length : 1; }

	void setX(int _x) = delete;
	void setY(int _x) = delete;

	void setLength(int _x) { length = _x; }

	inline bool is_ortho_direction(V v) {
		return (d == SH) ? v.getY() && !v.getX() :
			(d == SV) ? v.getX() && !v.getY() : true;
	}
};

inline S S1 = S(1, S::SB);

struct VS {
	V pos;
	V size;

	bool operator==(const VS& other) const {
		return pos == other.pos && size == other.size;
	}
};

inline VS is_in_bounds_dim(const V& o, const V& pos, const V& size, unsigned short area) {
	V s = pos - V(area, area);
	V e = pos + V(area, area);
	V te = o + size;
	V is = V(std::max(o.getX(), s.getX()), std::max(o.getY(), s.getY()));
	V ie = V(std::min(te.getX(), e.getX()), std::min(te.getY(), e.getY()));
	V diff = ie - is;
	V si = V(std::max(0, diff.getX()), std::max(0, diff.getY()));
	return { is,si };
}
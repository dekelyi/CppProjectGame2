#pragma once
#include "prelude.h"

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
		inline bool operator==(const V& other) const {
			return (getX() == other.getX()) && (getY() == other.getY());
		}
		inline bool operator!=(const V& other) const {
			return !(*this == other);
		}

		inline bool is_out_of_bounds() {
			return this->getX() < 0 || this->getY() < 0 || this->getX() >= SIZE_X || this->getY() >= SIZE_Y;
		}
};

class S : public V {
public:
	enum SD { SH, SV, SB } d;
	int length;

	S(int _l, SD _d) : length(_l), d(_d) {}
	S(SD _d) : S(1, _d) {}

	virtual inline int getX() const override { return (d == SH || d == SB) ? length : 1; }
	virtual inline int getY() const override { return (d == SV || d == SB) ? length : 1; }

	void setX(int _x) = delete;
	void setY(int _x) = delete;

	void setLength(int _x) { length = _x; }

	inline bool is_same_direction(V v) {
		return (d == SH) ? v.getX() && !v.getY() :
			(d == SV) ? v.getY() && !v.getX() : true;
	}

	inline bool is_ortho_direction(V v) {
		return (d == SH) ? v.getY() && !v.getX() :
			(d == SV) ? v.getX() && !v.getY() : true;
	}
};

inline S S1 = S(1, S::SB);
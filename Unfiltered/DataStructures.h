#pragma once
#include <iostream>
#include <math.h>
#include <assert.h>

// Contains position and offset to parent
struct AttatchmentPoint {
public:
	AttatchmentPoint() {}
	AttatchmentPoint(int* parentX, int* parentY) : parentX(parentX), parentY(parentY) {}
	AttatchmentPoint(int* parentX, int* parentY, float offsetX, float offsetY) : parentX(parentX), parentY(parentY), offsetX(offsetX), offsetY(offsetY) {}
	~AttatchmentPoint() { parentX = NULL; parentY = NULL; }

	void setParentPosition(int* parentX, int* parentY) { this->parentX = parentX; this->parentY = parentY; }
	void setOffset(float offsetX, float offsetY) { this->offsetX = offsetX; this->offsetY = offsetY; }

	int getTotalOffsetX() { 
		if (parentX == NULL || parentY == NULL) {
			return 0;
		}
		else {
			return *parentX + int(offsetX);
		}
	}
	int getTotalOffsetY() {
		if (parentX == NULL || parentY == NULL) {
			return 0;
		}
		else {
			return *parentY + int(offsetY);
		}
	}

	int* parentX = NULL;
	int* parentY = NULL;
	float offsetX = 0;
	float offsetY = 0;
};

// Float version of SDL_Point
struct Point {
public:
	float x = NULL;
	float y = NULL;
	Point() {}
	Point(float x, float y) : x(x), y(y) {}
	bool notNull() { return x != NULL && y != NULL; }
};

// std::pair apparently isn't optimized by all implementations of the C++ standard, so I'm just using this instead
// Source: https://www.youtube.com/watch?v=3LsRYnRDSRA
template <typename First, typename Second>
struct VX_S_FastPair {
public:
	First first;
	Second second;
};

// Vector containing x1,y1 x2,y2 and some helper functions for vector math, as well as printing
struct Vector2 {
public:
	float posx = 0; float posy = 0; // x1, y1
	float dirx = 0; float diry = 0; // x2, y2

	// For rays that are attached to a moving object, with a set direction
	float* px = NULL;
	float* py = NULL;
	float* dx = NULL;
	float* dy = NULL;

	bool dirKnown = false;

	Vector2() {}
	Vector2(float* posx, float* posy, float* dirx, float* diry) {
		if (posx == NULL || posy == NULL || dirx == NULL || diry == NULL) {
			std::cout << "Error initializing vector " << this << ": pos/dir ptr was null." << std::endl;
		}
		else {
			this->px = posx; this->py = posy;
			this->dx = dirx; this->dy = diry;
		}
	}
	Vector2(float posx, float posy) : posx(posx), posy(posy) {}
	Vector2(float posx, float posy, float dirx, float diry) : posx(posx), posy(posy), dirx(dirx), diry(diry) {}

	float magnitude() {
		return sqrt(pow((posx + dirx), 2) + pow((posy + diry), 2));
	}

	friend std::ostream& operator << (std::ostream& os, const Vector2& v) {
		os << "(x, y): pos(" << v.posx << ", " << v.posy << ") dir(" << v.dirx << ", " << v.diry << ')';
		return os;
	}
};
#pragma once

#include <math.h>
#include <assert.h>
#include <iostream>
#include <SDL.h>
#include "DataStructures.h"

class Ray {
public:

	Ray() {}
	Ray(float posx, float posy) {
		ray = Vector2(posx, posy);
	}
	Ray(float posx, float posy, float dirx, float diry) {
		ray = Vector2(posx, posy, dirx, diry);
	}
	Ray(float* posx, float* posy, float* dirx, float* diry) {
		ray = Vector2(posx, posy, dirx, diry);
	}

	Point cast(Vector2 vec);
	Vector2 ray = Vector2();
};
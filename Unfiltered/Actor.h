#pragma once
#include "../Entity/Entity.h"
#include <iostream>

enum class VX_E_ActorMoves {
	JUMP,
	LEFT,
	RIGHT,
	NONE
};

enum class VX_E_ActorStates {
	JUMPING,
	STANDING,
	FALLING,
	IDLE
};

class VX_C_Actor
{
public:
	VX_C_Actor();
	~VX_C_Actor();

	void setKeyStatePtr(const Uint8* ptr);
	// Handles continous presses (walk)
	VX_E_ActorMoves getInput();
	// Handles per action presses (jump)
	VX_E_ActorMoves getInput(SDL_Event e); // Needs to be called inside SDL event polling section of frame
	VX_C_Entity* getEntity();
	void setSprite(const char* filePath, VX_C_Window* targetWindow);
	void setPhysics(bool gravity);
	void move(int x, int y);
	// Handle per action pressed input
	void handlePerActionInput(SDL_Event e);
	// Handle continous input
	void handleContInput();

	// Moving helpers
	void walk();
	void jump();

	// Direction facing; 1 right, -1 left
	int direction = 1;
	bool renderable = false;
	bool physics = false;

private:
	const Uint8* keyStatePtr = NULL;
	VX_E_ActorMoves currentMove = VX_E_ActorMoves::NONE;
	VX_E_ActorStates state = VX_E_ActorStates::IDLE;
	VX_C_Entity entity;
};


#pragma once
#include "Window.h"
#include "GameManager.h"
#include "Debugger.h"

// Used to log performance stats, frame effifiency, time to calcualte per frame,
// how much time each part of the loop takes etc.
// Very barebones at the moment
class DebugPerformance : Debugger
{
public:
	void updateStore(VX_C_Window* targetWindow);
	void registerStore(VX_C_GameManager* manager);

	void logFrame(float time) {
		this->timePerFame = time;
	}

	void setPos(int x, int y) {
		this->posX = x; this->posY = y;
	}

private:
	float timePerFame = 0;
	int posX = 0;
	int posY = 0;
};

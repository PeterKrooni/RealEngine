#pragma once
#include <iostream>

class DebugComponent
{
public:
	DebugComponent(){}
	~DebugComponent() {
		hasPhysics = NULL;
		hasRenderable = NULL;
		hasParent = NULL;
		hasChildren = NULL;
		entityID = NULL;
		posX = NULL;
		posY = NULL;
		velX = NULL;
		velY = NULL;
	}

	void setBoolChecks(bool* hasPhysics, bool* hasRenderable, bool* hasParent, bool* hasChildren) {
		this->hasPhysics = hasPhysics;
		this->hasParent = hasParent;
		this->hasRenderable = hasRenderable;
		this->hasChildren = hasChildren;
	}

	void setID(int* entityID) {
		this->entityID = entityID;
	}

	void setPhysicsChecks(int* posX, int* posY, float* velX, float* velY) {
		this->posX = posX;
		this->posY = posY;
		this->velX = velX;
		this->velY = velY;
	}

	bool* hasPhysics = NULL;
	bool* hasRenderable = NULL;
	bool* hasParent = NULL;
	bool* hasChildren = NULL;
	int* entityID = NULL;
	int* posX = NULL;
	int* posY = NULL;
	float* velX = NULL;
	float* velY = NULL;
};


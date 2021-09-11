#pragma once
#include <vector>
#include "../../Entity/Entity.h"
#include "../../Unfiltered/Actor.h"
#include "../../Unfiltered/DataStructures.h"

class VX_C_PhysicsManager
{
public:
	VX_C_PhysicsManager();
	~VX_C_PhysicsManager();

	float gravityStrength = 0.4;

	void addEntity(VX_C_Entity* entity);
	void handleFrame();

	// Calculate coillision with point (intented for raycasting collision detection)
	void calculateCollision(VX_C_Entity* en1, VX_C_Entity* en2, VX_E_PhysicsHitboxContactPoints point);
	void calculateFriction(VX_C_Entity* en, VX_E_PhysicsHitboxContactPoints point);

private:
	std::vector<VX_C_Entity*> entities;
	std::vector<VX_S_PhysicsHitbox*> hitboxes;
};


#pragma once
#include "../Component Managers/RenderManager.h"
#include "../Component Managers/PhysicsManager.h"
#include "../../Map/MapReader.h"
#include "../../Blueprints/EntityBlueprints.h"
#include <stdlib.h>

enum class Managers {
	RENDER,
	PHYSICS,
};

class VX_C_GameManager
{
public:
	VX_C_GameManager(VX_C_Window* target);

	VX_C_RenderManager renderManager;
	VX_C_PhysicsManager physicsManager;

	void addEntity(VX_C_Entity* entity, Managers managerType);
	// Add dynamic texture to render manager
	void addDynamicTexture(VX_C_DynamicTexture* dynamicTexture);
	void processFrame();

	void sendTestEntity(VX_C_Window* target);

	void loadAutoEntities();
	// Add auto entities to managers
	void buildAutoEntities(VX_C_Window* target, std::vector<std::vector<VX_N_Blueprints::VX_E_EntityBlueprint>> entitiesToBuild);
	// Entities that aren't interacted with directly by the player; sky, ground etc.
	std::vector<VX_C_Entity> autoEntities;
private:
	int mNextEntityID = 1;
};


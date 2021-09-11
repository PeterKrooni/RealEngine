#include "GameManager.h"
#include <iostream>

VX_C_GameManager::VX_C_GameManager(VX_C_Window* target) {
	renderManager = VX_C_RenderManager(target);
	physicsManager = VX_C_PhysicsManager();
}

void VX_C_GameManager::addEntity(VX_C_Entity* entity, Managers managerType)
{
	switch (managerType)
	{
	case Managers::RENDER:
		renderManager.addEntity(entity);
		break;
	case Managers::PHYSICS:
		physicsManager.addEntity(entity);
		break;
	default:
		break;
	}
	if (!entity->entityIDIsSet()) {
		entity->setEntityID(this->mNextEntityID);
		this->mNextEntityID++;
	}
}

void VX_C_GameManager::addDynamicTexture(VX_C_DynamicTexture* dynamicTexture)
{
	this->renderManager.addDynamicTexture(dynamicTexture);
}

void VX_C_GameManager::processFrame()
{
	physicsManager.handleFrame();
	renderManager.handleFrame();
}

void VX_C_GameManager::sendTestEntity(VX_C_Window* target)
{
	int pos = autoEntities.size();
	int sendCount = 5;
	autoEntities.resize(pos + sendCount);
	for (int i = 0; i < sendCount; i++) {
		autoEntities[i + pos].addRenderableComponent(target, RenderLayer::FOREGROUND);
		autoEntities[i + pos].loadTexture("GROUND2020.png");
		addEntity(&autoEntities[i + pos], Managers::RENDER);
		autoEntities[i + pos].moveEntity(rand() % 10 + 1, rand() % 10 + 1);
		autoEntities[i + pos].addPhysicsComponent();
		autoEntities[i + pos].toggleGravity();
		autoEntities[i + pos].setVelocity(rand() % 10 + 1, rand() % 10 + 1);
		addEntity(&autoEntities[i + pos], Managers::PHYSICS);
	}
	std::cout << "Size: " << autoEntities.size() << "\n";
}

void VX_C_GameManager::loadAutoEntities()
{
}


void VX_C_GameManager::buildAutoEntities(VX_C_Window* target, std::vector<std::vector<VX_N_Blueprints::VX_E_EntityBlueprint>> entitiesToBuild)
{
	VX_C_Texture airText = VX_N_Blueprints::VX_C_EntityTextures::getAirTexture(target);
	VX_C_Texture groundText = VX_N_Blueprints::VX_C_EntityTextures::getGroundTexture(target);

	// Count total entities (inefficient but this function isnt called often anyways)
	int entityCount = 0;
	for (auto a : entitiesToBuild) {
		for (auto b : a) {
			entityCount++;
		}
	}

	int aapos = 0; // used to translate x, y position to autoEntities (only i) position. Increment each iteration of any entity
	autoEntities.resize(entityCount);
	for (int x = 0; x < entitiesToBuild.size(); x++) {
		for (int y = 0; y < entitiesToBuild[x].size(); y++) {

			VX_E_EntityTypes type = entitiesToBuild[x][y].entityType;
			bool gravity = entitiesToBuild[x][y].gravity;
			bool physics = entitiesToBuild[x][y].physics;
			VX_E_PhysicsState state = entitiesToBuild[x][y].physState;
			RenderLayer renderLayer = entitiesToBuild[x][y].layer;
			const char* groundPath = entitiesToBuild[x][y].groundPath.c_str();
			const char* airPath = entitiesToBuild[x][y].airPath.c_str();

			switch (type) {
			case VX_E_EntityTypes::AIR:
				autoEntities[aapos].addRenderableComponent(target, RenderLayer::BACKGROUND);
				autoEntities[aapos].loadTexture(airPath);
				addEntity(&autoEntities[aapos], Managers::RENDER);
				autoEntities[aapos].setRenderLayer(renderLayer);
				break;
			case VX_E_EntityTypes::GROUND:
				autoEntities[aapos].addRenderableComponent(target, RenderLayer::BACKGROUND);
				autoEntities[aapos].loadTexture(groundPath);
				addEntity(&autoEntities[aapos], Managers::RENDER);
				autoEntities[aapos].setRenderLayer(renderLayer);
				break;
			case VX_E_EntityTypes::NONE:
				break;
			default:
				break;
			}
			
			autoEntities[aapos].moveEntity(y*20, x*20);

			if (physics) {
				autoEntities[aapos].addPhysicsComponent();
				addEntity(&autoEntities[aapos], Managers::PHYSICS);
				if (gravity) {
					autoEntities[aapos].toggleGravity();
				}
				autoEntities[aapos].getPhysicsComponent()->setState(state);
			}
			aapos++;
		}
	}
	std::cout << "Entities built from mapfile: " << entityCount << std::endl;
}
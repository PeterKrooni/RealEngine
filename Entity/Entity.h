#pragma once
#include "../Components/RenderableComponent.h"
#include "../Components/EntityPositionComponent.h"
#include "../Components/PhysicsComponent.h"
#include "../Components/CompositeComponent.h"
#include "../Components/DebugComponent.h"

class VX_C_Entity
{
public:
	VX_C_Entity();
	// Add components
	void addRenderableComponent(VX_C_Window* target, RenderLayer layer = RenderLayer::NONE);
	void addEntityPositionComponent();
	void addPhysicsComponent();
	void addCompositeComponent();
	void addDebugComponent();
	// Remove components
	void removeDebugComponent();
	// Get component of type
	RenderableComponent* getRenderableComponent();
	EntityPositionComponent* getEntityPositionComponent();
	PhysicsComponent* getPhysicsComponent();
	DebugComponent* getDebugComponent();

	// Convenient helpers
	void moveEntity(int x, int y);
	int getEntityPositionX();
	int getEntityPositionY();

	// Helpers -- rendering
	void setRenderLayer(RenderLayer layer);
	void loadTexture(const char* filePath);
	void loadTexture(VX_C_Texture texture);
	void loadTextToTexture(std::string content, SDL_Color color);

	// Helpers -- physics
	void toggleGravity();
	void setVelocity(int x, int y);
	void markAsGround();
	VX_S_PhysicsHitbox* getHitbox();
	// Requires renderable with texture and physics component
	void updateHitbox();
	bool hasVelocity();

	// Helpers -- composite component
	void addChild(VX_C_Entity* child, float offsetX, float offsetY);
	void setOffsetToParent(float  offsetX, float offsetY);
	AttatchmentPoint getAttatchmentPoint();

	// Debuggers
	void toggleVelocityTracer();
	void togglePhysicsRayDisplay();
	void toggleHitboxOutline();

	// Checks
	bool hasPhysics = false;
	bool hasRenderable = false;
	bool hasDebug = false;
	bool hasChildren = false;
	bool hasParent = false;

	bool isActor = false;

	// Debug checks
	bool tracingVelocity = false;
	bool tracingRayDisplay = false;
	bool tracingHitboxOutline = false;

	// Entity metadata
	int getEntityID();
	void setEntityID(int id);
	bool entityIDIsSet();

private:
	// Store components
	RenderableComponent renderableComponent;
	EntityPositionComponent entityPositionComponent = EntityPositionComponent();
	PhysicsComponent physicsComponent;
	CompositeComponent compositeComponent;
	DebugComponent debugComponent;

	//  Error printing shorthands
	void printErrorWithPosition(std::string err);

	// Attatment point to any parents
	AttatchmentPoint mAttatchmentPoint;

	// Entity gets assigned entityID -1 upon initialization, game manager is responsible for giving final entity ID
	int mEntityID = -1;
};


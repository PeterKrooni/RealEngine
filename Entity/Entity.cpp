#include "Entity.h"
#include "../Components/PhysicsComponent.h"
#include <iostream>

VX_C_Entity::VX_C_Entity()
{
}

void VX_C_Entity::addRenderableComponent(VX_C_Window* target, RenderLayer layer)
{
	if (target == NULL) {
		this->printErrorWithPosition("addRenderableComponent: target window was null.");
	}else{
		this->renderableComponent = RenderableComponent(target, layer);
		this->hasRenderable = true;
	}
}

void VX_C_Entity::addEntityPositionComponent()
{
	this->entityPositionComponent = EntityPositionComponent();
}

void VX_C_Entity::addPhysicsComponent()
{
	this->physicsComponent = PhysicsComponent();

	this->physicsComponent.hitbox = VX_S_PhysicsHitbox(
		&entityPositionComponent.x, 
		&entityPositionComponent.y, 
		&renderableComponent.textureW,
		&renderableComponent.textureH);

	this->hasPhysics = true;
}

void VX_C_Entity::addCompositeComponent()
{
	this->compositeComponent = CompositeComponent();
}

void VX_C_Entity::addDebugComponent()
{
	this->debugComponent = DebugComponent();
	this->debugComponent.setBoolChecks(&hasPhysics, &hasRenderable, &hasParent, &hasChildren);
	this->debugComponent.setPhysicsChecks(&entityPositionComponent.x, &entityPositionComponent.y, &physicsComponent.velX, &physicsComponent.velY);
	this->debugComponent.setID(&mEntityID);
	this->hasDebug = true;
}

void VX_C_Entity::removeDebugComponent()
{
	this->debugComponent = DebugComponent();
	this->hasDebug = false;
}


RenderableComponent* VX_C_Entity::getRenderableComponent()
{
	return &this->renderableComponent;
}

EntityPositionComponent* VX_C_Entity::getEntityPositionComponent()
{
	return &this->entityPositionComponent;
}

PhysicsComponent* VX_C_Entity::getPhysicsComponent()
{
	return &this->physicsComponent;
}

DebugComponent* VX_C_Entity::getDebugComponent()
{
	return &this->debugComponent;
}

void VX_C_Entity::moveEntity(int x, int y)
{
	entityPositionComponent.setX(x);
	entityPositionComponent.setY(y);
}

int VX_C_Entity::getEntityPositionX()
{
	return entityPositionComponent.getX();
}

int VX_C_Entity::getEntityPositionY()
{
	return entityPositionComponent.getY();
}

void VX_C_Entity::setRenderLayer(RenderLayer layer)
{
	this->renderableComponent.setRenderLayer(layer);
}

void VX_C_Entity::loadTexture(const char* filePath)
{
	if (filePath == "") {
		this->printErrorWithPosition("loadTexture: filepath was empty.");
	}
	else {
		if (this->renderableComponent.targetIsSet()) {
			renderableComponent.loadTexture(filePath);
			// Update physics hitbox if texture was loaded after physics compontent was added
			if (hasPhysics) {
				physicsComponent.hitbox.w = &renderableComponent.textureW;
				physicsComponent.hitbox.h = &renderableComponent.textureH;
			}
		}
		else {
			this->printErrorWithPosition("loadtexture: target window in renderableComponent was null.");
		}
	}
	
}

void VX_C_Entity::loadTexture(VX_C_Texture texture)
{
	this->renderableComponent.loadTexture(texture);
}

void VX_C_Entity::loadTextToTexture(std::string content, SDL_Color color)
{
	this->renderableComponent.loadTextToTexture(content, color);
}

void VX_C_Entity::toggleGravity()
{
	if (hasPhysics) {
		this->physicsComponent.gravity = !this->physicsComponent.gravity;
	}
	else {
		this->printErrorWithPosition("toggleGravity: entity has no physics component.");
	}
}

void VX_C_Entity::setVelocity(int x, int y)
{
	physicsComponent.setVelX(x);
	physicsComponent.setVelY(y);
}

void VX_C_Entity::markAsGround()
{
	physicsComponent.setState(VX_E_PhysicsState::GROUND);
}

VX_S_PhysicsHitbox* VX_C_Entity::getHitbox()
{
	return &physicsComponent.hitbox;
}

void VX_C_Entity::updateHitbox()
{
	this->physicsComponent.hitbox = VX_S_PhysicsHitbox(
		&entityPositionComponent.x,
		&entityPositionComponent.y,
		&renderableComponent.textureW,
		&renderableComponent.textureH);
}

bool VX_C_Entity::hasVelocity()
{
	return physicsComponent.velX > 0 || physicsComponent.velY > 0;
}

void VX_C_Entity::addChild(VX_C_Entity* child, float offsetX, float offsetY)
{
	this->hasChildren = true;
	child->hasParent = true;
	child->mAttatchmentPoint = AttatchmentPoint(&entityPositionComponent.x, &entityPositionComponent.y, offsetX, offsetY);
	this->compositeComponent.addChild(child->getEntityID(), &child->mAttatchmentPoint);
}

void VX_C_Entity::setOffsetToParent(float offsetX, float offsetY)
{
}

AttatchmentPoint VX_C_Entity::getAttatchmentPoint()
{
	return this->mAttatchmentPoint;
}

void VX_C_Entity::toggleVelocityTracer()
{
	tracingVelocity = !tracingVelocity;
}

void VX_C_Entity::togglePhysicsRayDisplay()
{
	tracingRayDisplay = !tracingRayDisplay;
}

void VX_C_Entity::toggleHitboxOutline()
{
	tracingHitboxOutline = !tracingHitboxOutline;
}

int VX_C_Entity::getEntityID()
{
	if (this->mEntityID == -1) {
		std::cout << "Warning: tried to get entity ID from entity but entity was never assigned an ID." << std::endl;
	}
	return this->mEntityID;
}

void VX_C_Entity::setEntityID(int id)
{
	if (this->mEntityID != -1) {
		std::cout << "Warning: reassignment of enitityID for entity " << this->mEntityID << std::endl;
	}
	this->mEntityID = id;
}

bool VX_C_Entity::entityIDIsSet()
{
	return this->mEntityID != -1;
}

void VX_C_Entity::printErrorWithPosition(std::string err)
{
	std::cout << "Error: entity at (" << this->getEntityPositionX() << ", " << this->getEntityPositionY() << ") - " << err << std::endl;;
}

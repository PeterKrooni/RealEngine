#include "RenderManager.h"
#include <iostream>

VX_C_RenderManager::VX_C_RenderManager(VX_C_Window* target)
{
	this->mTargetWindow = target;
}

VX_C_RenderManager::VX_C_RenderManager()
{
}

VX_C_RenderManager::~VX_C_RenderManager()
{
	this->mTargetWindow = NULL;
}

void VX_C_RenderManager::addEntity(VX_C_Entity* entity)
{
	this->entities.push_back(entity);
}

void VX_C_RenderManager::addDynamicTexture(VX_C_DynamicTexture* dynamicTexture)
{
	this->dynamicTextures.push_back(dynamicTexture);
}

void VX_C_RenderManager::handleFrame()
{

	std::vector<VX_C_Entity*>	back;
	std::vector<VX_C_Entity*>	fore;
	std::vector<VX_C_Entity*>	entity;
	VX_C_Entity*			    actor = NULL;
	bool hasActor = false;

	for (VX_C_Entity* a : this->entities) {
		switch (a->getRenderableComponent()->getRenderLayer()) {
		case RenderLayer::NONE:
			break;
		case RenderLayer::BACKGROUND:
			back.push_back(a);
			break;
		case RenderLayer::FOREGROUND:
			fore.push_back(a);
			break;
		case RenderLayer::ENTITY:
			entity.push_back(a);
			break;
		case RenderLayer::ACTOR:
			hasActor = true;
			actor = a;
			break;
		default:
			break;
		}

		if (a->tracingVelocity){
			int centerX = a->getEntityPositionX() + (*a->getHitbox()->w / 2);
			int centerY = a->getEntityPositionY() + (*a->getHitbox()->h / 2);
			int velX = centerX + (a->getPhysicsComponent()->velX*2);
			int velY = centerY + (a->getPhysicsComponent()->velY*2);
			SDL_Color green(0, 255, 20, 255);
			this->mTargetWindow->drawLine(centerX, velX, centerY, velY, green);
		}
		if (a->tracingRayDisplay){
			for (auto ray : a->getPhysicsComponent()->getRays()) {
				SDL_Color blue(0, 0, 255, 255);
				this->mTargetWindow->drawLine(ray.ray.ray.posx, ray.ray.ray.dirx, ray.ray.ray.posy, ray.ray.ray.diry, blue);
			}
		}
		if (a->tracingHitboxOutline){
			SDL_Color red(255, 0, 0, 255);
			int x = *a->getHitbox()->x;
			int y = *a->getHitbox()->y;
			int w = *a->getHitbox()->w;
			int h = *a->getHitbox()->h;

			// top
			this->mTargetWindow->drawLine(x, x + w, y, y, red);
			// right
			this->mTargetWindow->drawLine(x+w, x + w, y, y+h, red);
			// bottom
			this->mTargetWindow->drawLine(x, x + w, y+h, y+h, red);
			// left
			this->mTargetWindow->drawLine(x, x, y, y+h, red);
		}
	}

	// Render in order
	if (!back.empty()) {
		for (VX_C_Entity* b : back) {
			int x = b->getEntityPositionComponent()->getX();
			int y = b->getEntityPositionComponent()->getY();
			if (b->hasParent) {
				x = b->getAttatchmentPoint().getTotalOffsetX();
				y = b->getAttatchmentPoint().getTotalOffsetY();

			}
			b->getRenderableComponent()->getTexture()->render(this->mTargetWindow, x, y);
		}
	}

	if (!fore.empty()) {
		for (auto f : fore) {
			int x = f->getEntityPositionComponent()->getX();
			int y = f->getEntityPositionComponent()->getY();
			if (f->hasParent) {
				x = f->getAttatchmentPoint().getTotalOffsetX();
				y = f->getAttatchmentPoint().getTotalOffsetY();

			}
			f->getRenderableComponent()->getTexture()->render(this->mTargetWindow, x, y);
		}
	}

	if (!entity.empty()){
		for (auto e : entity) {
			int x = e->getEntityPositionComponent()->getX();
			int y = e->getEntityPositionComponent()->getY();
			if (e->hasParent) {
				x = e->getAttatchmentPoint().getTotalOffsetX();
				y = e->getAttatchmentPoint().getTotalOffsetY();

			}
			e->getRenderableComponent()->getTexture()->render(this->mTargetWindow, x, y);
		}
	}

	if (hasActor){
		int x = actor->getEntityPositionComponent()->getX();
		int y = actor->getEntityPositionComponent()->getY();
		if (actor->hasParent) {
			x = actor->getAttatchmentPoint().getTotalOffsetX();
			y = actor->getAttatchmentPoint().getTotalOffsetY();

		}
		actor->getRenderableComponent()->getTexture()->render(this->mTargetWindow, x, y);
	}

	for (auto a : back) {
		a = NULL;
	}
	for (auto a : fore) {
		a = NULL;
	}
	for (auto a : entity) {
		a = NULL;
	}

	back.clear();
	fore.clear();
	entity.clear();

	actor = NULL;

	for (auto dt : dynamicTextures) {
		dt->update(mTargetWindow);
		dt->texture.render(this->mTargetWindow, dt->x, dt->y);
	}
}

void VX_C_RenderManager::toggleDebug()
{
	for (auto a : entities) {
		a->getRenderableComponent()->toggleDebug();
	}
}

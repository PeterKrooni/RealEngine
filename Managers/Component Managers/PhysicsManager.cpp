#include "PhysicsManager.h"
#include <iostream>
#include <map>

VX_C_PhysicsManager::VX_C_PhysicsManager()
{
}

VX_C_PhysicsManager::~VX_C_PhysicsManager()
{
	for (auto a : entities) {
		a = NULL;
	}
	entities.clear();
}

void VX_C_PhysicsManager::addEntity(VX_C_Entity* entity)
{
	this->entities.push_back(entity);
	this->hitboxes.push_back(entity->getHitbox());
}

void VX_C_PhysicsManager::handleFrame()
{
	for (auto a : entities) {
		// Skip collisions for entity if it's marked as ground
		if (a->getPhysicsComponent()->getState() != VX_E_PhysicsState::GROUND) {
			if (a->getPhysicsComponent()->gravity) {
				a->getPhysicsComponent()->applyForce(0, this->gravityStrength);
			}
			for (auto b : entities) {
				// Skip calculating if hitboxes are the same 
				if (a->getEntityID() != b->getEntityID()) {
					VX_S_PhysicsRayHit rayHit = a->getPhysicsComponent()->cast(*b->getHitbox());
					if (rayHit.hasHit()) {
						this->calculateCollision(a, b, rayHit.point);
					}
				}
			}
			a->moveEntity(a->getEntityPositionX() + a->getPhysicsComponent()->velX, a->getEntityPositionY() + a->getPhysicsComponent()->velY);
		}
	}
}

void VX_C_PhysicsManager::calculateCollision(VX_C_Entity* en1, VX_C_Entity* en2, VX_E_PhysicsHitboxContactPoints point)
{
	switch (point) {
	case VX_E_PhysicsHitboxContactPoints::BOTTOM:
		en1->moveEntity(en1->getEntityPositionX(), (*en2->getHitbox()->y) - (*en1->getHitbox()->h));
		en1->getPhysicsComponent()->setVelY(0);
		en1->getPhysicsComponent()->setState(VX_E_PhysicsState::STANDING);
		break;
	case VX_E_PhysicsHitboxContactPoints::TOP:
		en1->moveEntity(en1->getEntityPositionX(), (*en2->getHitbox()->y) + (*en2->getHitbox()->h));
		en1->getPhysicsComponent()->setVelY(0);
		en1->getPhysicsComponent()->setState(VX_E_PhysicsState::STANDING);
		break;
	case VX_E_PhysicsHitboxContactPoints::LEFT:
		en1->moveEntity((*en2->getHitbox()->x + *en2->getHitbox()->w), en1->getEntityPositionY());
		en1->getPhysicsComponent()->setVelX(0);
		break;
	case VX_E_PhysicsHitboxContactPoints::RIGHT:
		en1->moveEntity((*en2->getHitbox()->x - *en1->getHitbox()->w), en1->getEntityPositionY());
		en1->getPhysicsComponent()->setVelX(0);
		break;
	case VX_E_PhysicsHitboxContactPoints::NONE:
		break;
	}

	calculateFriction(en1, point);
}

void VX_C_PhysicsManager::calculateFriction(VX_C_Entity* en, VX_E_PhysicsHitboxContactPoints point)
{
	float en1_velX = en->getPhysicsComponent()->velX;
	float en1_velY = en->getPhysicsComponent()->velY;
	float en1_mass = en->getPhysicsComponent()->mass;
	float en1_friction = en->getPhysicsComponent()->friction;
	float en1_resistance = en1_mass * en1_friction;

	switch (point) {
	case VX_E_PhysicsHitboxContactPoints::BOTTOM:
		if (en1_velX < 0 && en1_velX + en1_resistance <= 0) {
			en->getPhysicsComponent()->applyForce(en1_resistance, 0);
		}
		else if (en1_velX > 0 && en1_velX - en1_resistance >= 0) {
			en->getPhysicsComponent()->applyForce(-en1_resistance, 0);
		}
		else if ((en1_velX < 0 && en1_velX + en1_resistance > 0) || (en1_velX > 0 && en1_velX - en1_resistance < 0)) {
			en->getPhysicsComponent()->setVelX(0);
		}
		break;
	case VX_E_PhysicsHitboxContactPoints::TOP:
		break;
	case VX_E_PhysicsHitboxContactPoints::LEFT:
		break;
	case VX_E_PhysicsHitboxContactPoints::RIGHT:
		break;
	case VX_E_PhysicsHitboxContactPoints::NONE:
		break;
	}
}
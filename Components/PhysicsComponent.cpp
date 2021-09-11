
#include "PhysicsComponent.h"
#include <iostream>

PhysicsComponent::PhysicsComponent()
{
}

PhysicsComponent::~PhysicsComponent()
{
}

void PhysicsComponent::applyForce(float x, float y)
{
	this->velX += x;
	this->velY += y;
	this->updateState();
}

void PhysicsComponent::setForce(float x, float y)
{
	this->velX = x;
	this->velY = y;
	this->updateState();
}

void PhysicsComponent::setVelX(float x)
{
	this->velX = x;
}

void PhysicsComponent::setVelY(float y)
{
	this->velY = y;
	this->updateState();
}

VX_E_PhysicsState PhysicsComponent::getState() {
	return this->state;
}

void PhysicsComponent::setState(VX_E_PhysicsState state) {
	this->state = state;
}

void PhysicsComponent::updateState()
{
	if (velY < 0) {
		this->setState(VX_E_PhysicsState::FALLING);
	}
	if (velY > 0) {
		this->setState(VX_E_PhysicsState::JUMPING);
	}
	if (velY == 0.0) {
		this->setState(VX_E_PhysicsState::STANDING);
	}
}

// TODO: Edge case where only 1 ray count
void PhysicsComponent::setRays() {

	// Clear all rays and resize to fit all rays on the component
	rays.clear();

	// Get length of longest side 
	const int longestSideLength = *hitbox.w > *hitbox.h ? *hitbox.w : *hitbox.h;

	// this should defo be a constant somewhere
	const int smallestHitboxSize = 20;

	// Amt. of rays to set on each side (conditional assignemnt to prevent integer division by 0
	const int raysPerSide = longestSideLength/smallestHitboxSize < 2 ? hitbox.raysCount : longestSideLength / smallestHitboxSize;
	// This is just for sake of modularity, all hitboxes have 4 sides as of now
	const int numberOfSides = 4;

	// Resize to fit all rays on each side
	rays.resize(numberOfSides*raysPerSide);

	// Divide each ray equally spread out on each side + edges
	// With hitbox.w = 100 and 4 rays per side, w/4 would be 25, but then we would end up with 5 portions:a<
	//	0, 25, 50, 75, 100
	// So instead we discard the 0th portion and divide the portions into raysPerSide-1, so we end up with
	//	0, 33, 66, 100 -> 4 portions with edges included
	float xPortions = *hitbox.w / (raysPerSide-1); 
	float yPortions = *hitbox.h / (raysPerSide-1);

	int pos = 0; // for keeping track of position in vector
	for (int i = 0; i < numberOfSides; i++) {
		for (int n = 0; n < raysPerSide; n++) {

			// These are here to avoid rays hitting hitboxes that are one pixel next to them
			bool atLowerEdge = n == 0;
			bool atUpperEdge = n + 1 == raysPerSide;

			// Side of origin of ray
			VX_E_PhysicsHitboxContactPoints p = VX_E_PhysicsHitboxContactPoints::NONE;

			// Default ray potiions
			float rayXPosition = 0;
			float rayYPosition = 0;
			float rayXDestination = 0;
			float rayYDestination = 0;

			// Hitbox positions - used for ray origins
			float hbXPosition = *this->hitbox.x;
			float hbYPosition = *this->hitbox.y;
			float hbWidth = *this->hitbox.w;
			float hbHeight = *this->hitbox.h;

			// Physics component velocity - used for ray destination
			float physCompXVelocity = this->velX;
			float physCompYVelocity = this->velY;

			// Data shared between top/bottom and left/right sides
			if (i == 0 || i == 2) {
				rayXPosition = hbXPosition + (n * xPortions);
				rayXPosition += atLowerEdge ? 1 : atUpperEdge ? -1 : 0;
			}
			if (i == 1 || i == 3) {
				rayYPosition = hbYPosition + (n * yPortions);
				rayYPosition += atLowerEdge ? 1 : atUpperEdge ? -1 : 0;
			}

			switch (i) {
			case 0: // top side
				rayYPosition = hbYPosition;
				rayXDestination = rayXPosition;
				rayYDestination = physCompYVelocity >= 0 ? rayYPosition : rayYPosition + physCompYVelocity;
				p = VX_E_PhysicsHitboxContactPoints::TOP;
				break;
			case 1: // right side 
				rayXPosition = hbXPosition + hbWidth;
				rayYDestination = rayYPosition;
				rayXDestination = physCompXVelocity <= 0 ? rayXPosition : rayXPosition + physCompXVelocity;
				p = VX_E_PhysicsHitboxContactPoints::RIGHT;
				break;
			case 2: // bottom side
				rayYPosition = hbYPosition + hbHeight;
				rayXDestination = rayXPosition;
				rayYDestination = physCompYVelocity <= 0 ? rayYPosition : rayYPosition + physCompYVelocity;
				p = VX_E_PhysicsHitboxContactPoints::BOTTOM;
				break;
			case 3: // left side
				rayXPosition = hbXPosition;
				rayYDestination = rayYPosition;
				rayXDestination = physCompXVelocity >= 0 ? rayXPosition : rayXPosition + physCompXVelocity;
				p = VX_E_PhysicsHitboxContactPoints::LEFT;
				break;
			}
			rays[pos] = VX_S_PhysicsBoxRay(p, Ray(rayXPosition, rayYPosition, rayXDestination, rayYDestination), i);
			pos++;
		}
	}

	/*
	// Adds raysCount rays on each side of the component
	int pos = 0;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < hitbox.raysCount; i++) {
			VX_E_PhysicsHitboxContactPoints p = VX_E_PhysicsHitboxContactPoints::NONE;
			Ray ray;
			int n = i;
			float x = 0;
			float y = 0;
			float dx = velX;
			float dy = velY;
			switch (j) {
			case 0:
				p = VX_E_PhysicsHitboxContactPoints::TOP;
				x = *hitbox.x + (i*xPortions);
				y = *hitbox.y;
				if (dy > 0) {
					dy -= dy;
				}
				break;
			case 1:
				p = VX_E_PhysicsHitboxContactPoints::RIGHT;
				x = *hitbox.x + *hitbox.w;
				y = *hitbox.y + (i * yPortions);
				if (dx < 0) {
					dx -= dx;
				}
				break;
			case 2:
				p = VX_E_PhysicsHitboxContactPoints::BOTTOM;
				x = *hitbox.x + (i * xPortions);
				y = *hitbox.y+*hitbox.h;
				if (dy < 0) {
					dy -= dy;
				}
				break;
			case 3:
				p = VX_E_PhysicsHitboxContactPoints::LEFT;
				x = *hitbox.x;
				y = *hitbox.y + (i * yPortions);
				if (dx > 0) {
					dx -= dx;
				}
				break;
			}

			ray.ray = Vector2(x, y, dx, dy);
			rays[pos] = VX_S_PhysicsBoxRay(p, ray, i);
			pos++;
		}
	}*/
}

VX_S_PhysicsRayHit PhysicsComponent::cast(VX_S_PhysicsHitbox target) {
	target.setBoundaryVectors();
	this->setRays();

	// Point + distance from ray origin
	std::vector<VX_S_FastPair<VX_S_FastPair<Point, int>, VX_E_PhysicsHitboxContactPoints>> collisions;

	// Loops through each of the rays, and cast them towards every side of the target hitbox
	for (VX_S_PhysicsBoxRay ray : this->rays) {
		VX_E_PhysicsHitboxContactPoints pointOfOrigin = ray.point; // What side the current ray is casted from

		// Intersection point;
		Point p;

		switch (pointOfOrigin) {
		case VX_E_PhysicsHitboxContactPoints::TOP:
			p = ray.cast(target.bottom);
			break;
		case VX_E_PhysicsHitboxContactPoints::RIGHT:
			p = ray.cast(target.left);
			break;
		case VX_E_PhysicsHitboxContactPoints::BOTTOM:
			p = ray.cast(target.top);
			break;
		case VX_E_PhysicsHitboxContactPoints::LEFT:
			p = ray.cast(target.right);
			break;
		}

		if (p.notNull()) {
			// Distance from ray origin to line intersection
			int distance = sqrt(pow(ray.ray.ray.posx - p.x, 2) + pow(ray.ray.ray.posy - p.y, 2));
			VX_S_FastPair<Point, int> point_dist_pair(p, distance);
			VX_S_FastPair<VX_S_FastPair<Point, int>, VX_E_PhysicsHitboxContactPoints> collision(point_dist_pair, pointOfOrigin);
			collisions.push_back(collision);
		}	
	}

	// Get collision with lowest abs value
	int cmin = INT_MAX;
	for (auto a : collisions) {
		if (a.first.second < cmin) {
			cmin = a.first.second;
		}
	}
	for (auto a : collisions) {
		if (a.first.second == cmin) {
			return VX_S_PhysicsRayHit(a.first.first, a.second);
		}
	}

	return VX_S_PhysicsRayHit(Point(NULL, NULL));
}

std::vector<VX_S_PhysicsBoxRay> PhysicsComponent::getRays()
{
	return rays;
}

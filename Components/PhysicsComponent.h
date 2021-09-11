#pragma once
#include "Ray.h"
#include "DataStructures.h"
#include <iostream>
#include <vector>

enum class VX_E_PhysicsState {
	JUMPING,
	FALLING,
	STANDING,
	GROUND, // Used to mark tiles as ground
	NONE
};

enum class VX_E_PhysicsHitboxContactPoints {
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	NONE
};

struct VX_S_PhysicsHitbox {
public:
	int* x = nullptr;
	int* y = nullptr;
	int* w = nullptr;
	int* h = nullptr;

	// Rays per side of hitbox
	// Higher count per side gives better collision detection accuracy, at the cost of performance
	int raysCount = 2;

	Vector2 top;
	Vector2 bottom;
	Vector2 left;
	Vector2 right;

	int* rotation = nullptr;

	VX_S_PhysicsHitbox() {}
	VX_S_PhysicsHitbox(int* x, int* y, int* w, int* h) {
		this->x = x; this->y = y; this->w = w; this->h = h;
	}
	~VX_S_PhysicsHitbox() {
		this->x = nullptr;
		this->y = nullptr;
		this->w = nullptr;
		this->h = nullptr;

		this->rotation = nullptr;
	}

	void setHitboxRays(int nPerSide) {
		this->raysCount = nPerSide;
	}

	// This could be done by having vectors store pointers to these positions instead, but that would require some rework
	// in how the physics hitbox itself is initialized (storing x+w and y+h somewhere in the entity itself, then giving the
	// hitbox a reference to these values)
	void setBoundaryVectors() {
		top = Vector2(*x, *y, *x + *w, *y);
		bottom = Vector2(*x, *y + *h, *x + *w, *y + *h);
		left = Vector2(*x, *y, *x, *y + *h);
		right = Vector2(*x + *w, *y, *x + *w, *y + *h);
	}

	// used to do hitbox calculations in some cases (like next frame iterations in physics manager)
	int nextX = 0;
	int nextY = 0;
	int nextW = 0;
	int nextH = 0;
};

// Ray attached to a hitbox
struct VX_S_PhysicsBoxRay {
public:
	Ray ray; // the ray object
	VX_E_PhysicsHitboxContactPoints point; // which side of the hitbox the ray is attached to 
	int n; // which of the rays on the hitbox this ray is (since raysCount from hitbox is per side)

	VX_S_PhysicsBoxRay() {}
	VX_S_PhysicsBoxRay(Ray ray) : ray(ray) {}
	VX_S_PhysicsBoxRay(VX_E_PhysicsHitboxContactPoints point, Ray ray) : point(point), ray(ray) {}
	VX_S_PhysicsBoxRay(VX_E_PhysicsHitboxContactPoints point, Ray ray, int n) : n(n), point(point), ray(ray) {}

	// Wrapper for ray cast
	Point cast(Vector2 target) { return ray.cast(target); }
};

struct VX_S_PhysicsRayHit {
public:
	Point hit = Point(NULL, NULL); // Coordinate of intersection - (NULL, NULL) if no intersection
	VX_E_PhysicsHitboxContactPoints point = VX_E_PhysicsHitboxContactPoints::NONE; // Which the ray that hit was casted from

	bool hasHit() {
		return hit.x != NULL && hit.y != NULL;
	}

	VX_S_PhysicsRayHit() {}
	VX_S_PhysicsRayHit(Point hit) : hit(hit) {}
	VX_S_PhysicsRayHit(Point hit, VX_E_PhysicsHitboxContactPoints point) : hit(hit), point(point) {}
};

class PhysicsComponent
{
public:
	// Continous forces (always applied)
	bool gravity = false;
	bool hasHitbox = true;
	float mass = 1;

	// Reistance forces
	float friction = 1;

	// Constants
	float walkSpeed = 10;
	float jumpStrength = 10;

	// Hitbox
	VX_S_PhysicsHitbox hitbox;

	// Velocity
	float velX = 0;
	float velY = 0;

	// Have a guess
	PhysicsComponent();
	~PhysicsComponent();

	// Force methods
	void applyForce(float x, float y);
	void setForce(float x, float y);
	void setVelX(float x);
	void setVelY(float y);

	// State methods
	VX_E_PhysicsState getState();
	void setState(VX_E_PhysicsState state);

	// Ray functions
	void setRays(); // update rays to have this->velocity range
	std::vector<VX_S_PhysicsBoxRay> getRays();
	VX_S_PhysicsRayHit cast(VX_S_PhysicsHitbox target); // find closest ray cast hit on another hitbox

private:
	// Call on each force application or velocity change
	void updateState();
	VX_E_PhysicsState state = VX_E_PhysicsState::NONE;
	std::vector<VX_S_PhysicsBoxRay> rays;
};


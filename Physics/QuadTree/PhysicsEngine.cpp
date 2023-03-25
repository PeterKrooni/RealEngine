#include "PhysicsEngine.h"
#include <iostream>

bool PhysicsEngine::colliding(RigidBody& rb1, RigidBody& rb2)
{
	bool coll =
		(rb1.pos_x < rb2.pos_x + rb2.w)
		&& (rb1.pos_x + rb1.w > rb2.pos_x)
		&& (rb1.pos_y < rb2.pos_y + rb2.h)
		&& (rb1.h + rb1.pos_y > rb2.pos_y);
	return coll;
}

bool PhysicsEngine::colliding(RigidBody& rb, StaticBody& sb)
{
	bool coll =
		(rb.pos_x < sb.pos_x + sb.w)
		&& (rb.pos_x + rb.w > sb.pos_x)
		&& (rb.pos_y < sb.pos_y + sb.h)
		&& (rb.h + rb.pos_y > sb.pos_y);
	return coll;
}

std::vector<RigidBody> PhysicsEngine::getBodies()
{
	std::vector<RigidBody> rb = rigidBodies;
	for (StaticBody b : staticBodies) {
		rb.push_back(RigidBody{0, b.pos_x, b.pos_y, 0, 0, b.w, b.h, 0 });
	}
	return rb;
}

int PhysicsEngine::addRigidBody(float pos_x, float pos_y, float vel_x, float vel_y, float w, float h, float mass)
{
	int id_for_body = new_id;
	rigidBodies.push_back(RigidBody{ id_for_body, pos_x, pos_y, vel_x, vel_y, w, h, mass });
	new_id++;
	return id_for_body;
}

int PhysicsEngine::addStaticBody(float pos_x, float pos_y, float w, float h)
{
	int id_for_body = new_id;
	staticBodies.push_back(StaticBody{ id_for_body, pos_x, pos_y, w, h });
	new_id++;
	return id_for_body;
}
int PhysicsEngine::update(FQT2 fqt)
{
	int num_iterations = 0;
	for (RigidBody& b1 : rigidBodies) {
		b1.pos_x += b1.vel_x;
		b1.pos_y += b1.vel_y;

		AABB range{ b1.pos_y, b1.pos_x, b1.h, b1.w };
		std::vector<int> extid = fqt.query(range, NULL, true);
		std::vector<RigidBody> rbQueried;
		for (int id : extid) {
			rbQueried.push_back(getRigidBody(id));
		}
		for (RigidBody& b2 : rbQueried) {
			if (b1.id != b2.id) {
				StaticBody ptbSB = rigidToStatic(b2);
				if (colliding(b1, ptbSB)) {
					staticBodyCollide(b1, ptbSB);
				}
			}
		}
		/*
		for (RigidBody& b2 : rigidBodies) {
			num_iterations++;
			if (b1.id != b2.id) {
				StaticBody ptbSB = rigidToStatic(b2);
				if (colliding(b1, ptbSB)) {
					staticBodyCollide(b1, ptbSB);
				}
			}
		}*/

		for (StaticBody sb : staticBodies) {
			if (colliding(b1, sb)) {
				staticBodyCollide(b1, sb);
			}
		}
	}
	return num_iterations;
}

RigidBody& PhysicsEngine::getRigidBody(int id)
{
	RigidBody rb;
	for (RigidBody& r : rigidBodies) {
		if (r.id == id) {
			return r;
		}
	}
	return rb;
}

void PhysicsEngine::rigidBodyCollide(RigidBody& rb1, RigidBody& rb2)
{
	StaticBody oldB1 = getOldBody(rb1);
	StaticBody oldB2 = getOldBody(rb2);

	float m1 = rb1.mass;
	float m2 = rb2.mass;
	float elasticity_of_collision = 0.777;
https://www.mathsisfun.com/physics/collisions.html
	float v1x = 
		((elasticity_of_collision * (m2 * (rb2.vel_x - rb1.vel_x))) + 
		(m1 * rb1.vel_x) + 
		(m2 * rb2.vel_x)) / (m1 + m2);

	float v1y =
		(elasticity_of_collision * (m2 * (rb2.vel_y - rb1.vel_y)) +
		(m1 * rb1.vel_y) +
		(m2 * rb2.vel_y)) / (m1 + m2);

	float v2x =
		((elasticity_of_collision * (m1 * (rb1.vel_x - rb2.vel_x))) +
		(m1 * rb2.vel_x) +
		(m2 * rb1.vel_x)) / (m1 + m2);

	float v2y =
		((elasticity_of_collision * (m1 * (rb1.vel_y - rb2.vel_y))) +
		(m1 * rb2.vel_y) +
		(m2 * rb1.vel_y)) / (m1 + m2);

	rb1.vel_x = v1x; 
	rb1.vel_y = v1y; 
	rb2.vel_x = v2x; 
	rb2.vel_y = v2y; 

	return;
}

void PhysicsEngine::staticBodyCollide(RigidBody& rb, StaticBody otherObj)
{
	StaticBody old = getOldBody(rb);
	StaticBody current = { 0, rb.pos_x, rb.pos_y, rb.w, rb.h };

	float boxLeft = current.pos_x;
	float boxRight = boxLeft + current.w;
	float boxTop = current.pos_y;
	float boxBottom = boxTop + current.h;

	float oldBoxLeft = old.pos_x;
	float oldBoxRight = oldBoxLeft + old.w;
	float oldBoxTop = old.pos_y;
	float oldBoxBottom = oldBoxTop + old.h;

	float otherObjLeft = otherObj.pos_x;
	float otherObjRight = otherObjLeft + otherObj.w;
	float otherObjTop = otherObj.pos_y;
	float otherObjBottom = otherObjTop + otherObj.h;

	bool left = false;
	bool right = false;
	bool top = false;
	bool bottom = false;

	if (oldBoxRight <= otherObjLeft && 
		boxRight >= otherObjLeft) {
		left = true;
	}
	if (oldBoxLeft >= otherObjRight && 
		boxLeft <= otherObjRight) {
		right = true;
	}
	if (oldBoxBottom <= otherObjTop && 
		boxBottom >= otherObjTop) {
		top = true;
	}
	if (oldBoxTop >= otherObjBottom && 
		boxTop <= otherObjBottom) {
		bottom = true;
	}

	if (left || right) {
		rb.vel_x = -rb.vel_x;
	}
	if (top || bottom) {
		rb.vel_y = -rb.vel_y;
	}
}

StaticBody PhysicsEngine::getOldBody(RigidBody& rb)
{
	return { 0, rb.pos_x - rb.vel_x, rb.pos_y - rb.vel_y, rb.w, rb.h };
}

StaticBody PhysicsEngine::rigidToStatic(RigidBody& rb)
{
	return { 0, rb.pos_x, rb.pos_y, rb.w, rb.h };
}

RigidBody PhysicsEngine::staticAsRigid(StaticBody& sb)
{
	return { 0, sb.pos_x, sb.pos_y, 0, 0, sb.w, sb.h };
}

void PhysicsEngine::addForce(int rb_id, float force_x, float force_y)
{
	for (RigidBody& b : rigidBodies) {
		if (b.id == rb_id) {
			b.vel_x += abs(b.vel_x) < speed_lim ? force_x : 0;
			b.vel_y += abs(b.vel_y) < speed_lim ? force_y : 0;
			return;
		}
	}
}

void PhysicsEngine::addPos(int rb_id, float pos_x, float pos_y)
{
	for (RigidBody& b : rigidBodies) {
		if (b.id == rb_id) {
			b.pos_x += pos_x;
			b.pos_y += pos_y;
			return;
		}
	}
}

void PhysicsEngine::setPos(int rb_id, float pos_x, float pos_y)
{
	for (RigidBody& b : rigidBodies) {
		if (b.id == rb_id) {
			b.pos_x = pos_x;
			b.pos_y = pos_y;
			return;
		}
	}	
	for (StaticBody& b : staticBodies) {
		if (b.id == rb_id) {
			b.pos_x = pos_x;
			b.pos_y = pos_y;
			return;
		}
	}
}



#pragma once
#include <vector>
#include <SDL_rect.h>
#include "QuadTree.h"
#include "FQT2.h"


struct Pnt {
	float x; float y;
};


struct StaticBody {
	int id;
	float pos_x, pos_y, w, h;
	Pnt pos;
};


struct RigidBody {
	int id;
	float pos_x, pos_y, vel_x, vel_y,  w, h;
	float mass;
};


enum Side {
	TOP,
	BOTTOM,
	RIGHT,
	LEFT
};

class PhysicsEngine
{	
private:
	float gravity = 0.001;
	float friction = 0.1;
	float speed_lim = 5;
	int new_id = 1;
	std::vector<RigidBody> rigidBodies;
	std::vector<StaticBody> staticBodies;
	bool colliding(RigidBody& rb1, RigidBody& rb2);
	bool colliding(RigidBody& rb, StaticBody& sb);
	StaticBody rigidToStatic(RigidBody& rb);
	RigidBody staticAsRigid(StaticBody& sb);
	void staticBodyCollide(RigidBody& rb, StaticBody sb);
	void rigidBodyCollide(RigidBody& rb1, RigidBody& rb2);
	StaticBody getOldBody(RigidBody& rb);
public: 
	std::vector<RigidBody> getBodies();
	int addRigidBody(float pos_x, float pos_y, float vel_x, float vel_y, float w, float h, float mass);
	int addStaticBody(float pos_x, float pos_y, float w, float h);
	int update(FQT2 fqt);
	RigidBody& getRigidBody(int id);
	void addForce(int rb_id, float force_x, float force_y);
	void addPos(int rb_id, float pos_x, float pos_y);
	void setPos(int rb_id, float pos_x, float pos_y);

	void drawByExtId(SDL_Renderer* renderer, std::vector<int> ids) {
		for (int i : ids) {
			RigidBody r = getRigidBody(i);
			SDL_Rect rect;
			rect.x = r.pos_x;
			rect.y = r.pos_y;
			rect.w = r.h;
			rect.h = r.w;
			SDL_SetRenderDrawColor(renderer, 34, 234, 34, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	void drawEntities(SDL_Renderer* renderer) {
		for (RigidBody r : rigidBodies) {
			SDL_Rect rect;
			rect.x = r.pos_x;
			rect.y = r.pos_y;
			rect.w = r.h;
			rect.h = r.w;
			SDL_SetRenderDrawColor(renderer, 234, 234, 234, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
};


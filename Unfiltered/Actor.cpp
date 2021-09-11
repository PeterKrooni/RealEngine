#include "Actor.h"

VX_C_Actor::VX_C_Actor()
{
}

VX_C_Actor::~VX_C_Actor()
{
	keyStatePtr = NULL;
}

void VX_C_Actor::setKeyStatePtr(const Uint8* ptr)
{
	this->keyStatePtr = ptr;
}

VX_E_ActorMoves VX_C_Actor::getInput()
{
	// RIGHT
	if (this->keyStatePtr[SDL_SCANCODE_D]) {
		return VX_E_ActorMoves::RIGHT;
	}
	// LEFT
	else if (this->keyStatePtr[SDL_SCANCODE_A]) {
		return VX_E_ActorMoves::LEFT;
	}
	return VX_E_ActorMoves::NONE;
}

VX_E_ActorMoves VX_C_Actor::getInput(SDL_Event e)
{
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_SPACE:
			return VX_E_ActorMoves::JUMP;
			break;
		default:
			break;
			}
	}
	return VX_E_ActorMoves::NONE;
}

VX_C_Entity* VX_C_Actor::getEntity()
{
	return &entity;
}

void VX_C_Actor::setSprite(const char* filePath, VX_C_Window* targetWindow)
{
	entity.addRenderableComponent(targetWindow, RenderLayer::ACTOR);
	entity.loadTexture(filePath);
	renderable = true;
}

void VX_C_Actor::setPhysics(bool gravity)
{
	entity.addPhysicsComponent();
	if (gravity)
		entity.toggleGravity();
	physics = true;
}

void VX_C_Actor::move(int x, int y)
{
	entity.moveEntity(x, y);
}

void VX_C_Actor::handlePerActionInput(SDL_Event e)
{
	VX_E_ActorMoves frameInput = getInput(e);

	if (frameInput == VX_E_ActorMoves::JUMP) {
		std::cout << "jump \n";
		if (entity.getPhysicsComponent()->getState() == VX_E_PhysicsState::STANDING) {
			this->jump();
		}
	}


}

void VX_C_Actor::handleContInput()
{
	VX_E_ActorMoves contInput = getInput();

	if (contInput == VX_E_ActorMoves::LEFT) {
		this->direction = -1;
		this->walk();
	}
	if (contInput == VX_E_ActorMoves::RIGHT) {
		this->direction = 1;
		this->walk();
	}
}

void VX_C_Actor::walk()
{
	int x = entity.getEntityPositionX();
	int y = entity.getEntityPositionY();
	this->entity.getPhysicsComponent()->setVelX(entity.getPhysicsComponent()->walkSpeed * this->direction);
}

void VX_C_Actor::jump()
{
	this->entity.getEntityPositionComponent()->setY(this->entity.getEntityPositionComponent()->getY() - 10);
	this->entity.getPhysicsComponent()->applyForce(0, -this->entity.getPhysicsComponent()->jumpStrength);
}

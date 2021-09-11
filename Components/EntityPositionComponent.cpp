#include "EntityPositionComponent.h"

void EntityPositionComponent::setX(int x)
{
	this->x = x;
}

void EntityPositionComponent::setY(int y)
{
	this->y = y;
}

int EntityPositionComponent::getX()
{
	return this->x;
}

int EntityPositionComponent::getY()
{
	return this->y;
}

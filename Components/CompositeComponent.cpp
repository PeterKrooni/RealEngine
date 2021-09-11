#include "CompositeComponent.h"

CompositeComponent::CompositeComponent()
{
}

void CompositeComponent::addChild(int childID, AttatchmentPoint* attatchmentPoint)
{
	mChildAttatchmentPointMap.emplace(childID, attatchmentPoint);
}

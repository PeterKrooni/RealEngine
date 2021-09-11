#include "ChildEntity.h"

VX_C_ChildEntity::VX_C_ChildEntity(VX_C_Entity* parent, float offsetX = 0, float offsetY = 0)
{
	if (parent == NULL) {
		printf("Warning: Tried to link child entity to parent, but parent was null.");
	}
	mAttatchmentPoint = AttatchmentPoint(&parent->getEntityPositionComponent()->x, &parent->getEntityPositionComponent()->y, offsetX, offsetY);
}

VX_C_ChildEntity::~VX_C_ChildEntity()
{
	this->mParent = NULL;
}

bool VX_C_ChildEntity::linkToParent(VX_C_Entity* parent)
{
	if (parent == NULL) {
		return false;
	}
	else {
		mAttatchmentPoint.setParentPosition(&parent->getEntityPositionComponent()->x, &parent->getEntityPositionComponent()->y);
	}
}

void VX_C_ChildEntity::setOffset(float offsetX, float offsetY)
{
	this->mAttatchmentPoint.setOffset(offsetX, offsetY);
}

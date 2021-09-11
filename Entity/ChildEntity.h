#pragma once
#include "Entity.h"
#include "DataStructures.h"
#include <stdio.h>

/*
	Entity attached to a parent entity
*/
class VX_C_ChildEntity : public VX_C_Entity
{
public:
	VX_C_ChildEntity(){}
	VX_C_ChildEntity(VX_C_Entity* parent, float offsetX, float offsetY);
	~VX_C_ChildEntity();

	// Returns false if not parent is null
	bool linkToParent(VX_C_Entity* parent);
	// Set offset from parent origins
	void setOffset(float offsetX, float offsetY);

private:
	VX_C_Entity* mParent = NULL;
	AttatchmentPoint mAttatchmentPoint;
};



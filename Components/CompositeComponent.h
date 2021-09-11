#pragma once
#include <vector>
#include <map>
#include "DataStructures.h"

/*
*	Used to link multiple entiities to one entitiy
*	Link child position to parent position with an offset
*/
class CompositeComponent {
public:
	CompositeComponent();
	void addChild(int childID, AttatchmentPoint* attatchmentPoint);
private:
	// Child entity to attatchment point mapping
	std::map<int, AttatchmentPoint*> mChildAttatchmentPointMap;
};




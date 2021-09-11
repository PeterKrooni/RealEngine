#include "DebugEntity.h"

DebugEntity::DebugEntity()
{
	mWhite.r = 0;
	mWhite.g = 0;
	mWhite.b = 0;
	mWhite.a = 255;
}

DebugEntity::~DebugEntity()
{
	this->mSelectedEntityData = NULL;
}

void DebugEntity::updateStore(VX_C_Window* targetWindow)
{
	if (mSelectedEntityData == NULL) {
		// Maybe log this in the mLogStore instead?
		printf("Warning: tried to update debugEntity store but component ptr was null.");
	}
	else {
		textStore.clear();

		int lineCount = 9;
		textStore.resize(lineCount);

		textStore[0] = VX_C_DynamicTexture(mSelectedEntityData->hasPhysics, this->mWhite);
		textStore[1] = VX_C_DynamicTexture(mSelectedEntityData->hasRenderable, this->mWhite);
		textStore[2] = VX_C_DynamicTexture(mSelectedEntityData->hasParent, this->mWhite);
		textStore[3] = VX_C_DynamicTexture(mSelectedEntityData->hasChildren, this->mWhite);
		textStore[4] = VX_C_DynamicTexture(mSelectedEntityData->entityID, this->mWhite);
		textStore[5] = VX_C_DynamicTexture(mSelectedEntityData->posX, this->mWhite);
		textStore[6] = VX_C_DynamicTexture(mSelectedEntityData->posY, this->mWhite);
		textStore[7] = VX_C_DynamicTexture(mSelectedEntityData->velX, this->mWhite);
		textStore[8] = VX_C_DynamicTexture(mSelectedEntityData->velY, this->mWhite);

		textStore[0].setPretext("Has physics: ");
		textStore[1].setPretext("Has renderable: ");
		textStore[2].setPretext("Has parent: ");
		textStore[3].setPretext("Has children: ");
		textStore[4].setPretext("Entity ID: ");
		textStore[5].setPretext("Position X: ");
		textStore[6].setPretext("Position Y: ");
		textStore[7].setPretext("Velocity X: ");
		textStore[8].setPretext("Velocity Y: ");

	}
}

void DebugEntity::registerStore(VX_C_GameManager* manager)
{
	for (int i = 0; i < textStore.size(); i++) {
		textStore[i].x = 50;
		textStore[i].y = 40 + i * 20;
		manager->addDynamicTexture(&textStore[i]);
	}
}

void DebugEntity::setEntity(VX_C_Entity* entity)
{
	entity->addDebugComponent();
	this->mSelectedEntityData = entity->getDebugComponent();
}

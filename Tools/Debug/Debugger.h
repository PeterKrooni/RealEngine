#pragma once
#include "../../Entity/Entity.h"
#include "../../SDL Wrappers/Window.h"
#include "../../Managers/System Managers/GameManager.h"
#include "../../Unfiltered/DynamicTexture.h"

/*
*	On screen text log used for debugging (e.g. displaying event logs, entity data or frame drops)
*/
class Debugger
{
public:
	Debugger() {
		mWrapperEntity = VX_C_Entity();
		mWrapperEntity.addCompositeComponent();
	}

	void setPosition(int x, int y) { mWrapperEntity.moveEntity(x, y); }
	virtual void updateStore(VX_C_Window* targetWindow) {}
	virtual void registerStore(VX_C_GameManager* manager) {}

	// Enitites for on screen text
	std::vector<VX_C_DynamicTexture> textStore;

	// Used as parent container for store
	VX_C_Entity mWrapperEntity;
};


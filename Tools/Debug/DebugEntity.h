#pragma once
#include <map>
#include "../../Components/DebugComponent.h"
#include "Debugger.h"

// Used to show data over a selected entity
class DebugEntity : public Debugger
{
public:
	DebugEntity();
	~DebugEntity();

	// Add debug component to entity and store ptr to the component
	void setEntity(VX_C_Entity* entity);
	void updateStore(VX_C_Window* targetWindow);
	void registerStore(VX_C_GameManager* manager);

private:

	// Text color
	SDL_Color mWhite;

	// Entity to display
	DebugComponent* mSelectedEntityData = NULL;
};


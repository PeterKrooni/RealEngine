#include "DebugPerformance.h"

void DebugPerformance::updateStore(VX_C_Window* targetWindow)
{
	textStore.resize(1);
	textStore[0] = VX_C_DynamicTexture(&timePerFame, SDL_Color(255, 255, 255, 255));
	textStore[0].setPretext("Ms per frame: ");
}

void DebugPerformance::registerStore(VX_C_GameManager* manager)
{
	textStore[0].x = posX;
	textStore[0].y = posY;
	manager->addDynamicTexture(&textStore[0]);
}

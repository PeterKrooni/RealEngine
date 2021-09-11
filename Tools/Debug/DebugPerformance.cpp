#include "DebugPerformance.h"

void DebugPerformance::updateStore(VX_C_Window* targetWindow)
{
	textStore.resize(1);
	SDL_Color black;
	black.r = 255;
	black.g = 255;
	black.b = 255;
	black.a = 255;
	textStore[0] = VX_C_DynamicTexture(&timePerFame, black);
	textStore[0].setPretext("Ms per frame: ");
}

void DebugPerformance::registerStore(VX_C_GameManager* manager)
{
	textStore[0].x = posX;
	textStore[0].y = posY;
	manager->addDynamicTexture(&textStore[0]);
}

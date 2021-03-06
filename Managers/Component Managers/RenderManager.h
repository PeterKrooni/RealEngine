#pragma once
#include <vector>
#include "../../Entity/Entity.h"
#include "../../Components/RenderableComponent.h"
#include "../../SDL Wrappers/Texture.h"
#include "../../Unfiltered/DynamicTexture.h"

class VX_C_RenderManager
{
public:
	VX_C_RenderManager(VX_C_Window* target);
	VX_C_RenderManager();
	~VX_C_RenderManager();

	void addEntity(VX_C_Entity* entity);
	void addDynamicTexture(VX_C_DynamicTexture* dynamicTexture);
	void handleFrame();

	// Draw outline around textures
	void toggleDebug();

private:
	VX_C_Window* mTargetWindow = NULL;
	std::vector<VX_C_Entity*> entities;
	std::vector<VX_C_DynamicTexture*> dynamicTextures;
};


#pragma once
#include "../Managers/System Managers/GameManager.h"
#include "../Entity/Entity.h"
#include "../Blueprints/EntityBlueprints.h"

class VX_C_MapReader
{
public:
	static std::vector<std::vector<VX_N_Blueprints::VX_E_EntityBlueprint>> parseMap(const char* filePath);

private:
	static const int configLineCount = 6;
	static const char tileSeparator = '.';
	static const char physicsDenoter = 'E';
	static const char gravityDenoter = 'F';
	static std::vector<std::string> readMap(const char* filePath);
	static VX_N_Blueprints::VX_E_EntityBlueprint parseTile(std::string tile);
};


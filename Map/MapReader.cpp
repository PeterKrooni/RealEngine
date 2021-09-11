#include "MapReader.h"
#include <fstream>
#include <iostream>

std::vector<std::vector<VX_N_Blueprints::VX_E_EntityBlueprint>> VX_C_MapReader::parseMap(const char* filePath)
{
	std::vector<std::vector<VX_N_Blueprints::VX_E_EntityBlueprint>> ret;

	int currentLine = 0;

	for (auto s : readMap(filePath)) {
		if (currentLine >= configLineCount) { // skip over initial config lines in map
			std::vector< VX_N_Blueprints::VX_E_EntityBlueprint> currentLine;
			std::string currentTile = "";
			for (char c : s) {
				if (c == tileSeparator) { // start parsing next tile'
					currentLine.push_back(parseTile(currentTile));
					currentTile = "";
				}
				else {
					currentTile += c;
				}
			}
			ret.push_back(currentLine);
		}
		currentLine++;
	}
	return ret;
}

std::vector<std::string> VX_C_MapReader::readMap(const char* filePath)
{
	std::vector<std::string> ret;
	std::string line;
	auto f = std::ifstream(filePath);
	while (std::getline(f, line)) {
		ret.push_back(line);
	}
	return ret;
}

VX_N_Blueprints::VX_E_EntityBlueprint VX_C_MapReader::parseTile(std::string tile)
{
	bool gravity = false;
	bool physics = false;
	VX_E_EntityTypes type = VX_E_EntityTypes::NONE;
	VX_E_PhysicsState physState = VX_E_PhysicsState::NONE;
	RenderLayer renderLayer = RenderLayer::NONE;

	if (tile.length() > 0) {
		switch (tile[0]) {
		case 'G':
			type = VX_E_EntityTypes::GROUND;
			break;
		case 'A':
			type = VX_E_EntityTypes::AIR;
			break;
		default:
			break;
		}
	}
	if (tile.length() > 1) {
		switch (tile[1]) {
		case physicsDenoter:
			physics = true;
			break;
		case gravityDenoter:
			physics = true;
			gravity = true;
			break;
		case 'D':
			physics = false;
			gravity = false;
			break;
		}
	}
	if (tile.length() > 2) {
		switch (tile[2]) {
		case 'N':
			break;
		case 'J':
			physState = VX_E_PhysicsState::JUMPING;
			break;
		case 'S':
			physState = VX_E_PhysicsState::STANDING;
			break;
		case 'F':
			physState = VX_E_PhysicsState::FALLING;
			break;
		case 'G':
			physState = VX_E_PhysicsState::GROUND;
			break;
		}
	}
	if (tile.length() > 3) {
		switch (tile[3]) {
		case 'N':
			break;
		case 'B':
			renderLayer = RenderLayer::BACKGROUND;
			break;
		case 'F':
			renderLayer = RenderLayer::FOREGROUND;
			break;
		case 'E':
			renderLayer = RenderLayer::ENTITY;
			break;
		case 'A':
			renderLayer = RenderLayer::ACTOR;
			break;
		}
	}

	return VX_N_Blueprints::VX_E_EntityBlueprint(gravity, physics, type, physState, renderLayer);
}

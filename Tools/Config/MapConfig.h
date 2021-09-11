#pragma once
#include <iostream>

/*
*	Rudementary config system for map
*	Needs to be able to add custom sprites, but this requires some rework in 
*	other places (namely in map reader and map class)
*/
class VX_C_MapSpriteConfig {
public:

	VX_C_MapSpriteConfig(){}

	void setGround(std::string groundPath) {
		this->ground = groundPath;
	}
	void setAir(std::string airPath) {
		this->air = airPath;
	}

	std::string ground;
	std::string air;
};
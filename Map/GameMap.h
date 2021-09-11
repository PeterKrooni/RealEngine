#pragma once
#include "Window.h"
#include "Texture.h"
#include <Vector>
#include <iostream>
#include <fstream>
#include <map>

enum class VX_EC_MapTileType {
	AIR,
	GROUND,
	NONE
};

class VX_C_MapTile {
public:
	VX_EC_MapTileType tileType = VX_EC_MapTileType::NONE;
};

class VX_C_GameMap {
private:
	VX_C_Window* mTargetWindow = NULL;
	int mapX, mapY;
	std::vector<std::vector<VX_C_MapTile>> tiles;

	void resizeMap(int newMapX, int newMapY) {
		mapX = newMapX;
		mapY = newMapY;
		tiles.resize(newMapX);
		for (std::vector<VX_C_MapTile>& t : tiles) {
			t.resize(newMapY);
		}
	}

	void readMapFromFile(const char* filePath) {
		std::string mapString;
		auto f = std::ifstream(filePath);
		int highestX = 0;
		int xpos = 0;
		int ypos = 0;
		// list of x, y to character 
		std::vector<std::pair<std::pair<int, int>, char>> mapTileCoords;
		while (getline(f, mapString)) {
			for (char c : mapString) {
				if (c == ',') {
					xpos++;
				}
				else {
					std::pair<int, int> tilePos; 
					tilePos.first = xpos;
					tilePos.second = ypos;
					std::pair<std::pair<int, int>, char> tile;
					tile.first = tilePos;
					tile.second = c;
					mapTileCoords.push_back(tile);
				}
			}
			if (xpos > highestX) {
				highestX = xpos;
			}
			xpos = 0;
			ypos++;
		}

		resizeMap(highestX+1, ypos);
		for (std::pair<std::pair<int, int>, char> tile : mapTileCoords) {
			VX_EC_MapTileType mtType = VX_EC_MapTileType::NONE;
			switch (tile.second) {
			case 'G':
				mtType = VX_EC_MapTileType::GROUND;
				break;
			case 'A':
				mtType = VX_EC_MapTileType::AIR;
			}

			setTile(tile.first.first, tile.first.second, mtType);
		}
	}

	VX_C_Texture mGroundTexture;
	VX_C_Texture mAirTexture;

public:
	VX_C_GameMap(int mapX, int mapY) {
		resizeMap(mapX, mapY);
	}

	// Window that map gets drawn to
	void setDrawTarget(VX_C_Window* target) {
		this->mTargetWindow = target;
	}
	VX_C_Window* getTargetWindow() {
		return this->mTargetWindow;
	}

	void loadMap(const char* filePath) {
		readMapFromFile(filePath);
	}

	// Initialize map tile textures
	void initializeTextures() {
		if (this->mTargetWindow == nullptr) {
			printf("Cannot initialize Game Map Textures: Draw target window was null.");
		}
		else {
			mAirTexture.loadFromFile("SKY2020.png", this->mTargetWindow->fetchRender());
			mGroundTexture.loadFromFile("GROUND2020.png", this->mTargetWindow->fetchRender());
		}
	}

	// Draw tiles in map
	void draw() {
		int xpos = 0;
		int ypos = 0;
		for (auto x : tiles) {
			ypos = 0;
			for (auto y : x) {
				switch (y.tileType) {
				case VX_EC_MapTileType::NONE:
					break;
				case VX_EC_MapTileType::GROUND:
					mGroundTexture.render(mTargetWindow, xpos * mGroundTexture.getWidth(), ypos * mGroundTexture.getHeight());
					break;
				case VX_EC_MapTileType::AIR:
					mAirTexture.render(mTargetWindow, xpos * mAirTexture.getWidth(), ypos * mAirTexture.getHeight());
					break;
				}
				ypos++;
			}
			xpos++;
		}
	}

	// Getters and setters for tiles
	VX_C_MapTile							getTile(int x, int y) {
		return tiles[x][y];
	}
	std::vector<std::vector<VX_C_MapTile>>	getAllTiles() {
		return tiles;
	}
	void									setTile(int x, int y, VX_EC_MapTileType newType) {
		VX_C_MapTile newTile;
		newTile.tileType = newType;
		tiles[x][y] = newTile;
	}
};





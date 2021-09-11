#include <vector>

#include "Map.h"
#include <iostream>

namespace VX_Map {

	VX_Map::VX_AC_AbastractMapData::VX_E_MapTileTypes VX_C_MapTile::returnTileType ()
	{
		return mTileType;
	}

	void VX_C_MapTile::setTileType (VX_E_MapTileTypes type)
	{
		mTileType = type;
	}
	VX_C_MapTile* VX_C_MapChunck::getTiles() { return *mChunckTiles; }

	std::vector<std::vector<VX_C_MapTile>> VX_C_MapChunck::returnTiles ()
	{
		// Resize tile 2d vector to tilesPerChunck size each
		std::vector<std::vector<VX_C_MapTile>> tiles;
		tiles.resize (tilesPerChunck);
		for (int i = 0; i < tiles.size (); i++) {
			tiles[i].resize (tilesPerChunck);
		}

		// Copy tiles 2d array to vector
		for (int i = 0; i < tilesPerChunck; i++) {
			for (int j = 0; j < tilesPerChunck; j++) {
				tiles[i][j] = mChunckTiles[i][j];
			}
		}
		return tiles;
	}
	VX_C_MapTile VX_C_MapChunck::returnTile (int x, int y) { return mChunckTiles[x][y]; }
	void VX_Map::VX_C_MapChunck::setTileType (int x, int y, VX_E_MapTileTypes type) 
	{ 
		mChunckTiles[x][y].setTileType(type); 
	}
	void VX_Map::VX_C_MapChunck::setAllTiles (VX_Map::VX_C_MapTile tiles[tilesPerChunck][tilesPerChunck]) 
	{ 
		for (int i = 0; i < tilesPerChunck; i++) {
			for (int j = 0; j < tilesPerChunck; j++) {
				mChunckTiles[i][j] = tiles[i][j];
			}
		}
	}
	void VX_C_MapChunck::setAllTiles (VX_E_MapTileTypes type)
	{
		for (int i = 0; i < tilesPerChunck; i++) {
			for (int j = 0; j < tilesPerChunck; j++) {
				VX_C_MapTile tile;
				tile.setTileType (type);
				mChunckTiles[i][j] = tile;
			}
		}
	}
	void VX_C_MapChunck::setAllTilesInRange (int x1, int y1, int x2, int y2, VX_E_MapTileTypes type)
	{
		for (int i = 0; i < abs (x1 - x2); i++) {
			for (int j = 0; j < abs (y1 - y2); j++) {
				setTileType (i, j, type);
			}
		}
	}
	void VX_C_MapChunck::setAbsoluteChunckCoordinates (int x, int y)
	{
		mChunckCoordinatesX = x; mChunckCoordinatesY = y;
	}
	int VX_C_MapChunck::getAbsoluteXChunckCoordinate ()
	{
		return mChunckCoordinatesX;
	}
	int VX_C_MapChunck::getAbsoluteYChunckCoordinate ()
	{
		return mChunckCoordinatesY;
	}
	void VX_C_MapChunck::setTargetWindow (VX_C_Window* targetWindow)
	{
		this->targetWindow = targetWindow;
	}
	void VX_C_MapChunck::bakeChunk ()
	{
		// Can skip baking if chunk already is part of a tile group that only shows one texture anyways
		if (tileGroupID == 0) {
			std::vector<std::vector<VX_C_MapTile>> chunkTiles = this->returnTiles ();
			std::vector<std::string> filePathsForTexturesToBake;

			for (int i = 0; i < chunkTiles.size (); i++) {
				for (int j = 0; j < chunkTiles[i].size (); j++) {
					VX_C_Texture t;
					switch (chunkTiles[i][j].returnTileType ()) {
					case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileTypes::AIR:
						filePathsForTexturesToBake.push_back ("SKY2020.png");
						break;
					case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileTypes::GROUND:
						filePathsForTexturesToBake.push_back ("GROUND2020.png");
						break;
					case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileTypes::WALL:
						filePathsForTexturesToBake.push_back ("WALL2020.png");
						break;
					case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileTypes::NONE:
						break;
					default:
						break;
					}
				}
			}
			int h = 0;
			if (chunkTiles.size () > 0) {
				h = chunkTiles[0].size ();
			}
			//bakedTexture.fuseTexture (targetWindow, filePathsForTexturesToBake, chunkTiles.size(), h);
			this->isBaked = true;
		}
	}
	VX_C_Texture VX_C_MapChunck::getBakedTexture ()
	{
		return bakedTexture;
	}
	void VX_C_MapChunck::setTileGroupID (int id)
	{
		tileGroupID = id;
	}
	int VX_C_MapChunck::getTileGroupID ()
	{
		return tileGroupID;
	}

	VX_C_Map::VX_C_Map (int chunksX, int chunksY, int mWindowLocationX, int mWindowLocationY)
	{
		this->mWindowLocationX = mWindowLocationX;
		this->mWindowLocationY = mWindowLocationY;
		this->chunksX = chunksX; 
		this->chunksY = chunksY;

		// For now this initalies a bunch of empty chuncks
		VX_C_MapChunck chunck;
		VX_E_MapTileTypes a = NONE;
		VX_Map::VX_C_MapTile tiles[tilesPerChunck][tilesPerChunck];
		
		// Resize x axis of chunck vector to map x count chunks size
		mChuncks.resize (chunksX);

		// Set all tiles in tile array to type a
		for (int i = 0; i < tilesPerChunck; i++) {
			for (int j = 0; j < tilesPerChunck; j++) {
				tiles[i][j].setTileType(a);
			}
		}

		// Place tile array in each chunck, and initialize absolute (screen) coordinates of chuncks
		for (int i = 0; i < chunksX; i++) {
			// Resize y axis of chunck vector to chunksTY
			mChuncks[i].resize (chunksY); 
			for (int j = 0; j < chunksY; j++) {
				mChuncks[i][j].setAllTiles (tiles);

				// This math is a bit weird, but essentially it translates to 
				// absolute map location to window, which chunck we're * each chunck size
				int absChunckX = mWindowLocationX + (i * (tilesPerChunck * tileSize));
				int absChunckY = mWindowLocationY + (j * (tilesPerChunck * tileSize));

				// This way we have found exactly where on the window this chunck is located
				mChuncks[i][j].setAbsoluteChunckCoordinates (absChunckX, absChunckY);
			}
		}
		// mChunks now has 20x20 chuncks all with var a type, and absolute window coordiantes
	}
	VX_C_Map::~VX_C_Map ()
	{
		// Save map chuncks to a file or something, idk what this game does yet so
		mTargetWindow = NULL;
	}
	void VX_C_Map::readMapFile () { /* TODO */ }
	void VX_C_Map::loadChuncks () { /* TODO SOONER RATHER THAN LATER */ }
	void VX_C_Map::loadTileSprites ()
	{
		mAIRtext.loadFromFile ("SKY2020.png", mTargetWindow->fetchRender());
		mGROUNDtext.loadFromFile ("GROUND2020.png", mTargetWindow->fetchRender ());
		mWALLtext.loadFromFile ("WALL_2020.png", mTargetWindow->fetchRender ());

		mSKYBOX_NORMALtext.loadFromFile("SKY_TILE_300300.png", mTargetWindow->fetchRender ());
		mSKYBOX_DARKtext.loadFromFile ("SKY300300.png", mTargetWindow->fetchRender ());
		mGROUND_NORMALtext.loadFromFile ("SKY300300.png", mTargetWindow->fetchRender ());
		mGROUND_DARKtext.loadFromFile ("SKY300300.png", mTargetWindow->fetchRender ());
		mWALL_NORMALtext.loadFromFile ("SKY300300.png", mTargetWindow->fetchRender ());
		mWALL_DARKtext.loadFromFile ("SKY300300.png", mTargetWindow->fetchRender ());
		mSINGLEtext.loadFromFile ("SKY300300.png", mTargetWindow->fetchRender ());
	}
	VX_C_MapChunck VX_C_Map::returnChunck (int x, int y)
	{
		return mChuncks[x][y];
	}
	VX_C_MapChunck* VX_C_Map::getChunck (int x, int y)
	{
		return &mChuncks[x][y];
	}
	void VX_C_Map::setTileInChunck (VX_C_MapChunck* chunck, VX_E_MapTileTypes type, int tilex, int tiley) 
	{
		chunck->setTileType (tilex, tiley, type);
	}
	void VX_C_Map::setTilesInChunck (VX_C_MapChunck* chunck, VX_E_MapTileTypes type, int tilex1, int tiley1, int tilex2, int tiley2)
	{
		chunck->setAllTilesInRange (tilex1, tiley1, tilex2, tiley2, type);
	}
	void VX_C_Map::setAllTilesInRange (int x1, int y1, int x2, int y2, VX_E_MapTileTypes type, VX_E_MapTileGroupTypes groupType)
	{

		bool groupTiles = (groupType != VX_E_MapTileGroupTypes::SINGLE && type == VX_E_MapTileTypes::NONE);
		int newGroupID = 1;

		// Init vector of maptiles with some type
		std::vector<VX_Map::VX_C_MapTile> tileGroup; 
		
		for (int i = 0; i < mChuncks.size (); i++) {
			for (int j = 0; j < mChuncks[i].size (); j++) {
				int chunckXStart = mChuncks[i][j].getAbsoluteXChunckCoordinate ();
				int chunckXEnd = chunckXStart + (tilesPerChunck * tileSize);
				int chunckYStart = mChuncks[i][j].getAbsoluteYChunckCoordinate ();
				int chunckYEnd = chunckYStart + (tilesPerChunck * tileSize);
				if (x1 <= chunckXStart
					&& x2 >= chunckXEnd
					&& y1 <= chunckYStart
					&& y2 >= chunckYEnd
					) {

					// TODO: Select only specific tiles inside chunck

					// Set all single tiles if no group, and bake to increase performance
					if (!groupTiles) {
						mChuncks[i][j].setAllTiles (type);
						mChuncks[i][j].bakeChunk ();
					}
					// Send tiles to vector if group
					else if (groupTiles) {
						mChuncks[i][j].isBaked = false; // Set baked flag to false
						mChuncks[i][j].setAllTiles (VX_E_MapTileTypes::NONE);
						// Get next group ID number from mapTileGroupCollection (gets the last element in the vectors first element and adds one, so ID's are 1, 2, 3...n)
						if (mapTileGroupCollection.size () > 0) {
							newGroupID = mapTileGroupCollection.back ().begin ()->first + 1;
						}
						mChuncks[i][j].setTileGroupID (newGroupID);
						for (std::vector<VX_Map::VX_C_MapTile> vm : mChuncks[i][j].returnTiles ()) {
							for (VX_Map::VX_C_MapTile m : vm) {
								tileGroup.push_back (m);
							}
						}
					}
					
				}
			}
		}

		// Create a map with the group type and tiles 
		if (groupTiles) {
			std::map<int, std::pair<VX_E_MapTileGroupTypes, std::vector < VX_Map::VX_C_MapTile >>> m;
			m.insert ({ newGroupID, std::make_pair (groupType, tileGroup) });
			mapTileGroupCollection.push_back(m);
		}
	}
	VX_Map::VX_C_MapTile VX_C_Map::getTileInChunck (VX_C_MapChunck* chunck, int tilex, int tiley)
	{
		VX_Map::VX_C_MapTile t = chunck->returnTile (tilex, tiley);
		return t;
	}
	void VX_C_Map::printMap ()
	{
		if(mChuncks.size()>0){
			std::cout << "Map contains: " << std::endl;
			for (int i = 0; i < mChuncks.size(); i++) {
				std::cout << "X axis row " << i << ": " << std::endl;
				if(mChuncks[i].size()>0){
					for (int j = 0; j < mChuncks[i].size(); j++) {
						string spacing = "  \t";
						if (j > 9) {
							spacing = " \t";
						}
std::cout << "Chunck nr. " << j << ", at coordinates x=" << mChuncks[i][j].getAbsoluteXChunckCoordinate() << ", y=" << mChuncks[i][j].getAbsoluteYChunckCoordinate () << spacing;
						VX_Map::VX_C_MapTile tilesInChunck[tilesPerChunck][tilesPerChunck] = { *mChuncks[i][j].getTiles () };
						int c = 0;
						int c0 = 0;
						for (int k = 0; k < tilesPerChunck; k++) {
							for (int l = 0; l < tilesPerChunck; l++) {
								VX_Map::VX_C_MapTile tile = tilesInChunck[k][l];
								if (tile.returnTileType() == VX_Map::VX_AC_AbastractMapData::AIR) {
									c++;
								}
								else if (tile.returnTileType() == VX_Map::VX_AC_AbastractMapData::GROUND){
									c0++;
								}
							}
						}	
						std::cout << "AIR x" << c << ", GROUND x " << c0 << std::endl;
					}
					std::cout << std::endl;
				}
				else {
					std::cout << "Map row " << i << "is empty." << std::endl;
				}
			}
		}
		else {
			std::cout << "Map is empty." << std::endl;
		}
	}
	void VX_C_Map::setDrawTarget (VX_C_Window* window)
	{
		mTargetWindow = window;
		for (int i = 0; i < mChuncks.size (); i++) {
			for (int j = 0; j < mChuncks[i].size (); j++) {
				mChuncks[i][j].setTargetWindow (window);
			}
		}
	}
	void VX_C_Map::loadMapSprites ()
	{
		loadTileSprites ();
	}
	void VX_C_Map::drawTiles ()
	{
		// Group IDS that have already been found
		std::vector<int> usedGroupIDS = {0}; // if no zero here then fucky wucky happens
		for (std::vector<VX_Map::VX_C_MapChunck> vm : mChuncks) {
			for (VX_Map::VX_C_MapChunck m : vm) {
				if (m.isBaked) {
					m.getBakedTexture ().render (mTargetWindow, m.getAbsoluteXChunckCoordinate(), m.getAbsoluteYChunckCoordinate());
				} /* Ideally, every texture should be baked to avoid quadruple for loops*/ else {
					std::vector<std::vector<VX_Map::VX_C_MapTile>> tiles = {m.returnTiles () };
					//std::map<int, bool> tileGroupsUsed; <-- not really the big performance hitter...

					for (int i = 0; i < tilesPerChunck; i++) {
						for (int j = 0; j < tilesPerChunck; j++) {

							// Rendering

							// Group tiles
							int tileGroupID = m.getTileGroupID ();
							bool groupUsed = std::count (usedGroupIDS.begin (), usedGroupIDS.end (), tileGroupID);

							if (!groupUsed) {
								usedGroupIDS.push_back (tileGroupID);
								for (std::map<int, std::pair<VX_E_MapTileGroupTypes, std::vector < VX_Map::VX_C_MapTile >>> map : mapTileGroupCollection) {
									if (map.contains (tileGroupID)) {

										int x = m.getAbsoluteXChunckCoordinate () + (i * tileSize);
										int y = m.getAbsoluteYChunckCoordinate () + (j * tileSize);

										// Group tile type of current group
										VX_E_MapTileGroupTypes t = map.at (tileGroupID).first;

										// width and height is shared for now
										int wh = map.at (tileGroupID).second.size () * tileSize;
										SDL_Rect targetRect = { x, y, wh, wh }; // See if deleteable, SDL stinky

										switch (t) {
										case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileGroupTypes::SKYBOX_DARK:
											break;
										case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileGroupTypes::SKYBOX_NORMAL:
											mSKYBOX_NORMALtext.render (mTargetWindow, x, y);
											break;
										case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileGroupTypes::GROUND_DARK:
											break;
										case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileGroupTypes::GROUND_NORMAL:
											break;
										case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileGroupTypes::WALL_DARK:
											break;
										case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileGroupTypes::WALL_NORMAL:
											break;
										case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileGroupTypes::SINGLE:
											break;
										default:
											break;
										}
									}
								}
							}
							// Individual tiles
							else {
								int x = m.getAbsoluteXChunckCoordinate () + (i * tileSize);
								int y = m.getAbsoluteYChunckCoordinate () + (j * tileSize);
								switch (tiles[i][j].returnTileType ()) {
								case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileTypes::AIR:
									mAIRtext.render (mTargetWindow, x, y);
									break;
								case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileTypes::GROUND:
									mGROUNDtext.render (mTargetWindow, x, y);
									break;
								case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileTypes::WALL:
									mWALLtext.render (mTargetWindow, x, y);
									break;
								case VX_Map::VX_AC_AbastractMapData::VX_E_MapTileTypes::NONE:
									break;
								default:
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	int VX_C_Map::getMapSizeX ()
	{
		return mChuncks.size ();
	}
	int VX_C_Map::getMapSizeY ()
	{
		if (mChuncks.size () > 0) {
			return mChuncks[0].size ();
		}
		else {
			return 0;
		}
	}
}
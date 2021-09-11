#pragma once
#include "Texture.h"
#include <map>
#include <variant>

using namespace std;

/*
*	Map
*	- All events relating to the map itself occurs here
*	TODO: Unsure where to store objects/items on the map... Here or another place... I guess here would make a lot of sense
*/
namespace VX_Map {

	/*
	*	Abstract map data
	*	- for some common consts
	*/
	class VX_AC_AbastractMapData {
	public:

		/*
		*	MapTileTypes
		*	- All types of tiles possible on the map
		*/
		enum VX_E_MapTileTypes {
			AIR,
			GROUND,
			WALL, 
			NONE
		};

		/*
		*	Directions for tiles
		*	- Neighbouring tiles
		*/
		enum VX_E_MapDirections {
			NORTH,
			NORTHEAST,
			EAST, 
			SOUTHEAST,
			SOUTH,
			SOUTHWEST,
			WEST,
			NORTHWEST
		};

		/*
		*	Tile Groups
		*	- For grouping tiles to display a texture that spans more than one tile
		*/
		enum VX_E_MapTileGroupTypes {
			SKYBOX_NORMAL,
			SKYBOX_DARK,
			GROUND_NORMAL,
			GROUND_DARK,
			WALL_NORMAL,
			WALL_DARK,
			SINGLE // same as none
		};

		// TODO: Observerer pattern on map events
		void pingObserver ();

		// WARNING: CHANGING THIS CONSTANT CAN DESTROY MAPS/MAP GENERATION? COMPLETELY
		// --- PROCEED WITH CAUTION
		const static int tilesPerChunck = 4; // Misleading name, this is sqrt of total tiles in chunck

		// Tile size in pixels
		// WARNING: Changing this might be dangerous; as previously stated in MapChunck, proceed with caution.
		const static int tileSize = 16;
	};

	/*
	*	Map Tile
	*	- Singular map tile, has tile size consts, contains getters and setters for the tile type
	*	TODO: Support multiple types of tile in the same tile (for transitions between tiles?)
	*/
	class VX_C_MapTile : VX_AC_AbastractMapData {
	public:

		void setTileType (VX_E_MapTileTypes type);
		VX_E_MapTileTypes returnTileType ();

	private:

		// Tile type
		VX_E_MapTileTypes mTileType = VX_E_MapTileTypes::NONE;

		// Tile coordinates in chunck
		int mTileCoordinatesX, mTileCoordinatesY = 0;
	};

	/*
	*	Map Chunck
	*	- Contains a set of tiles, this is mainly for storing tiles and manipulating them
	*	(mostly in the form of wrapper functions, but some tile gravity stuff too i guess? TODO)
	*/
	class VX_C_MapChunck : VX_AC_AbastractMapData, VX_C_MapTile {
	public:

		// NOTE!!! This doesn't return any proper data, don't use for now
		// I honestly also don't see why a pointer to the chunck tiles 2d array should be used anywhere.
		// While it is possible using typedef and stuff like that, it will just make for messy code that enables access 
		// to data that already have setters and getters to change the data in a safer way.
		// Might just delete this and be better off for it.
		// Return pointer to chunk tiles 2d array 
		VX_C_MapTile* getTiles ();

		// Return chunk tiles 2d array
		std::vector<std::vector<VX_C_MapTile>> returnTiles ();

		// Return specific tile in chunck at x, y
		VX_C_MapTile returnTile (int x, int y);

		// Change tile type to type at x, y
		void setTileType (int x, int y, VX_E_MapTileTypes type);

		// Set all chunck types 
		void setAllTiles (VX_C_MapTile tiles[tilesPerChunck][tilesPerChunck]);
		void setAllTiles (VX_E_MapTileTypes type);

		// Set all chunck types in range (x1<x2, y1<y2)
		void setAllTilesInRange (int x1, int y1, int x2, int y2, VX_E_MapTileTypes type);

		// WARNING:: This is really only to be used on initializing the map, moving this WILL mess up the entire map object if done carelessly
		// Set absolute screen coordinates of chunck 
		void setAbsoluteChunckCoordinates (int x, int y);
		// Just some getters for these values.
		int getAbsoluteXChunckCoordinate ();
		int getAbsoluteYChunckCoordinate ();

		// TODO: Need absolute coordinates for these
		// Get neighouring tile in direction dir
		VX_C_MapTile getNeighbouringTile (VX_E_MapDirections dir, VX_C_MapTile tile);

		// Get neighouring tile type in direction dir
		VX_E_MapTileTypes getNeighbouringTileType (VX_E_MapDirections dir, VX_C_MapTile tile);

		void setTargetWindow (VX_C_Window* targetWindow);

		void bakeChunk ();
		VX_C_Texture getBakedTexture ();
		bool isBaked = false;

		void setTileGroupID (int id);
		int getTileGroupID ();

	private:

		// This is the tile data for the chunck
		VX_C_MapTile mChunckTiles[tilesPerChunck][tilesPerChunck];

		// Baked texture of chunk
		VX_C_Texture bakedTexture;

		VX_C_Window* targetWindow = NULL;

		// Group id (if tiles are grouped)
		int tileGroupID = 0; // NOTE, groups with ID 0 are ignored, first valid has group id 1

		// Chunck coordinates in map
		int mChunckCoordinatesX, mChunckCoordinatesY = 0;
	};

	/*
	*	Map class
	*	- This is where chunck loading and saving etc. happens
	*/
	class VX_C_Map : VX_AC_AbastractMapData, VX_C_MapChunck {
		public:

			// Initialize map with chunksX * chunksY
			VX_C_Map (int chunksX = 0, int chunksY = 0, int mWindowLocationX = 0, int mWindowLocationY = 0);
			
			// Save map here?
			~VX_C_Map ();

			// Get chunck in map coordinate at chunck array location x, y
			VX_C_MapChunck returnChunck (int x, int y);
			VX_C_MapChunck* getChunck (int x, int y);

			// Set tile in chunck						 Tile type to set to     coordinates of tile inside chuncks
			void setTileInChunck (VX_C_MapChunck* chunck, VX_E_MapTileTypes type, int tilex, int tiley);

			// Set multiple tiles in chunck in range (x1, y1) U (x2, y2)
			void setTilesInChunck (VX_C_MapChunck* chunck, VX_E_MapTileTypes type, int tilex1, int tiley1, int tilex2, int tiley2);

			// Set all chunck types in range (x1<x2, y1<y2), where (x1, x2, y1, y2) coordinates are window coordinates
			void setAllTilesInRange (int x1, int y1, int x2, int y2, VX_E_MapTileTypes type = VX_E_MapTileTypes::NONE, VX_E_MapTileGroupTypes groupType = VX_E_MapTileGroupTypes::SINGLE); 

			// Get tile in chunck at tile coordinate
			VX_Map::VX_C_MapTile getTileInChunck (VX_C_MapChunck* chunck, int tilex, int tiley);
			
			// O(n^6), this doesnt really compute... like ever. TODO
			void printMap ();

			// Set map draw target
			void setDrawTarget (VX_C_Window* window);

			// Load map sprites, has to be done after window is initialized
			void loadMapSprites ();

			// Draw all tiles in mChuncks to screen
			void drawTiles ();

			// Get level size x and y
			int getMapSizeX ();
			int getMapSizeY ();

		private:
			
			// Read map file into mChuncks vector
			void readMapFile ();
			
			// Load new chuncks into mChuncks vector
			void loadChuncks ();

			// Load all map tile sprites into map
			void loadTileSprites ();

			VX_C_Texture mAIRtext;
			VX_C_Texture mGROUNDtext;
			VX_C_Texture mWALLtext;

			VX_C_Texture mSKYBOX_NORMALtext;
			VX_C_Texture mSKYBOX_DARKtext;
			VX_C_Texture mGROUND_NORMALtext;
			VX_C_Texture mGROUND_DARKtext;
			VX_C_Texture mWALL_NORMALtext;
			VX_C_Texture mWALL_DARKtext;
			VX_C_Texture mSINGLEtext;

			VX_C_Window* mTargetWindow = NULL;

			std::vector<std::vector<VX_C_MapChunck>> mChuncks;

			// TODO: Does this really need the map tiles? Maybe for when implementing only grouping certain tiles in chunck, not all. Keeping for now.
			std::vector<std::map<int, std::pair<VX_E_MapTileGroupTypes, std::vector < VX_Map::VX_C_MapTile >>>> mapTileGroupCollection;

			int mWindowLocationX, mWindowLocationY = 0;
			int chunksX, chunksY = 0;
	};
}
#ifndef FILE_IO_H
#define FILE_IO_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class FileIO
{
public:
	FileIO();
	~FileIO();

	void Init(const int height, const int width, const int Tileheight, const int Tilewidth,
		const int theMap_Height, const int theMap_Width, int theTileSize_Height=25, int theTileSize_Width=25);
	bool Read(const string mapName);
	
	vector<vector<int> > Map;

	int GetNumOfTiles_Height(void) { return NoofTiles_Height; };		// Get the number of tiles for height of the screen
	int GetNumOfTiles_Width(void)  { return NoofTiles_Width;  };		// Get the number of tiles for width of the screen
	int GetTileSize_Height(void) { return theTileSize_Height; };	    // Get height of tile
	int GetTileSize_Width(void)  { return theTileSize_Width;  };		// Get width of tile

	int getNumOfTiles_MapHeight(void) { return theNumOfTiles_MapHeight; };	// Get the number of tiles for height of the map
	int getNumOfTiles_MapWidth(void)  { return theNumOfTiles_MapWidth;	};	// Get the number of tiles for width of the map


protected:
	int ScreenHeight;
	int ScreeenWidth;
	int NoofTiles_Height;
	int NoofTiles_Width;

	int theTileSize_Height;
	int theTileSize_Width;

	int theMap_Height;				// The map's height
	int theMap_Width;				// The map's width
	int theNumOfTiles_MapHeight;	// Number of tiles in the map's height
	int theNumOfTiles_MapWidth;		// Number of tiles in the map's width

};

#endif // !FILE_IO_H

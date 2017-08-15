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

	int GetNumOfTiles_Height(void) { return m_iNoofTiles_Height; };		// Get the number of tiles for height of the screen
	int GetNumOfTiles_Width(void)  { return m_iNoofTiles_Width;  };		// Get the number of tiles for width of the screen
	int GetTileSize_Height(void) { return m_iTileSize_Height; };	    // Get height of tile
	int GetTileSize_Width(void)  { return m_iTileSize_Width;  };		// Get width of tile

	int getNumOfTiles_MapHeight(void) { return m_iNumOfTiles_MapHeight; };	// Get the number of tiles for height of the map
	int getNumOfTiles_MapWidth(void)  { return m_iNumOfTiles_MapWidth;	};	// Get the number of tiles for width of the map


protected:
	int m_iScreenHeight;
	int m_iScreeenWidth;
	int m_iNoofTiles_Height;
	int m_iNoofTiles_Width;

	int m_iTileSize_Height;
	int m_iTileSize_Width;

	int m_iMap_Height;				// The map's height
	int m_iMap_Width;				// The map's width
	int m_iNumOfTiles_MapHeight;	// Number of tiles in the map's height
	int m_iNumOfTiles_MapWidth;		// Number of tiles in the map's width

};

#endif // !FILE_IO_H

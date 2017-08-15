#include "FileIO.h"

FileIO::FileIO()
	: ScreenHeight(0),
	ScreeenWidth(0),
	NoofTiles_Height(0),
	NoofTiles_Width(0)
{
	Map.clear();
}

FileIO::~FileIO()
{
	Map.clear();
}

void FileIO::Init(const int height, const int width, const int Tileheight, const int Tilewidth,
	const int theMap_Height, const int theMap_Width, int theTileSize_Height, int theTileSize_Width)
{
	ScreenHeight = height;
	ScreeenWidth = width;
	NoofTiles_Height = Tileheight;
	NoofTiles_Width = Tilewidth;
	this->theMap_Height = theMap_Height;
	this->theMap_Width = theMap_Width;
	this->theTileSize_Height = theTileSize_Height;
	this->theTileSize_Width = theTileSize_Width;

	// Calculate the number of tiles for map height and map width
	NoofTiles_Height = (int)(theMap_Height / theTileSize_Height);
	NoofTiles_Width = (int)(theMap_Width / theTileSize_Width);

	// Resize the screen map array based on the number of tiles in the map height
	Map.resize(NoofTiles_Height);
	for (int i = 0; i < NoofTiles_Height; ++i)
	{
		// Resize each element inside screen map array based on the number of tiles in the map width
		Map[i].resize(NoofTiles_Width);
	}
}

bool FileIO::Read(const string mapName)
{
	int height = 0;
	int maxColumn = 0;

	ifstream file(mapName.c_str());
	if (file.is_open())
	{
		int i = 0, k = 0;
		while (file.good())
		{
			string aLineOfText = "";
			getline(file, aLineOfText);

			if (height >= NoofTiles_Height)
				break;

			// If this line is not a comment line, then process it
			if (!(aLineOfText.find("//*") == NULL) && aLineOfText != "")
			{
				if (height == 0)
				{
					// This is the first line of the map data file
					string token;
					istringstream iss(aLineOfText);
					while (getline(iss, token, ','))
					{
						// Count the number of columns
						maxColumn = atoi(token.c_str());
					}
					if (maxColumn != NoofTiles_Width)
						return false;
				}
				else
				{
					int theColumnCounter = 0;

					string token;
					istringstream iss(aLineOfText);
					while (getline(iss, token, ',') && (theColumnCounter<NoofTiles_Width))
					{
						Map[height][theColumnCounter++] = atoi(token.c_str());
					}
				}
			}

			height++;
		}
	}



	return true;
}

#include "FileIO.h"

FileIO::FileIO()
	: m_iScreenHeight(0),
	m_iScreeenWidth(0),
	m_iNoofTiles_Height(0),
	m_iNoofTiles_Width(0)
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
	m_iScreenHeight = height;
	m_iScreeenWidth = width;
	m_iNoofTiles_Height = Tileheight;
	m_iNoofTiles_Width = Tilewidth;
	m_iMap_Height = theMap_Height;
	m_iMap_Width = theMap_Width;
	m_iTileSize_Height = theTileSize_Height;
	m_iTileSize_Width = theTileSize_Width;

	// Calculate number of tiles for map height and map width
	m_iNoofTiles_Height = (int)(theMap_Height / theTileSize_Height);
	m_iNoofTiles_Width = (int)(theMap_Width / theTileSize_Width);

	// Resize the screen map array based on the number of tiles in map height
	Map.resize(m_iNoofTiles_Height);
	for (int i = 0; i < m_iNoofTiles_Height; ++i)
	{
		// Resize each element inside screen map array based on the number of tiles in map width
		Map[i].resize(m_iNoofTiles_Width);
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
			string Text = "";
			getline(file, Text);

			if (height >= m_iNoofTiles_Height)
				break;

			// If this line is not a comment line, then process it
			if (!(Text.find("//*") == NULL) && Text != "")
			{
				if (height == 0)
				{
					// This is the first line of the map data file
					string token;
					istringstream iss(Text);
					while (getline(iss, token, ','))
					{
						// Count the number of columns
						maxColumn = atoi(token.c_str());
					}

					if (maxColumn != m_iNoofTiles_Width)
						return false;
				}
				else
				{
					int ColumnCounter = 0;

					string token;
					istringstream iss(Text);
					while (getline(iss, token, ',') && (ColumnCounter< m_iNoofTiles_Width))
					{
						Map[height][ColumnCounter++] = atoi(token.c_str());
					}
				}
			}

			height++;
		}
	}



	return true;
}

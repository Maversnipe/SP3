#include "MapEditor.h"
#include "BrickBlock.h"
#include "GlassBlock.h"
#include "GrassBlock.h"
#include "MetalBlock.h"
#include "WoodBlock.h"
#include "Application.h"
#include <fstream>
#include <iostream>

MapEditor *MapEditor::instance = 0;

MapEditor::MapEditor()
	:currblockint(0)
	,b_isEditing(true)
	,brickPos(0,0,0)
{
}

MapEditor::~MapEditor()
{

}

void MapEditor::Init(Grid* grid)
{
	blockmanager = new Block*[totalnumblocks];
	blockmanager[0] = new Brickblock(grid);
	blockmanager[1] = new Glassblock(grid);
	blockmanager[2] = new Grassblock(grid);
	blockmanager[3] = new Metalblock(grid);
	blockmanager[4] = new Woodblock(grid);
}

Block * MapEditor::FetchBlocks(std::vector<Block*>& m_vBlocks, Grid* m_grid)
{
	for (std::vector<Block *>::iterator it = m_vBlocks.begin(); it != m_vBlocks.end(); ++it)
	{
		Block *go = (Block *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}

	Block *go = new Block(m_grid);
	m_vBlocks.push_back(go);

	go->active = true;
	return go;
}

void MapEditor::SaveMap(std::vector<Block*>& blocklist)
{
	int mapwidth = 64;
	int mapheight = 38;
	bool found = false;

	std::ofstream myfile;
	myfile.open("Maps//example.csv");
	myfile << "//";
	for (unsigned index = 1; index < mapwidth+1; ++index)
	{
		myfile << index << ",";
	}
	myfile << "\n";

	for (unsigned y = 2; y < mapheight+2; ++y)
	{
		for (unsigned x = 1; x < mapwidth+1; ++x)
		{
			for (unsigned i = 0; i < blocklist.size(); ++i)
			{
				if (blocklist[i]->pos.x / 4 == x && blocklist[i]->pos.y/4 == y && blocklist[i]->active)
				{
					myfile << blocklist[i]->Btype << ",";
					found = true;
					break;
				}
			}
			
			//if no blocks here
			if (!found)
			{
				myfile << "0,";
			}
			found = false;

		}
		myfile << "\n";
	}
	myfile.close();

	std::cout << "SAVE FILE" << std::endl;
}

void MapEditor::DeleteMap()
{
}

void MapEditor::PlaceBlock(std::vector<Block*>& blocklist, Grid* &m_grid)
{
	for (unsigned i = 0; i < blocklist.size(); ++i)
	{
		//check if there is any blocks on the same position
		if (blocklist[i]->pos == brickPos && blocklist[i]->active == true)
		{
			if (blocklist[i]->Btype == blockmanager[currblockint]->Btype)	//else if same block type
				blocklist[i]->active = false;	//block set active false

				return;
		}
	}
	//else if no blocks
	//place blocks
	Block* go = FetchBlocks(blocklist, m_grid);
	go->type = GameObject::GO_BLOCK;
	go->pos = brickPos;
	go->scale.Set(4.f, 4.f, 1.f);
	go->vel.Set(0, 0, 0);
	go->mass = 1.f;
	go->Btype = blockmanager[currblockint]->Btype;
	go->aabb.SetAABB(go->pos, go->scale);
	m_grid->Add(go);
	std::cout << go->pos << std::endl;
}

void MapEditor::SwitchBlock(int index)
{
	currblockint = index;
	if (currblockint >= totalnumblocks)
		currblockint = 0;
	if (currblockint == -1)
		currblockint = totalnumblocks - 1;
	std::cout << currblockint << std::endl;
}

bool MapEditor::GetIsEditing()
{
	return b_isEditing;
}

void MapEditor::SetIsEditing(bool editing)
{
	b_isEditing = editing;
}

void MapEditor::Update(double dt, Vector3 mousepos)
{
	//switch tools
	{
		static bool isQ = false;
		if (Application::IsKeyPressed('Q') && !isQ)
			isQ = true;
		else if (!Application::IsKeyPressed('Q') && isQ)
		{
			SwitchBlock(currblockint - 1);
			isQ = false;
		}
		static bool isE = false;
		if (Application::IsKeyPressed('E') && !isE)
			isE = true;
		else if (!Application::IsKeyPressed('E') && isE)
		{
			SwitchBlock(currblockint + 1);
			isE = false;
		}
	}

	int gridx, gridy;
	if ((int)mousepos.x % 4 >= 2)
	{
		gridx = (int)mousepos.x + (4 - (int)mousepos.x % 4);
	}
	else
	{
		gridx = (int)mousepos.x - ((int)mousepos.x % 4);
	}
	if ((int)mousepos.y % 4 >= 2)
	{
		gridy = (int)mousepos.y + (4 - (int)mousepos.y % 4);
	}
	else
	{
		gridy = (int)mousepos.y - ((int)mousepos.y % 4);
	}

	brickPos = Vector3(gridx, gridy, 0);//update to mouse pos
	blockmanager[currblockint]->pos = brickPos;
	blockmanager[currblockint]->scale.Set(4, 4, 1);
}

GameObject * MapEditor::GetCurrentBlock()
{
	return blockmanager[currblockint];
}

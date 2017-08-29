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
	:currblockint(1)
	, b_isEditing(true)
	, brickPos(0, 0, 0)
{
}

MapEditor::~MapEditor()
{

}

void MapEditor::Init()
{
	blockmanager = new Block*[totalnumblocks];
	blockmanager[0] = new Grassblock();
	blockmanager[1] = new Glassblock();
	blockmanager[2] = new Woodblock();
	blockmanager[3] = new Metalblock();
	blockmanager[4] = new Brickblock();
}

Block * MapEditor::FetchBlocks(std::vector<Block*>& m_vBlocks)
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

	Block *go = new Block();
	m_vBlocks.push_back(go);

	go->active = true;
	return go;
}

void MapEditor::SaveMap(std::vector<Block*>& blocklist)
{
	int mapwidth = 48;
	int mapheight = 28;
	bool found = false;

	std::ofstream myfile;
	myfile.open("Maps//example.csv");
	myfile << "//";
	for (unsigned index = 1; index < mapwidth + 1; ++index)
	{
		myfile << index << ",";
	}
	myfile << "\n";

	for (unsigned y = mapheight + 1; y > 1; --y)
	{
		for (unsigned x = 1; x < mapwidth + 1; ++x)
		{
			for (unsigned i = 0; i < blocklist.size(); ++i)
			{
				if (blocklist[i]->pos.x / 4 == x && blocklist[i]->pos.y / 4 == y && blocklist[i]->active)
				{
					std::cout << blocklist[i]->Btype + 1 << ",";
					myfile << blocklist[i]->Btype+1 << ",";
					found = true;
					break;
				}
			}

			//if no blocks here
			if (!found)
			{
				std::cout << "0,";
				myfile << "0,";
			}
			found = false;

		}
		std::cout << "\n";
		myfile << "\n";
	}
	myfile.close();

	std::cout << "SAVE FILE" << std::endl;
}

int MapEditor::DeleteMap(std::vector<Block*>& blocklist)
{
	int noDeleted =0;
	for (unsigned i = 0; i < blocklist.size(); ++i)
	{
		if (blocklist[i]->active && blocklist[i]->pos.y >= 20)
		{
			blocklist[i]->active = false;
			noDeleted++;
		}
	}
	return noDeleted;
}

bool MapEditor::PlaceBlock(std::vector<Block*>& blocklist)
{
	for (unsigned i = 0; i < blocklist.size(); ++i)
	{
		//check if there is any blocks on the same position
		if (blocklist[i]->pos == brickPos && blocklist[i]->active == true)
		{
			return false;
		}
	}
	//else if no blocks
	//place blocks
	Block* go = FetchBlocks(blocklist);
	go->type = GameObject::GO_BLOCK;
	go->pos = brickPos;
	go->scale.Set(4.f, 4.f, 1.f);
	go->vel.Set(0, 0, 0);
	go->mass = 1.f;
	go->Btype = blockmanager[currblockint]->Btype;
	go->aabb.SetAABB(go->pos, go->scale);
	return true;
}

bool MapEditor::RemoveBlock(std::vector<Block*>& blocklist)
{
	for (unsigned i = 0; i < blocklist.size(); ++i)
	{
		//check if there is any blocks on the same position
		if (blocklist[i]->pos == brickPos && blocklist[i]->active == true)
		{
			if (blocklist[i]->Btype == blockmanager[currblockint]->Btype)	//else if same block type
			{
				blocklist[i]->active = false;	//block set active false
				return true;
			}
		}
	}
	return false;

}

void MapEditor::SwitchBlock(int index)
{
	currblockint = index;
	if (currblockint >= totalnumblocks)
		currblockint = 1;
	if (currblockint == 0)
		currblockint = totalnumblocks - 1;
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
	if (gridy < 20)
		gridy = 20;

	brickPos = Vector3(gridx, gridy, 0);//update to mouse pos

	blockmanager[currblockint]->pos = brickPos;
	blockmanager[currblockint]->scale.Set(4, 4, 1);
}

GameObject * MapEditor::GetCurrentBlock()
{
	return blockmanager[currblockint];
}

#include "Grid.h"
#include "../GameObject.h"
#include "../Blocks.h"
#include "../GrassBlock.h"
#include "../GlassBlock.h"
#include "../WoodBlock.h"
#include "../MetalBlock.h"
#include "../BrickBlock.h"
#include "../Cannonball.h"
#include "../DrillProj.h"
#include "../missile.h"

Grid::Grid()
{
	int index = 0;
	for (int x = 0; x < NUM_CELLS_X; x++)
	{ // Clears the grid
		for (int y = 0; y < NUM_CELLS_Y; y++)
		{
			m_cells[x][y] = NULL;
		}
	}
}

Grid::~Grid()
{
}

void Grid::Add(GameObject* GO)
{
	// To determine which grid cell the Game Object is in
	int cellX = (int)((GO->pos.x - 2.f) / (float)CELL_SIZE);
	int cellY = (int)((GO->pos.y - 6.f) / (float)CELL_SIZE);

	GameObject* OriginalFront = m_cells[cellX][cellY];
	if (OriginalFront != NULL)
	{
		OriginalFront->prev_ = GO;
	}

	// Add to front of the list of the cell that the object is in
	GO->prev_ = NULL;
	GO->next_ = OriginalFront;
	m_cells[cellX][cellY] = GO;

	GO->m_iCurrCellX = cellX;
	GO->m_iCurrCellY = cellY;
}

void Grid::Remove(GameObject* GO)
{
	// To remove the Object accordingly
										// If A -> B -> C, where B is GO, A is GO->prev_ and C is GO->next_
	GameObject* BeforeGO = GO->prev_;
	GameObject* AfterGO = GO->next_;
	if (GO->prev_ != NULL)
	{
		BeforeGO->next_ = GO->next_; // This assigns A's next value to be C instead of B
	}
	if (GO->next_ != NULL)
	{
		AfterGO->prev_ = GO->prev_; // This assigns C's previous value to be A instead of B
	}

	if (GO == m_cells[GO->m_iCurrCellX][GO->m_iCurrCellY])
	{
		m_cells[GO->m_iCurrCellX][GO->m_iCurrCellY] = AfterGO; // If B is the head of the linked list, change head to B->next_
	}

	GO->prev_ = NULL;
	GO->next_ = NULL;
}

void Grid::Move(GameObject* GO)
{
	int newX = (int)((GO->pos.x - 2.f) / (float)CELL_SIZE);
	int newY = (int)((GO->pos.y - 6.f) / (float)CELL_SIZE);

	if (newX == GO->m_iCurrCellX && newY == GO->m_iCurrCellY)
		return; // Checks if object has not changed to different grid cell
				// If true, return

	Remove(GO); // Remove object from current cell
	if (newX >= 0 && newY >= 0 && newX < NUM_CELLS_X && newY < NUM_CELLS_Y)
		Add(GO); // Add object to new grid cell
}

bool Grid::CheckCollision(GameObject* GO, GameObject** GO2)
{
	int cellX = GO->m_iCurrCellX;
	int cellY = GO->m_iCurrCellY;
	bool check = false;
	GameObject* temp = m_cells[cellX][cellY]; // To check all the stuff that is next to 

	// Check current grid cell
	check = CheckCollisionLoop(temp, GO, &(*GO2));

	if (!check)
	{
		/*
		The diagram below is of a 3 by 3 grid around the object's cell, C
		X X O		This checks in the area surrounding C, 
		X C O		such that it only checks the cells where X is marked
		X O O		This ensures that the grid always check towards the left-top side cells
					therefore increasing effiency
		*/
		if (cellX > 0 && cellY < NUM_CELLS_Y - 1)
		{ // Top left
			temp = m_cells[cellX - 1][cellY + 1];
			check = CheckCollisionLoop(temp, GO, &(*GO2));
		}
		if (cellX > 0)
		{ // Left
			temp = m_cells[cellX - 1][cellY];
			check = CheckCollisionLoop(temp, GO, &(*GO2));
		}
		if (cellY < NUM_CELLS_Y - 1)
		{ // Top
			temp = m_cells[cellX][cellY + 1];
			check = CheckCollisionLoop(temp, GO, &(*GO2));
		}
		if (cellX > 0 && cellY > 0)
		{ // Bottom left
			temp = m_cells[cellX - 1][cellY - 1];
			check = CheckCollisionLoop(temp, GO, &(*GO2));
		}
	}

	return check;
}

bool Grid::CheckCollisionLoop(GameObject* temp, GameObject* GO, GameObject** GO2)
{ // Loop that checks all objects within grid cell
	bool check = false;
	while (temp != NULL)
	{ // To check all the stuff that is next of GO
		if (temp == GO)
		{
			temp = temp->next_;
			continue;
		}
		if (temp->Btype == GameObject::BLOCK_TYPE::GO_GRASS &&
			GO->Btype == GameObject::BLOCK_TYPE::GO_GRASS)
		{
			temp = temp->next_;
			continue;
		}

		if (GO->type == GameObject::GO_BALL)
		{ // If GO is a tool, do circle collision check
			check = CollisionManager::getCManager()->CheckCollisionC(GO, temp);
		}
		else if (GO->type == GameObject::GO_BLOCK)
		{ // Else if GO is a block, do box collision check
			check = CollisionManager::getCManager()->CheckCollisionB(GO, temp);
		}

		if (check)
		{ // Break loop if true
			//*GO2 = temp;
			if(GO->type == GameObject::GO_BLOCK)
				CollisionManager::getCManager()->CollisionResponseB(GO, temp);
			else if(GO->type == GameObject::GO_BALL)
				CollisionManager::getCManager()->CollisionResponseC(GO, temp);
			//break;
		}
		temp = temp->next_;
	}
	return check;
}
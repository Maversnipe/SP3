#include "Grid.h"
#include "Unit.h"

Grid::Grid()
{
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
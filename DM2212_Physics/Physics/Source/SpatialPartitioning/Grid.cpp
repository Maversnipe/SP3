#include "Grid.h"

Grid::Grid()
{
	for (int x = 0; x < NUM_CELLS; x++)
	{ // Clears the grid
		for (int y = 0; y < NUM_CELLS; y++)
		{
			//m_cells[x][y] = NULL;
		}
	}
}
#ifndef GRID_H
#define GRID_H
class Unit;

class Grid
{
public:
	Grid();
	~Grid();

	static const int NUM_CELLS_X = 12;
	static const int NUM_CELLS_Y = 7;
	
	static const int CELL_SIZE = 8;

private:
	Unit* m_cells[NUM_CELLS_X][NUM_CELLS_Y];
};

#endif
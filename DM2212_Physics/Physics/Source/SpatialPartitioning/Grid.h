#ifndef GRID_H
#define GRID_H
class GameObject;
class Block;

class Grid
{
public:
	Grid();
	~Grid();

	static const int NUM_CELLS_X = 12;
	static const int NUM_CELLS_Y = 7;
	static const int CELL_SIZE = 22;

	GameObject* GetCells(int cellX, int cellY) { return m_cells[cellX][cellY]; }

	void Add(GameObject* GO);
	void Remove(GameObject* GO);
	bool CheckCollision(GameObject* GO, GameObject** GO2);
	bool CheckCollisionLoop(GameObject* temp, GameObject* GO, GameObject** GO2);
	void Move(GameObject* GO);

private:
	GameObject* m_cells[NUM_CELLS_X][NUM_CELLS_Y];
};

#endif
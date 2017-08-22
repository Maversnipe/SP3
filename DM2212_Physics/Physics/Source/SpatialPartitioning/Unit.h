#ifndef UNIT_H
#define UNIT_H
#include "Vector3.h"
class Grid;

class Unit
{
	friend class Grid;
public:
	Unit(Grid* grid, Vector3 pos);
	~Unit();

	void Move(Vector3 pos_);

private:
	Vector3 pos;
	Grid* m_grid;

	Unit* next_;
	Unit* prev_;
};

#endif
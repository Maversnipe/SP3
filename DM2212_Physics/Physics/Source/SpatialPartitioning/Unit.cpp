#include "Unit.h"
#include "Grid.h"

Unit::Unit(Grid* grid, Vector3 pos_) 
	: m_grid(grid)
	, pos(pos_)
	, next_(NULL)
	, prev_(NULL)
{
}

Unit::~Unit()
{
}
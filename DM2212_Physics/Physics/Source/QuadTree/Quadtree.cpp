#include "Quadtree.h"
#include "../GameObject.h"

Quadtree::Quadtree() 
	: NW(NULL)
	, NE(NULL)
	, SW(NULL)
	, SE(NULL)
	, currNumOfObjects(0)
{
}

Quadtree::Quadtree(AABB boundary)
	: NW(NULL)
	, NE(NULL)
	, SW(NULL)
	, SE(NULL)
	, currNumOfObjects(0)
	, m_boundary(boundary)
{
}

Quadtree::~Quadtree()
{
	if(NW)
		delete NW;
	if(NE)
		delete NE;
	if(SW)
		delete SW;
	if(SE)
		delete SE;
}

bool Quadtree::Insert(GameObject* GO)
{
	if (!m_boundary.ContainsPoint(GO->pos))
		return false;

	if (currNumOfObjects < MAX_OBJECTS)
	{ // If the number of objects in grid cell is less than max
		m_objects.push_back(GO);
		++currNumOfObjects;
		return true;
	}

	// Subdivide if it has not been subdivided
	if (NW == NULL)
		Subdivide();

	if (NW->Insert(GO)) // Insert at top left
		return true;
	if (NE->Insert(GO)) // Insert at top right
		return true;
	if (SW->Insert(GO)) // Insert at bottom left
		return true;
	if (SE->Insert(GO)) // Insert at bottom right
		return true;

	return false;
}

void Quadtree::Subdivide()
{
	Vector3 size = Vector3((m_boundary.GetMaxAABB().x - m_boundary.GetPos().x) / 2.f,
		(m_boundary.GetMaxAABB().y - m_boundary.GetPos().y) / 2.f, 0.f);
	Vector3 center;
	AABB newAABB;
	
	// For Top left
	center = Vector3(m_boundary.GetPos().x - size.x,
		m_boundary.GetPos().y + size.y, 0.f);
	newAABB.SetAABB(center, size);
	NW = new Quadtree(newAABB);

	// For top right
	center = Vector3(m_boundary.GetPos().x + size.x,
		m_boundary.GetPos().y + size.y, 0.f);
	newAABB.SetAABB(center, size);
	NE = new Quadtree(newAABB);

	// For bottom left
	center = Vector3(m_boundary.GetPos().x - size.x,
		m_boundary.GetPos().y - size.y, 0.f);
	newAABB.SetAABB(center, size);
	SW = new Quadtree(newAABB);

	// For bottom right
	center = Vector3(m_boundary.GetPos().x + size.x,
		m_boundary.GetPos().y - size.y, 0.f);
	newAABB.SetAABB(center, size);
	SE = new Quadtree(newAABB);
}

std::vector<GameObject*> Quadtree::QueryRange(AABB range)
{
	std::vector<GameObject*> inRange;

	if (!m_boundary.IntersectsAABB(range))
		return inRange;

	for (auto &GO : m_objects)
	{
		if (range.ContainsPoint(GO->pos))
			inRange.push_back(GO);
	}

	if (NW == NULL)
		return inRange;

	std::vector<GameObject*> temp;

	// Top left
	temp = NW->QueryRange(range);
	inRange.insert(inRange.end(), temp.begin(), temp.end());

	// Top right
	temp = NE->QueryRange(range);
	inRange.insert(inRange.end(), temp.begin(), temp.end());


	// Bottom left
	temp = SW->QueryRange(range);
	inRange.insert(inRange.end(), temp.begin(), temp.end());


	// Bottom right
	temp = SE->QueryRange(range);
	inRange.insert(inRange.end(), temp.begin(), temp.end());
}

void Quadtree::Update(double dt)
{
	Iterate(this);
}

void Quadtree::Iterate(Quadtree* Qtree)
{
	
}
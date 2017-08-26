#include "minimap.h"
#include "Application.h"


CMinimap::CMinimap(void)
	: m_cMinimap_Background(NULL)
	, m_cMinimap_Border(NULL)
{
}

CMinimap::~CMinimap(void)	
{
	if (m_cMinimap_Background)
	{
		delete m_cMinimap_Background;
		m_cMinimap_Background = NULL;
	}
	if (m_cMinimap_Border)
	{
		delete m_cMinimap_Border;
		m_cMinimap_Border = NULL;
	}
}

void CMinimap::Init(float x, float y)
{
	position.Set(x, y, 0.f);
	scale.Set(30.0f, 20.0f, 1.f);
	//return true;
}

// Set the background mesh to this class
bool CMinimap::SetBackground(Mesh * aBackground)
{
	if (aBackground != NULL)
	{
		m_cMinimap_Background = aBackground;
		return true;
	}
	return false;
}
// Get the background mesh to this class
Mesh * CMinimap::GetBackground(void) const
{
	return m_cMinimap_Background;
}

// Set the border mesh to this class
bool CMinimap::SetBorder(Mesh * aBorder)
{
	if (aBorder != NULL)
	{
		m_cMinimap_Border = aBorder;
		return true;
	}
	return false;
}
// Get the border mesh to this class
Mesh * CMinimap::GetBorder(void) const
{
	return m_cMinimap_Border;
}

Vector3 CMinimap::getPosition()
{
	return position;
}

Vector3 CMinimap::getScale()
{
	return scale;
}


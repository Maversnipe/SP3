#include "background.h"

CBackground::CBackground(void)
	:m_CBackground(NULL)
{

}

CBackground::~CBackground(void)
{
	if (m_CBackground)
	{
		delete m_CBackground;
		m_CBackground = NULL;
	}
}

void CBackground::Init(/*float x, float y*/)
{
	position.Set(130, 95, -1.f);
	scale.Set(256.0f, 196.0f, 1.f);
}

// Set the background mesh to this class
bool CBackground::SetBackground(Mesh * aBackground)
{
	if (aBackground != NULL)
	{
		m_CBackground = aBackground;
		return true;
	}
	return false;
}
// Get the background mesh to this class
Mesh * CBackground::GetBackground(void) const
{
	return m_CBackground;
}

Vector3 CBackground::getPosition()
{
	return position;
}

Vector3 CBackground::getScale()
{
	return scale;
}
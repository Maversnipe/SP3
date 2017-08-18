#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
	this->defaultPos = pos;
	this->minBoundary.Set(-50.f, -50.f, -50.f); // Setting of Camera minimum X and Y
	this->maxBoundary.Set(50.f, 50.f, 50.f); // Setting of Camera maximum X and Y
	m_eMoveType = CLICK_N_DRAG;
}

void Camera::Reset()
{
	position.Set(defaultPos.x, defaultPos.y, defaultPos.z);
	target.Set(defaultTarget.x, defaultTarget.y , defaultTarget.z);
	up.Set(defaultUp.x, defaultUp.y, defaultUp.z);
	m_fOffset_x = 0.f;
	m_fOffset_y = 0.f;
}

void Camera::Update(double dt)
{
	if(m_eMoveType == KEYBOARD)
		K_CameraMovement(dt);
	else
		M_CameraMovement(dt);

	position.Set(defaultPos.x + m_fOffset_x, defaultPos.y + m_fOffset_y, defaultPos.z);
	target.Set(defaultTarget.x + m_fOffset_x, defaultTarget.y + m_fOffset_y, defaultTarget.z);
}

void Camera::K_CameraMovement(double dt)
{
	if (Application::IsKeyPressed(VK_UP))
	{
		if (position.y + m_fOffset_y < maxBoundary.y)
		{
			m_fOffset_y += 50.f * (float)dt;
			if (position.y + m_fOffset_y > maxBoundary.y)
			{
				m_fOffset_y = maxBoundary.y - position.y;
			}
		}
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		if (position.y + m_fOffset_y > minBoundary.y)
		{
			m_fOffset_y -= 50.f * (float)dt;
			if (position.y + m_fOffset_y < minBoundary.y)
			{
				m_fOffset_y = position.y - minBoundary.y;
			}
		}
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		if (position.x + m_fOffset_x > minBoundary.x)
		{
			m_fOffset_x -= 50.f * (float)dt;
			if (position.x + m_fOffset_x < minBoundary.x)
			{
				m_fOffset_x = position.x - minBoundary.x;
			}
		}
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		if (position.x + m_fOffset_x < maxBoundary.x)
		{
			m_fOffset_x += 50.f * (float)dt;
			if (position.x + m_fOffset_x > maxBoundary.x)
			{
				m_fOffset_x = maxBoundary.x - position.x;
			}
		}
	}
}

void Camera::M_CameraMovement(double dt)
{
	float m_worldHeight = 100.f;
	float m_worldWidth = m_worldHeight * (float)(Application::GetWindowWidth() / Application::GetWindowHeight());
	double x, y;
	Application::GetCursorPos(&x, &y);
	int w = Application::GetWindowWidth();
	int h = Application::GetWindowHeight();
	float posX = static_cast<float>(x) / w * m_worldWidth;
	float posY = (h - static_cast<float>(y)) / h * m_worldHeight;

	if (m_eMoveType == CLICK_N_DRAG)
	{
		static bool bLButtonState = false;
		if (Application::IsMousePressed(0))
		{
			if (!bLButtonState)
			{ // Check if it is first time mouse button pressed
				bLButtonState = true;
				firstPos.Set(posX + m_fOffset_x, posY + m_fOffset_y, defaultPos.z);
			}
			else
			{
				m_fOffset_x = -(posX - firstPos.x); // X axis movement
				if (defaultPos.x + m_fOffset_x > maxBoundary.x)
				{ // If offset to left is more than maxBoundary, set as maxBoundary
					m_fOffset_x = (maxBoundary.x - defaultPos.x);
				}
				else if (defaultPos.x + m_fOffset_x < minBoundary.x)
				{ // If offset to right is less than minBoundary, set as minBoundary
					m_fOffset_x = -(defaultPos.x - minBoundary.x);
				}

				m_fOffset_y = -(posY - firstPos.y); // Y axis movement
				if (defaultPos.y + m_fOffset_y > maxBoundary.y)
				{ // If offset to up is more than maxBoundary, set as maxBoundary
					m_fOffset_y = (maxBoundary.y - defaultPos.y);
				}
				else if (defaultPos.y + m_fOffset_y < minBoundary.y)
				{ // If offset to down is less than minBoundary, set as minBoundary
					m_fOffset_y = -(defaultPos.y - minBoundary.y);
				}
			}
		}
		else if (bLButtonState && !Application::IsMousePressed(0))
		{ // If mouse button is not pressed
			bLButtonState = false;
		}
	}
	else if (m_eMoveType == MOVE_WITH_MOUSE)
	{ // Window move with mouse
		if (posX > m_worldWidth * 0.75f)
		{ // Move to the left
			float speed = (posX - m_worldWidth * 0.75f) / (m_worldWidth * 0.25f);
			if (defaultPos.x + m_fOffset_x < maxBoundary.x)
			{
				m_fOffset_x += 50.f * (float)dt * speed;
				if (defaultPos.x + m_fOffset_x > maxBoundary.x)
				{ // If offset to left is more than maxBoundary, set as maxBoundary
					m_fOffset_x = maxBoundary.x - defaultPos.x;
				}
			}
		}
		else if (posX < m_worldWidth * 0.25f)
		{ // Move to the right
			float speed = ((m_worldWidth * 0.25f) - posX) / (m_worldWidth * 0.25f);
			if (defaultPos.x + m_fOffset_x > minBoundary.x)
			{
				m_fOffset_x -= 50.f * (float)dt * speed;
				if (defaultPos.x + m_fOffset_x < minBoundary.x)
				{ // If offset to right is less than minBoundary, set as minBoundary
					m_fOffset_x = -(defaultPos.x - minBoundary.x);
				}
			}
		}

		if (posY > m_worldHeight * 0.75f)
		{ // Move up
			float speed = (posY - m_worldHeight * 0.75f) / (m_worldHeight * 0.25f);
			if (defaultPos.y + m_fOffset_y < maxBoundary.y)
			{
				m_fOffset_y += 50.f * (float)dt * speed;
				if (defaultPos.y + m_fOffset_y > maxBoundary.y)
				{ // If offset to up is more than maxBoundary, set as maxBoundary
					m_fOffset_y = maxBoundary.y - defaultPos.y;
				}
			}
		}
		else if (posY < m_worldHeight * 0.25f)
		{ // Move down
			float speed = ((m_worldHeight * 0.25f) - posY) / (m_worldHeight * 0.25f);
			if (defaultPos.y + m_fOffset_y > minBoundary.y)
			{
				m_fOffset_y -= 50.f * (float)dt * speed;
				if (defaultPos.y + m_fOffset_y < minBoundary.y)
				{ // If offset to down is less than minBoundary, set as minBoundary
					m_fOffset_y = -(defaultPos.y - minBoundary.y);
				}
			}
		}
	}
}
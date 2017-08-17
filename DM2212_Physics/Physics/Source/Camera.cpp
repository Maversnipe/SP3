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
	this->minBoundary.Set(-100.f, -100.f, -100.f); // Setting of Camera minimum X and Y
	this->maxBoundary.Set(100.f, 100.f, 100.f); // Setting of Camera maximum X and Y
	m_eMoveType = MOVE_WITH_MOUSE;
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
		if (Application::IsMousePressed(0))
		{

		}
	}
	else if (m_eMoveType == MOVE_WITH_MOUSE)
	{
		if (posX > m_worldWidth * 0.75f)
		{
			float speed = (posX - m_worldWidth * 0.75f) / (m_worldWidth * 0.25f);
			if (position.x + m_fOffset_x < maxBoundary.x)
			{
				m_fOffset_x += 50.f * (float)dt * speed;
				if (position.x + m_fOffset_x > maxBoundary.x)
				{
					m_fOffset_x = maxBoundary.x - position.x;
				}
			}
		}
		else if (posX < m_worldWidth * 0.25f)
		{
			float speed = ((m_worldWidth * 0.25f) - posX) / (m_worldWidth * 0.25f);
			if (position.x + m_fOffset_x > minBoundary.x)
			{
				m_fOffset_x -= 50.f * (float)dt * speed;
				if (position.x + m_fOffset_x < minBoundary.x)
				{
					m_fOffset_x = position.x - minBoundary.x;
				}
			}
		}

		if (posY > m_worldHeight * 0.75f)
		{
			float speed = (posY - m_worldHeight * 0.75f) / (m_worldHeight * 0.25f);
			if (position.y + m_fOffset_y < maxBoundary.y)
			{
				m_fOffset_y += 50.f * (float)dt * speed;
				if (position.y + m_fOffset_y > maxBoundary.y)
				{
					m_fOffset_y = maxBoundary.y - position.y;
				}
			}
		}
		else if (posY < m_worldHeight * 0.25f)
		{
			float speed = ((m_worldHeight * 0.25f) - posY) / (m_worldHeight * 0.25f);
			if (position.y + m_fOffset_y > minBoundary.y)
			{
				m_fOffset_y -= 50.f * (float)dt * speed;
				if (position.y + m_fOffset_y < minBoundary.y)
				{
					m_fOffset_y = position.y - minBoundary.y;
				}
			}
		}
	}
}
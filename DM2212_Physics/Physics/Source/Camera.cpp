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
	this->minBoundary.Set(-10.f, -10.f, -10.f);
	this->maxBoundary.Set(10.f, 10.f, 10.f);
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(double dt)
{
	CameraMovement(dt);
}

void Camera::CameraMovement(double dt)
{
	if (Application::IsKeyPressed(VK_UP))
	{
		if (position.y < maxBoundary.y)
		{
			position.y += 50.f * (float)dt;
			target.y += 50.f * (float)dt;
			if (position.y > maxBoundary.y)
			{
				position.y = maxBoundary.y;
				target.y = maxBoundary.y;
			}
		}
	}
	if (Application::IsKeyPressed(VK_DOWN))
	{
		if (position.y > minBoundary.y)
		{
			position.y -= 50.f * (float)dt;
			target.y -= 50.f * (float)dt;
			if (position.y < minBoundary.y)
			{
				position.y = minBoundary.y;
				target.y = minBoundary.y;
			}
		}
	}
	if (Application::IsKeyPressed(VK_LEFT))
	{
		if (position.x > minBoundary.x)
		{
			position.x -= 50.f * (float)dt;
			target.x -= 50.f * (float)dt;
			if (position.x < minBoundary.x)
			{
				position.x = minBoundary.x;
				target.x = minBoundary.x;
			}
		}
	}
	if (Application::IsKeyPressed(VK_RIGHT))
	{
		if (position.x < maxBoundary.x)
		{
			position.x += 50.f * (float)dt;
			target.x += 50.f * (float)dt;
			if (position.x > maxBoundary.x)
			{
				position.x = maxBoundary.x;
				target.x = maxBoundary.x;
			}
		}
	}
}
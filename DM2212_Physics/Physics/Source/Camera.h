#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Reset();
	virtual void Update(double dt);

	void K_CameraMovement(double dt); // Camera Movements for keyboard
	void M_CameraMovement(double dt); // Camera Movements for mouse

	float GetOffset_x() { return m_fOffset_x; }
	float GetOffset_y() { return m_fOffset_y; }

	enum MoveType
	{
		CLICK_N_DRAG = 0,
		MOVE_WITH_MOUSE,
		KEYBOARD,
	};
	MoveType m_eMoveType;

private:
	Vector3 minBoundary; // Minimum X and Y that the camera can go to
	Vector3 maxBoundary; // Maximum X and Y that the camera can go to
	Vector3 defaultPos; // Original position of camera
	Vector3 defaultTarget; // Original target of camera
	Vector3 defaultUp; // Original up of camera
	Vector3 firstPos; // Position of mouse when it is first clicked

	float m_fOffset_y; // How much the camera position is offset by in Y axis
 	float m_fOffset_x; // How much the camera position is offset by in X axis
};

#endif
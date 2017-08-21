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
		MOVE_TYPE_TOTAL,
	};
	MoveType m_eMoveType; // Which type of movement to use
	void SetMoveType(MoveType _input) { m_eMoveType = _input; }

private:
	// Default values
	Vector3 defaultPos; // Original position of camera
	Vector3 defaultTarget; // Original target of camera
	Vector3 defaultUp; // Original up of camera
	
	// Mouse Movement
	Vector3 minBoundary; // Minimum X and Y that the camera can go to
	Vector3 maxBoundary; // Maximum X and Y that the camera can go to
	float m_fOffset_y; // How much the camera position is offset by in Y axis
	float m_fOffset_x; // How much the camera position is offset by in X axis

	// For MOVE_WITH_MOUSE MoveType
	Vector3 firstPos; // Position of mouse when it is first clicked

};

#endif
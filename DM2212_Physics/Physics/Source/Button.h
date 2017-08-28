#pragma once
#include "Vector3.h"

class Button
{
public:
	enum BUTTON_TYPE
	{
		BUTTON_NONE,
		BUTTON_PLAY,
		BUTTON_SANDBOX,
		BUTTON_OPTIONS,
		BUTTON_EXIT,
		BUTTON_MAX,
	};
	Button();
	Button(Vector3 scale, Vector3 pos, BUTTON_TYPE buttype);
	~Button();
	
	Vector3 GetScale();
	Vector3 GetPos();
	void SetScale(Vector3 scale);
	bool MouseCheck(Vector3 mousepos);

	void SetBig(bool big);
	bool GetBig();

	BUTTON_TYPE buttype;

private:
	Vector3 scale;
	Vector3 pos;
	bool big;
	bool active;
};
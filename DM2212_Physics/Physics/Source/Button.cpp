#include "Button.h"

Button::Button()
{
}

Button::Button(Vector3 scale, Vector3 pos,BUTTON_TYPE buttype)
	:scale(scale), pos(pos), buttype(buttype)
{
}

Button::~Button()
{
}

Vector3 Button::GetScale()
{
	return scale;
}

Vector3 Button::GetPos()
{
	return pos;
}

void Button::SetScale(Vector3 scale)
{
	this->scale = scale;
}

bool Button::MouseCheck( Vector3 mousepos)
{
	return (mousepos.x < pos.x + scale.x*0.5
		&& mousepos.x > pos.x - scale.x*0.5
		&& mousepos.y < pos.y + scale.y*0.5
		&&mousepos.y > pos.y - scale.y*0.5);
}

void Button::SetBig(bool big)
{
	this->big = big;
}

bool Button::GetBig()
{
	return big;
}

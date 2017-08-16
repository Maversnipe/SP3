#pragma once
#include "GameObject.h"
#include "Vector3.h"

class ToolsInfo : public GameObject
{
public:
	ToolsInfo();
	virtual ~ToolsInfo();

	virtual void Init();//set default data
	virtual void Update(double dt);//upadate data

	virtual void SetPrice(const int in_price);
	virtual int GetPrice() const;

	virtual void SetPos(const Vector3 in_pos);
	virtual Vector3 GetPos() const;

	virtual void UseTool();//stuff that it dos

protected:
	int i_Price;
	Vector3 v3_Position;

};
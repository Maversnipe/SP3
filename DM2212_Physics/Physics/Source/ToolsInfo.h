#pragma once
#include "GameObject.h"
#include "Vector3.h"
#include "SceneBase.h"
#include "Blocks.h"
#include <vector>
using std::vector;
class Grid;
class ToolsInfo : public GameObject
{
public:
	ToolsInfo(Grid* grid);
	virtual ~ToolsInfo();

	virtual void Init();//set default data
	virtual void Update(double dt,Vector3 mousepos);//upadate data

	virtual void SetPrice(const int in_price);
	virtual int GetPrice() const;

	virtual void SetPos(const Vector3 in_pos);
	virtual Vector3 GetPos() const;

	virtual bool UseTool(vector<Block*> blockList, vector<GameObject*> &goList);//stuff that it dos
	virtual GameObject* FetchGO(vector<GameObject*>& goList);

protected:
	int i_Price;

};
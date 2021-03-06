#pragma once
#include "Blocks.h"

class MapEditor
{
protected:
	static MapEditor *instance;
	MapEditor();

public:
	//singleton sutff
	static MapEditor *GetInstance()
	{
		if (!instance)
			instance = new MapEditor;
		return instance;
	}
	static bool DropInstance()
	{
		if (instance)
		{
			delete instance;
			instance = NULL;
			return true;
		}
		return false;
	}

	~MapEditor();

	void Init(Grid* grid);
	Block* FetchBlocks(std::vector<Block*>& blocklist, Grid* grid);

	void SaveMap(std::vector<Block*>& blocklist);
	int DeleteMap(std::vector<Block*>& blocklist);

	bool PlaceBlock(std::vector<Block*>& blocklist,Grid* &m_grid);
	bool RemoveBlock(std::vector<Block*>& blocklist, Grid* &m_grid);

	void SwitchBlock(int i);

	bool GetIsEditing();
	void SetIsEditing(bool editing);

	void Update(double dt, Vector3 mousepos);

	GameObject* GetCurrentBlock();

private:
	const int totalnumblocks = 5;
	int currblockint;
	bool b_isEditing;
	Vector3 brickPos;
	Block** blockmanager;

};
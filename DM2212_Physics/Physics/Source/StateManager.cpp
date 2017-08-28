#include "StateManager.h"

StateManager* StateManager::statemanager;

StateManager::~StateManager()
{
}

StateManager * StateManager::getInstance()
{
	if (!statemanager)
	{
		statemanager = new StateManager;
	}
	return statemanager;
}

void StateManager::Init()
{

}

void StateManager::Update()
{

}

void StateManager::ChangeState(int key)
{
	
}

StateManager::StateManager()
{

}

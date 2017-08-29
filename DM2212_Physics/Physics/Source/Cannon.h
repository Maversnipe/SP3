#ifndef CANNON_H
#define CANNON_H

#include "GameObject.h"
#include "Vector3.h"

class Cannon : public GameObject
{
public:
	Cannon();
	~Cannon();

	void Init();
	void Update(double dt, Vector3 mousepos);

	int Getbullet();
	void Decrease(int amount);

private:
	int m_ibullet;		//Bullets
};

#endif // !CANNON_H

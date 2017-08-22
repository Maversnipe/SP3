#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H

#include "GameObject.h"
#include "Blocks.h"

class Explosive : public GameObject
{
public:
	Explosive();
	~Explosive();

	void Setexplosiontime(double dt); 
	bool getexploding() { return exploding; };

	void Update(std::vector <GameObject*> objs, std::vector <Block*> blks, double dt);

	bool checkCollision(std::vector<GameObject *> &Objs, std::vector<Block *> &Blks);
	void Response();

protected:
	GameObject* affected;
	float m_fexplosiontime;
	float m_felapsedtime;
	bool exploding;

};

#endif //!EXPLOSIVE_H
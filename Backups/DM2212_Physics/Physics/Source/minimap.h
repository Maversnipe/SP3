#pragma once

#include "MatrixStack.h"
#include "SingletonTemplate.h"
#include "Mesh.h"

class CMinimap : public Singleton<CMinimap>
{
public:
	CMinimap(void);
	virtual ~CMinimap(void);

	void Init(float x, float y);//Initialise the class instance

	Mesh* m_cMinimap_Background;
	Mesh* m_cMinimap_Border;

	bool SetBackground(Mesh* aBackground); //Set BG mesh to this class
	Mesh* GetBackground(void) const; //Get BG mesh to this class

	bool SetBorder(Mesh* aBorder); //Set border mesh to this class
	Mesh* GetBorder(void) const; //Get border mesh to this class

	Vector3 getPosition();
	Vector3 getScale();

protected:
	//set position and scale
	Vector3 position;
	Vector3 scale;

};

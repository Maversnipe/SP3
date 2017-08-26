#pragma once

#include "MatrixStack.h"
#include "SingletonTemplate.h"
#include "Mesh.h"

class CBackground : public Singleton<CBackground>
{
public:
	CBackground(void);
	virtual ~CBackground(void);

	void Init(/*float x, float y*/);//Initialise the class instance

	Mesh* m_CBackground;

	bool SetBackground(Mesh* aBackground); //Set BG mesh to this class
	Mesh* GetBackground(void) const; //Get BG mesh to this class

	Vector3 getPosition();
	Vector3 getScale();

protected:
	//set position and scale
	Vector3 position;
	Vector3 scale;

};



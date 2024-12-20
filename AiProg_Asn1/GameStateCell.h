#pragma once
#define MAX_PARTICLES 100

#include <GameState.h>
#include <Camera.h>
#include "ParticleVar.h"


class GameStateCell : public GameState
{
public:

	virtual bool Init() override;
	virtual bool Update(double dt) override;
	virtual void Render() override;
	virtual void Destroy() override;

protected:
	void Rule(GameObject* obj1[], GameObject* obj2[], float g, float range = 120, int obj1Size = MAX_PARTICLES, int obj2Size = MAX_PARTICLES);
	void posWrap(GameObject* obj); 
	float findSqrt(float n);
	inline void updateParticle();
	void randParticles();

	Camera camera;

	ParticleVar greenDist; //range that green objects get affected by
	ParticleVar redDist; //range that red objects get affected by
	ParticleVar blueDist; //range that blue objects get affected by
	ParticleVar yellowDist; //range that yellow objects get affected by

	ParticleVar greenMag; //magnitude of the other color on green objects
	ParticleVar redMag; //magnitude of the other color on red objects
	ParticleVar blueMag; //magnitude of the other color on blue objects
	ParticleVar yellowMag; //magnitude of the other color on yellow objects

	float m_worldWidth;
	float m_worldHeight;
	GameObject* yellow_Obj[MAX_PARTICLES]; //Array of yellow particles
	GameObject* red_Obj[MAX_PARTICLES]; //Array of red particles
	GameObject* green_Obj[MAX_PARTICLES]; //Array of green particles
	GameObject* blue_Obj[MAX_PARTICLES]; //Array of blue particles
	GameObject* mouse[1]; //Mouse gameobject, DO NOT CHANGE TO A REGULAR POINTER

	float cursorRadius = 20.0f; //Size of the cursor
	float cursorMagnitude = 20.f; //Magnitude of attraction/repulsion the cursor emits when in use 

};


//#ifndef SCENE_CELL_H
//#define SCENE_CELL_H
//
//#define MAX_PARTICLES 200
//
//#include "GameObject.h"
//#include "Vector3.h"
//#include "Scene.h"
//#include "Mesh.h"
//#include "MatrixStack.h"
//#include "Camera.h"
//#include "Vertex.h"
//
//#include "ParticleVar.h"
//
//#include "MouseController.h"
//
//class SceneCell : public Scene
//{
//public:
//	enum GEOMETRY_TYPE
//	{
//		GEO_AXES,
//		GEO_TEXT,
//		GEO_YELLOW,
//		GEO_RED,
//		GEO_GREEN,
//		GEO_BLUE,
//		GEO_CURSOR,
//		NUM_GEOMETRY,
//	};
//
//	enum UNIFORM_TYPE
//	{
//		U_MVP = 0,
//		U_MODELVIEW,
//		U_MODELVIEW_INVERSE_TRANSPOSE,
//		U_MATERIAL_AMBIENT,
//		U_MATERIAL_DIFFUSE,
//		U_MATERIAL_SPECULAR,
//		U_MATERIAL_SHININESS,
//		U_LIGHTENABLED,
//		U_NUMLIGHTS,
//		U_LIGHT0_TYPE,
//		U_LIGHT0_POSITION,
//		U_LIGHT0_COLOR,
//		U_LIGHT0_POWER,
//		U_LIGHT0_KC,
//		U_LIGHT0_KL,
//		U_LIGHT0_KQ,
//		U_LIGHT0_SPOTDIRECTION,
//		U_LIGHT0_COSCUTOFF,
//		U_LIGHT0_COSINNER,
//		U_LIGHT0_EXPONENT,
//		U_COLOR_TEXTURE_ENABLED,
//		U_COLOR_TEXTURE,
//		U_TEXT_ENABLED,
//		U_TEXT_COLOR,
//		U_TOTAL,
//	};
//
//	SceneCell();
//	~SceneCell();
//
//	virtual bool Init();
//	virtual bool Update(double dt);
//	virtual void Render();
//	virtual void Exit();
//
//private:
//	void HandleKeyPress();
//	void RenderMesh(Mesh* mesh, bool enableLight);
//	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
//
//
//	//********************************************************************************************
//	/*!
//	\brief
//	Determines the behaviour of obj1 when in range of obj2
//
//	\param obj1[] - the particle type you want to get affected
//	\param obj2[] - the particle type that affects obj1
//	\param g - the force of attraction/repulsion, negative for attraction, positive for repulsion
//	\param range - the range at which obj1 gets affected by obj2, DEFAULT = 120
//	\param obj1Size - The array size of obj1, DEFAULT = MAX_PARTICLES
//	\param obj2Size - The array size of obj2, DEFAULT = MAX_PARTICLES
//
//	\return void
//	*/
//	void Rule(GameObject* obj1[], GameObject* obj2[], float g, float range = 120, int obj1Size = MAX_PARTICLES, int obj2Size = MAX_PARTICLES);
//	//********************************************************************************************
//
//
//	//********************************************************************************************
//	/*!
//	\brief
//	Allows for the gameObject to move through 1 side of the screen and pop out of the other
//
//	\param obj - The game object that you want to wrap the position with
//
//	\return void
//	*/
//	void posWrap(GameObject* obj);
//	//********************************************************************************************
//
//
//	//********************************************************************************************
//	/*!
//	\brief
//	Quick Inverse sqrt using some weird bit manipulation
//
//	\param n - The value you want to get 1/sqrt
//
//	\exception None
//	\return 1/sqrt the value you input
//	*/
//	float findSqrt(float n);
//	//********************************************************************************************
//
//
//	//********************************************************************************************
//	/*!
//	\brief
//	Updates the particle behaviour/movement
//
//	\return void
//	*/
//	inline void updateParticle();
//	//********************************************************************************************
//
//
//	//********************************************************************************************
//	/*!
//	\brief
//	Randomises the particle behaviour, call it in Init
//
//	\return void
//	*/
//	void randParticles();
//	//********************************************************************************************
//
//	float cursorRadius = 20.0f; //Size of the cursor
//	float cursorMagnitude = 20.f; //Magnitude of attraction/repulsion the cursor emits when in use
//
//	unsigned m_vertexArrayID;
//	Mesh* meshList[NUM_GEOMETRY];
//
//	unsigned m_programID;
//	unsigned m_parameters[U_TOTAL];
//
//	float m_worldWidth;
//	float m_worldHeight;
//	GameObject* yellow_Obj[MAX_PARTICLES]; //Array of yellow particles
//	GameObject* red_Obj[MAX_PARTICLES]; //Array of red particles
//	GameObject* green_Obj[MAX_PARTICLES]; //Array of green particles
//	GameObject* blue_Obj[MAX_PARTICLES]; //Array of blue particles
//	GameObject* mouse[1]; //Mouse gameobject, DO NOT CHANGE TO A REGULAR POINTER
//
//	float particleSize = 1; //particle size, change this how you wish
//	bool saveParticles = false; //bool for single key press for saving particle values
//	double mouseX, mouseY; //mouse coords
//
//	Camera camera;
//
//	//Use the corresponding color to whatever color is in the obj1 parameter in Rule();
//	//Use the corresponding parameter in your ParticleVar variable to whatever color is in the obj2 parameter in Rule();
//	//eg: Rule(green_Obj, red_Obj, greenMag.red, greenDist.red)
//
//	ParticleVar greenDist; //range that green objects get affected by
//	ParticleVar redDist; //range that red objects get affected by
//	ParticleVar blueDist; //range that blue objects get affected by
//	ParticleVar yellowDist; //range that yellow objects get affected by
//
//	ParticleVar greenMag; //magnitude of the other color on green objects
//	ParticleVar redMag; //magnitude of the other color on red objects
//	ParticleVar blueMag; //magnitude of the other color on blue objects
//	ParticleVar yellowMag; //magnitude of the other color on yellow objects
//
//
//
//	MS modelStack;
//	MS viewStack;
//	MS projectionStack;
//};
//
//#endif
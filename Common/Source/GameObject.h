#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <glm\glm.hpp>
#include "RenderParameters.h"
#include <vector>
#include "Mesh.h"
#include <map>

class GameObject
{
protected:

	//struct for determining the start and end of our sprite animations
	struct spriteAnimation {
	private:
		unsigned int animStartX;
		unsigned int animStartY;
		unsigned int animEndX;
		unsigned int animEndY;
		
	public:
		//constructor for our sprite animator
		spriteAnimation(unsigned int animStartX, unsigned int animStartY, unsigned int animEndX, unsigned int animEndY)
		{
			this->animStartX = animStartX;
			this->animEndX = animEndX;
			this->animStartY = animStartY;
			this->animEndY = animEndY;
		}
		spriteAnimation() 
		{
			this->animStartX = 0;
			this->animStartY = 0;
			this->animEndX = 0;
			this->animEndY = 0;
		};

		unsigned int getAnimStartX(void) { return animStartX; }
		unsigned int getAnimStartY(void) { return animStartY; }
		unsigned int getAnimEndX(void) { return animEndX; }
		unsigned int getAnimEndY(void) { return animEndY; }
	};

	//each gameObject must have these 3 parameters
	glm::vec2 position;
	glm::vec2 rotation;
	glm::vec2 scale;

	glm::vec2 self_position;
	glm::vec2 self_rotation;
	glm::vec2 self_scale;

	float opacityValue = 1.f;

	//for tilemap entities, these 3 are all we basically need, for game entities, we need physics-related code that we'll handle in Entity2D

	RenderParameters* renderParam = RenderParameters::GetInstance();
	
	std::vector<GameObject*> child;

	Mesh* mesh;

	//the value for our animated sprite sheet
	glm::vec2 animatedSpriteSize;

	//the name of our current animation string being played
	spriteAnimation* currentAnimation;

	//links our sprite animations to a name
	std::map<const char*, spriteAnimation*> spriteAnimationMap;

	float animationFrameTimer;
	bool isPlayingAnimation;

	unsigned int animationFrameX, animationFrameY;

	bool toRender = true;
	bool updateInBackend;

public:
	int currentLevel;
	unsigned int textureID;

	GameObject(glm::vec2 pos = glm::vec2(0, 0), glm::vec2 rot = glm::vec2(0, 0), glm::vec2 scl = glm::vec2(1, 1));
	~GameObject();

	void setPosition(glm::vec2 newPos);
	glm::vec2 getPosition();	
	
	void setRotation(glm::vec2 newRot);
	glm::vec2 getRotation();	
	
	void setScale(glm::vec2 newScl);
	glm::vec2 getScale();


	void SetMesh(Mesh* mesh);
	Mesh*& getMesh(void);

	void setOpacity(float opacityValue);
	float getOpacity(void);


	virtual void Init(std::vector<GameObject*>& objList, int currentLevel);

	virtual void Update(double dt) {};

	virtual void Render();

	virtual void RenderMesh();

	virtual void SetTexture(const char* texture);

	/*!
	@brief Determines the size of each sprite texture for the animated sprite renderer to reference 

	\param numRows - the number of rows within the sprite sheet
	\param numCol - the number of columns within the sprite sheet
	*/
	void sliceTexture(const unsigned int numRows, const unsigned int numCol);
	
	void createNewAnimation(const char* animationString, unsigned int animationStart, unsigned int animationEnd);

	void setCurrentAnimation(const char* animationString);

	void playAnimation(float frameDelay, float dt);
	//void playAnimation(float frameDelay, float dt, unsigned int iterationCount = 0);
	//void playAnimation(float frameDelay, float dt, float animationDuration);

	void pauseAnimation();
	void toggleAnimation();


};
#endif


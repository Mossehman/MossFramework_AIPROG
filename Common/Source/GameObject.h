#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <glm\glm.hpp>
#include "RenderParameters.h"
#include "IMessageReciever.h"
#include <vector>
#include "Mesh.h"
#include <map>

struct AnimatedSpriteParam {
	unsigned int StartIndex;
	unsigned int EndIndex;

	float frameDelay;
	unsigned int iterations;

	AnimatedSpriteParam(unsigned int StartIndex, unsigned int EndIndex, float frameDelay = 0.1f, unsigned int iterations = 0)
	{
		this->StartIndex = StartIndex;
		this->EndIndex = EndIndex;
		this->frameDelay = frameDelay;
		this->iterations = iterations;
	}
};

class GameObject : public IMessageReciever
{
protected:
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

	bool toRender = true;
	bool updateInBackend;


	unsigned int SpriteColumns = 1;
	unsigned int SpriteRows = 1;
	glm::vec2 SpriteTextureSize; 
	std::string CurrentAnimationID;

	int CurrentFrameIndex = 0; 
	float SpriteFrameTimer = 0.f; 
	AnimatedSpriteParam* CurrentAnimationParam = nullptr; 
	bool isPlayingAnimation = false; 
	
	std::map<std::string, AnimatedSpriteParam*> Animations; 
	int GetFrameIndex(int Column, int Row);
	glm::ivec2 GetColRow(int FrameIndex);

	glm::vec2 cachedScale;


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
	void UpdateAnimations(double dt);

	virtual void Render();

	virtual void RenderMesh();

	virtual void SetTexture(const char* texture, unsigned int NumColumns = 1, unsigned int NumRows = 1);

	void AddAnimation(std::string animName, AnimatedSpriteParam* newAnimation);

	void SetCurrentAnimation(std::string animName);

	int GetCurrentAnimationFrame(void);

	AnimatedSpriteParam* GetCurrentAnimationParam(void);




	// Inherited via IMessageReciever
	virtual int HandleMessage(BaseMessage* msg) override;

};
#endif


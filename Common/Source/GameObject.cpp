#include "GameObject.h"
#include "GL/glew.h"
#include "ImageLoader.h"
#include "MyMath.h"
#include "Map2D.h"


GameObject::GameObject(glm::vec2 pos, glm::vec2 rot, glm::vec2 scl)
{
	position = pos;
	rotation = rot;
	scale = scl;
	updateInBackend = false;

	self_position = glm::vec2(0, 0);
	self_rotation = glm::vec2(0, 0);
	self_scale = glm::vec2(1, 1);

	textureID = 0; //set the default texture ID
	//this->mesh = MeshBuilder::GenerateQuad("quad mesh", Color(1.f, 1.f, 1.f)); //set the default mesh on initalisation to be a quad mesh
	currentAnimation = nullptr;
	animationFrameTimer = 0.f;
	animatedSpriteSize = glm::vec2(0, 0);
	isPlayingAnimation = false;

}

GameObject::~GameObject()
{
	if (this->mesh)
	{
		delete this->mesh;
	}

	if (this->currentAnimation != nullptr)
	{
		delete currentAnimation;
	}
}

void GameObject::setPosition(glm::vec2 newPos)
{
	position = newPos;
}

glm::vec2 GameObject::getPosition()
{
	return position;
}

void GameObject::setRotation(glm::vec2 newRot)
{
	rotation = newRot;
}

glm::vec2 GameObject::getRotation()
{
	return rotation;
}

void GameObject::setScale(glm::vec2 newScl)
{
	scale = newScl;
}

glm::vec2 GameObject::getScale()
{
	return scale;
}

void GameObject::setMesh(Mesh* newMesh)
{
	mesh = newMesh;
	//if (mesh) { mesh->textureID = 0; }
}

Mesh*& GameObject::getMesh(void)
{
	return mesh;
}

void GameObject::setOpacity(float opacityValue)
{
	this->opacityValue = opacityValue;
}

float GameObject::getOpacity(void)
{
	return this->opacityValue;
}

void GameObject::Init(int currentLevel)
{
	this->currentLevel = currentLevel;
}

void GameObject::Render()
{
	if (!toRender /*|| currentLevel != Map2D::GetInstance()->GetCurrentLevel()*/) { return; }

	RenderParameters::GetInstance()->modelStack.PushMatrix();
	RenderParameters::GetInstance()->modelStack.Translate(position.x, position.y, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(rotation.x, 1, 0, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(rotation.y, 0, 1, 0);
	RenderParameters::GetInstance()->modelStack.Scale(scale.x, scale.y, 1);

	if (!child.empty())
	{
		for (int i = 0; i < child.size(); i++)
		{
			child[i]->Render();
		}
	}

	//std::cout << position.x << "," << position.y << std::endl;

	RenderParameters::GetInstance()->modelStack.PushMatrix();
	RenderParameters::GetInstance()->modelStack.Translate(self_position.x, self_position.y, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(self_rotation.x, 1, 0, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(self_rotation.y, 0, 1, 0);
	RenderParameters::GetInstance()->modelStack.Scale(self_scale.x, self_scale.y, 1);
	

	renderMesh();

	RenderParameters::GetInstance()->modelStack.PopMatrix();
	RenderParameters::GetInstance()->modelStack.PopMatrix();
}

void GameObject::renderMesh()
{
	Mtx44 MVP;
	MVP = RenderParameters::GetInstance()->projectionStack.Top() * RenderParameters::GetInstance()->viewStack.Top() * RenderParameters::GetInstance()->modelStack.Top();
	glUniformMatrix4fv(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_MVP], 1, GL_FALSE, &MVP.a[0]);
	

	if (textureID > 0)
	{
		glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(opacityValue);


	if (textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
}

void GameObject::setTexture(const char* texture)
{
	textureID = ImageLoader::GetInstance()->LoadTextureGetID(texture, true);
	mesh->textureID = textureID;
	std::cout << textureID << std::endl;
}

void GameObject::sliceTexture(const unsigned int numRows, const unsigned int numCol)
{
	// Check if numRows and numCol are greater than zero to prevent division by zero
	if (numRows == 0 || numCol == 0) {
		std::cerr << "Error: Number of rows and columns must be greater than zero." << std::endl;
		return;
	}

	// Use float division to correctly calculate sprite size
	this->animatedSpriteSize.x = 1.0f / numCol;
	this->animatedSpriteSize.y = 1.0f / numRows;
}


void GameObject::createNewAnimation(const char* animationString, unsigned int animationStart, unsigned int animationEnd)
{
	// Check if sprite has been sliced before creating an animation
	if (animatedSpriteSize.x == 1.0f && animatedSpriteSize.y == 1.0f) {
		std::cout << "Sprite has not been sliced yet, unable to create a new animation!" << std::endl;
		return;
	}

	// Calculate number of rows and columns
	unsigned int numRows = static_cast<unsigned int>(1.0f / animatedSpriteSize.y);
	unsigned int numColumns = static_cast<unsigned int>(1.0f / animatedSpriteSize.x);

	// Ensure that numRows and numColumns are not zero to prevent division errors
	if (numRows == 0 || numColumns == 0) {
		std::cerr << "Error: Invalid number of rows or columns after slicing." << std::endl;
		return;
	}

	// Clamp the animation start and end indices
	animationStart = Math::Clamp(animationStart, 0u, numRows * numColumns - 1);
	animationEnd = Math::Clamp(animationEnd, 0u, numRows * numColumns - 1);

	// Calculate sprite positions
	int spriteStartX = animationStart % numColumns;
	int spriteStartY = animationStart / numColumns;

	int spriteEndX = animationEnd % numColumns;
	int spriteEndY = animationEnd / numColumns;

	// Insert the sprite animation into the map
	spriteAnimationMap.insert(std::make_pair(animationString, new spriteAnimation(spriteStartX, spriteStartY, spriteEndX, spriteEndY)));
}


void GameObject::setCurrentAnimation(const char* animationString)
{
	//run a check to see if the animation string exists using std::map.find() (returns std::map.end() if not found)
	std::map<const char*, spriteAnimation*>::iterator it = spriteAnimationMap.find(animationString);
	if (it == spriteAnimationMap.end()) 
	{
		//if not found, log error and end program
		std::cout << "unable to loacate animation name: " << animationString << std::endl;
		return;
	}

	currentAnimation = spriteAnimationMap.at(animationString);
	isPlayingAnimation = false;
}

void GameObject::playAnimation(float frameDelay, float dt)
{
	if (currentAnimation == nullptr) 
	{ 
		std::cout << "Unable to play animation, no animation has been defined!" << std::endl;
		return; 
	}

	if (!isPlayingAnimation)
	{
		isPlayingAnimation = true;
		this->mesh = MeshBuilder::animatedMesh("animated sprite", currentAnimation->getAnimStartX(), currentAnimation->getAnimStartY());
		animationFrameTimer = 0.f;
		animationFrameX = currentAnimation->getAnimStartX();
		animationFrameY = currentAnimation->getAnimStartY();
		return;
	}

	animationFrameTimer += dt;


	if (animationFrameTimer > frameDelay)
	{
		animationFrameX++;
		if (animationFrameX > 1 / this->animatedSpriteSize.x) //our sprite animation on the x axis has exceeded the maximum column count
		{
			animationFrameX = 0;
			animationFrameY++;
		}

		this->mesh = MeshBuilder::animatedMesh("animated sprite", animationFrameX, animationFrameY);

		if (animationFrameX == currentAnimation->getAnimEndX() && animationFrameY == currentAnimation->getAnimEndY())
		{
			animationFrameX = currentAnimation->getAnimStartX();
			animationFrameY = currentAnimation->getAnimStartY();
		}
	}

}
//
//void GameObject::playAnimation(float frameDelay, float dt, unsigned int iterationCount)
//{
//}
//
//void GameObject::playAnimation(float frameDelay, float dt, float animationDuration)
//{
//}

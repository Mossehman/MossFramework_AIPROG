#include "GameObject.h"
#include "GL/glew.h"
#include "ImageLoader.h"
#include "MyMath.h"
#include "Map2D.h"


void GameObject::UpdateAnimations(double dt)
{
	if (Animations.size() <= 0 || CurrentAnimationParam == nullptr || !isPlayingAnimation) 
	{
		SpriteFrameTimer = 0;
		return; 
	}

	mesh = MeshBuilder::GenerateAnimatedQuad("Animated Quad", GetColRow(CurrentFrameIndex), SpriteTextureSize);
	mesh->textureID = textureID;
	
	SpriteFrameTimer += dt;
	if (SpriteFrameTimer >= CurrentAnimationParam->frameDelay)
	{
		SpriteFrameTimer = 0;
		CurrentFrameIndex++;

		if (CurrentFrameIndex > CurrentAnimationParam->EndIndex) { CurrentFrameIndex = CurrentAnimationParam->StartIndex; }
	}

}

int GameObject::GetFrameIndex(int Column, int Row)
{
	int colClamped = glm::clamp(Column, 1, (int)SpriteColumns);
	int rowClamped = glm::clamp(Row, 1, (int)SpriteRows);

	return (rowClamped - 1) * SpriteColumns + colClamped - 1;

}

glm::ivec2 GameObject::GetColRow(int FrameIndex)
{
	int frameIndexClamped = glm::clamp(FrameIndex, 0, (int)SpriteRows * (int)SpriteColumns - 1);
	int y = frameIndexClamped  / SpriteColumns + 1;
	int x = frameIndexClamped % SpriteColumns + 1;

	return glm::ivec2(x, y);

}

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

}

GameObject::~GameObject()
{
	if (this->mesh)
	{
		delete this->mesh;
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
	cachedScale = newScl;
}

glm::vec2 GameObject::getScale()
{
	return scale;
}

void GameObject::SetMesh(Mesh* newMesh)
{
	mesh = newMesh;
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

void GameObject::Init(std::vector<GameObject*>& objList, int currentLevel)
{
	this->currentLevel = currentLevel;
	objList.push_back(this);
}

void GameObject::Render()
{
	if (!toRender || currentLevel != Map2D::GetInstance()->GetCurrentLevel()) { return; }

	RenderParameters::GetInstance()->modelStack.PushMatrix();
	RenderParameters::GetInstance()->modelStack.Translate(position.x, position.y, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(rotation.x, 1, 0, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(rotation.y, 0, 1, 0);
	RenderParameters::GetInstance()->modelStack.Scale(scale.x, scale.y, 1);

	if (!child.empty())
	{
		for (int i = 0; i < child.size(); i++)
		{
			if (child[i] == nullptr) { continue; }
			child[i]->Render();
		}
	}

	//std::cout << position.x << "," << position.y << std::endl;

	RenderParameters::GetInstance()->modelStack.PushMatrix();
	RenderParameters::GetInstance()->modelStack.Translate(self_position.x, self_position.y, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(self_rotation.x, 1, 0, 0);
	RenderParameters::GetInstance()->modelStack.Rotate(self_rotation.y, 0, 1, 0);
	RenderParameters::GetInstance()->modelStack.Scale(self_scale.x, self_scale.y, 1);
	

	RenderMesh();

	RenderParameters::GetInstance()->modelStack.PopMatrix();
	RenderParameters::GetInstance()->modelStack.PopMatrix();
}

void GameObject::RenderMesh()
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

void GameObject::SetTexture(const char* texture, unsigned int NumColumns, unsigned int NumRows)
{
	textureID = ImageLoader::GetInstance()->LoadTextureGetID(texture, true);
	mesh->textureID = textureID;
	//std::cout << textureID << std::endl;
	if (NumColumns < 1 || NumRows < 1) { return; }
	if (NumColumns == 1 && NumRows == 1) { return; }

	isPlayingAnimation = true;
	SpriteRows = NumRows;
	SpriteColumns = NumColumns;
	SpriteTextureSize = glm::vec2(1 / (float)NumColumns, 1 / (float)NumRows);
}

void GameObject::AddAnimation(std::string animName, AnimatedSpriteParam* newAnimation)
{
	if (Animations.count(animName) > 0 || newAnimation == nullptr) { return; }
	Animations.insert(std::pair<std::string, AnimatedSpriteParam*>(animName, newAnimation));
}

void GameObject::SetCurrentAnimation(std::string animName)
{
	if (Animations.count(animName) <= 0) { return; }
	std::cout << "Animation set to: " << animName << std::endl;
	CurrentAnimationParam = Animations.at(animName);
	CurrentAnimationID = animName;
}

int GameObject::GetCurrentAnimationFrame(void)
{
	return this->CurrentFrameIndex;
}

AnimatedSpriteParam* GameObject::GetCurrentAnimationParam(void)
{
	return this->CurrentAnimationParam;
}

int GameObject::HandleMessage(BaseMessage* msg)
{
	return 0;
}

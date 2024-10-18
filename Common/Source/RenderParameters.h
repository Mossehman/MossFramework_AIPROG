#pragma once
#include "GL\glew.h"
#include "GLFW/glfw3.h"


#include "DesignPatterns/SingletonTemplate.h"
#include "MatrixStack.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "glm/glm.hpp"
#include "Camera.h"

class RenderParameters : public SingletonTemplate<RenderParameters>
{
	friend SingletonTemplate<RenderParameters>;

public:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_OPACITY,
		U_TOTAL,
	};

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	unsigned m_vertexArrayID;

	MS modelStack, viewStack, projectionStack;

	void Init(glm::vec4 bgColor, const char* vertexShader, const char* fragmentShader);
	void Render(Camera& camera, float width, float height);

	void setWindowReference(GLFWwindow*& window);
	GLFWwindow*& getCurrentWindow(void);

	void setWindowSize(float windowSizeX, float windowSizeY);
	float getWindowSizeX(void);
	float getWindowSizeY(void);

protected:
	RenderParameters() {};
	~RenderParameters();
	GLFWwindow* currentWindow;

	float windowSizeX = 1200.f, windowSizeY = 800.f;
};


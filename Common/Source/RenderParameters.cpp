#include "RenderParameters.h"
#include "shader.hpp"

void RenderParameters::Init(glm::vec4 bgColor, const char* vertexShader, const char* fragmentShader)
{
	glClearColor(bgColor.x, bgColor.y, bgColor.z, bgColor.w); //set our background color
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders(vertexShader, fragmentShader);
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_OPACITY] = glGetUniformLocation(m_programID, "opacity");

	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
}

void RenderParameters::Render(Camera& camera, float width, float height)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Mtx44 projection;
	projection.SetToOrtho(-width / 2, width / 2, -height / 2, height / 2, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	
	modelStack.LoadIdentity();
}

void RenderParameters::setWindowReference(GLFWwindow*& window)
{
	this->currentWindow = window;
}

GLFWwindow*& RenderParameters::getCurrentWindow(void)
{
	return currentWindow;
}

void RenderParameters::setWindowSize(float windowSizeX, float windowSizeY)
{
	if (windowSizeX > 0 && windowSizeY > 0)
	{
		this->windowSizeX = windowSizeX;
		this->windowSizeY = windowSizeY;
		return;
	}
	std::cout << "Window resize values were negative, unable to cache!" << std::endl;
	return;
}

float RenderParameters::getWindowSizeX(void)
{
	return windowSizeX;
}

float RenderParameters::getWindowSizeY(void)
{
	return windowSizeY;
}

RenderParameters::~RenderParameters()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

#include "GameStateWin.h"

#include <sstream>
#include <KeyboardController.h>
#include <LoadTGA.h>

bool GameStateWin::Init()
{
	RenderParameters::GetInstance()->Init(glm::vec4(0.f, 0.f, 0.3f, 0.8f), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
	camera.Init(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera.ToClamp = false;
	textMesh->textureID = LoadTGA("Image//calibri.tga");

	return true;
}

bool GameStateWin::Update(double dt)
{
    return true;
}

void GameStateWin::Render()
{
	std::ostringstream ss;
	RenderParameters::GetInstance()->Render(camera, 1200, 800);

	ss.str("");
	ss << "You Won! Press R to restart";
	RenderTextOnScreen(textMesh, "You Won! Press R to restart", Color(0, 1, 0), 3, 50, 14);
}

void GameStateWin::Destroy()
{
}

void GameStateWin::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);

	RenderParameters::GetInstance()->projectionStack.PushMatrix();
	RenderParameters::GetInstance()->projectionStack.LoadMatrix(ortho);
	RenderParameters::GetInstance()->viewStack.PushMatrix();
	RenderParameters::GetInstance()->viewStack.LoadIdentity();
	RenderParameters::GetInstance()->modelStack.PushMatrix();
	RenderParameters::GetInstance()->modelStack.LoadIdentity();
	RenderParameters::GetInstance()->modelStack.Translate(x, y, 0);
	RenderParameters::GetInstance()->modelStack.Scale(size, size, size);
	glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_TEXT_ENABLED], 1);
	glUniform3fv(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_TEXT_COLOR], 1, &color.r);
	glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_LIGHTENABLED], 0);
	glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_COLOR_TEXTURE], 0);
	float accum = 0;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(accum + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = RenderParameters::GetInstance()->projectionStack.Top() * RenderParameters::GetInstance()->viewStack.Top() * RenderParameters::GetInstance()->modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);

		accum += (float)fontWidth[(unsigned)text[i]] / 64;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(RenderParameters::GetInstance()->m_parameters[RenderParameters::GetInstance()->U_TEXT_ENABLED], 0);
	RenderParameters::GetInstance()->modelStack.PopMatrix();
	RenderParameters::GetInstance()->viewStack.PopMatrix();
	RenderParameters::GetInstance()->projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

#include "SceneCell.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Mtx44.h"
#include "LoadTGA.h"

#include "MouseController.h"

#define WIDTH 1200 //corresponds to window width
#define HEIGHT 800 //corresponds to window height

//#include <Windows.h>

#define time_scale 1
#define air_resistance 100;
#define gravity_constant -10;

SceneCell::SceneCell()
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		yellow_Obj[i] = NULL;
		red_Obj[i] = NULL;
		green_Obj[i] = NULL;
		blue_Obj[i] = NULL;
	}
}

SceneCell::~SceneCell()
{
}

bool SceneCell::Init()
{
	//Calculating aspect ratio
	m_worldHeight = 600.f;
	m_worldWidth = m_worldHeight * (800.f / 600.f);

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Load the shader programs
	m_programID = LoadShaders("Shader//comg.vertexshader",
								"Shader//comg.fragmentshader");
	glUseProgram(m_programID);

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");

	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Init VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}

	/*redRep.Set();
	greenRep.Set();
	blueRep.Set();
	yellowRep.Set();*/

	randParticles(); //Delete this if you do not want to randomise particle behaviour

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("Axes", 10000.f, 10000.f, 10000.f);
	meshList[GEO_YELLOW] = MeshBuilder::GeneratePolygon("Sphere", Color(1.0f, 0.984f, 0.f), 1, 24.f);
	meshList[GEO_RED] = MeshBuilder::GeneratePolygon("Sphere", Color(1.0f, 0.f, 0.f), 1, 24.f);
	meshList[GEO_GREEN] = MeshBuilder::GeneratePolygon("Sphere", Color(0.f, 1.0f, 0.f), 1, 24.f);
	meshList[GEO_BLUE] = MeshBuilder::GeneratePolygon("Sphere", Color(0.f, 0.f, 1.0f), 1, 24.f);
	meshList[GEO_CURSOR] = MeshBuilder::GeneratePolygonLine("Sphere", Color(1.f, 1.f, 1.0f), 1, 24.f);

	/*for (int i = 0; i < MAX_PARTICLES; i++)
	{
		yellow_Obj[i] = new GameObject(GameObject::GO_BALL);
		yellow_Obj[i]->pos = Vector3(Math::RandFloatMinMax(-WIDTH/2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2), 0.f);
		yellow_Obj[i]->scale = Vector3(particleSize, particleSize, 1.f);
		yellow_Obj[i]->vel = Vector3(0.f, 0.f, 0.f);
		yellow_Obj[i]->accl = Vector3(0.f, 0.f, 0.f);
		yellow_Obj[i]->active = true;

		red_Obj[i] = new GameObject(GameObject::GO_BALL);
		red_Obj[i]->pos = Vector3(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2), 0.f);
		red_Obj[i]->scale = Vector3(particleSize, particleSize, 1.f);
		red_Obj[i]->vel = Vector3(0.f, 0.f, 0.f);
		red_Obj[i]->accl = Vector3(0.f, 0.f, 0.f);
		red_Obj[i]->active = true;

		green_Obj[i] = new GameObject(GameObject::GO_BALL);
		green_Obj[i]->pos = Vector3(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2), 0.f);
		green_Obj[i]->scale = Vector3(particleSize, particleSize, 1.f);
		green_Obj[i]->vel = Vector3(0.f, 0.f, 0.f);
		green_Obj[i]->accl = Vector3(0.f, 0.f, 0.f);
		green_Obj[i]->active = true;

		blue_Obj[i] = new GameObject(GameObject::GO_BALL);
		blue_Obj[i]->pos = Vector3(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2), 0.f);
		blue_Obj[i]->scale = Vector3(particleSize, particleSize, 1.f);
		blue_Obj[i]->vel = Vector3(0.f, 0.f, 0.f);
		blue_Obj[i]->accl = Vector3(0.f, 0.f, 0.f);
		blue_Obj[i]->active = true;
	}

	mouse[0] = new GameObject(GameObject::GO_BALL);
	mouse[0]->pos.SetZero();
	mouse[0]->scale = Vector3(cursorRadius, cursorRadius, 1.f);
	mouse[0]->active = true; 
	
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");*/
	return true;

}

bool SceneCell::Update(double dt)
{
	//// Check for key press, you can add more for interaction
	//HandleKeyPress();

	//mouseX = CMouseController::GetInstance()->GetMousePositionX() - WIDTH/2;
	//mouseY = HEIGHT/2 - CMouseController::GetInstance()->GetMousePositionY();
	//mouse[0]->pos = Vector3(mouseX, mouseY, 0.f);


	//
	//updateParticle(); //Live updates all particles, VERY INEFFECIENT, try making yr own function without using literally every rule

	//for (int i = 0; i < MAX_PARTICLES; i++) //this sets the borders for the particles(causes them to wrap to the other side of the screen)
	//{
	//	posWrap(red_Obj[i]);
	//	posWrap(blue_Obj[i]);
	//	posWrap(green_Obj[i]);
	//	posWrap(yellow_Obj[i]);
	//}

	//if (Application::IsKeyPressed(0xBB)) //Press '+' to increase Cursor radius
	//{
	//	cursorRadius += dt * 10;
	//}

	//if (Application::IsKeyPressed(0xBD)) //Press '-' to reduce Cursor radius
	//{
	//	cursorRadius -= dt * 10;
	//}

	//if (cursorRadius < 1)
	//{
	//	cursorRadius = 1;
	//}

	//if (Application::IsKeyPressed('F')) //Check framerate
	//{
	//	std::cout << 1 / dt << std::endl;
	//}
	return true;
}

void SceneCell::Render()
{
	//// Clear color buffer every frame
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	////Projection matrix : Orthographic Projection
	//Mtx44 projection;
	//projection.SetToOrtho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2, -10, 10);
	//projectionStack.LoadMatrix(projection);

	//// Camera matrix
	//viewStack.LoadIdentity();
	//viewStack.LookAt(
	//	camera.position.x, camera.position.y, camera.position.z,
	//	camera.target.x, camera.target.y, camera.target.z,
	//	camera.up.x, camera.up.y, camera.up.z
	//);
	//// Model matrix : an identity matrix (model will be at the origin)
	//modelStack.LoadIdentity();

	////RenderMesh(meshList[GEO_AXES], false);

	//// Render objects
	//for (int i = 0; i < MAX_PARTICLES; i++)
	//{

	//	modelStack.PushMatrix();
	//	modelStack.Translate(green_Obj[i]->pos.x, green_Obj[i]->pos.y, green_Obj[i]->pos.z);
	//	modelStack.Scale(green_Obj[i]->scale.x, green_Obj[i]->scale.y, green_Obj[i]->scale.z);
	//	RenderMesh(meshList[GEO_GREEN], false);
	//	modelStack.PopMatrix();

	//	modelStack.PushMatrix();
	//	modelStack.Translate(yellow_Obj[i]->pos.x, yellow_Obj[i]->pos.y, yellow_Obj[i]->pos.z);
	//	modelStack.Scale(yellow_Obj[i]->scale.x, yellow_Obj[i]->scale.y, yellow_Obj[i]->scale.z);
	//	RenderMesh(meshList[GEO_YELLOW], false);
	//	modelStack.PopMatrix();


	//	modelStack.PushMatrix();
	//	modelStack.Translate(red_Obj[i]->pos.x, red_Obj[i]->pos.y, red_Obj[i]->pos.z);
	//	modelStack.Scale(red_Obj[i]->scale.x, red_Obj[i]->scale.y, red_Obj[i]->scale.z);
	//	RenderMesh(meshList[GEO_RED], false);
	//	modelStack.PopMatrix();

	//	modelStack.PushMatrix();
	//	modelStack.Translate(blue_Obj[i]->pos.x, blue_Obj[i]->pos.y, blue_Obj[i]->pos.z);
	//	modelStack.Scale(blue_Obj[i]->scale.x, blue_Obj[i]->scale.y, blue_Obj[i]->scale.z);
	//	RenderMesh(meshList[GEO_BLUE], false);
	//	modelStack.PopMatrix();
	//	
	//}

	//modelStack.PushMatrix();
	//modelStack.Translate(mouseX, mouseY, 0);
	//modelStack.Scale(cursorRadius, cursorRadius, 0);
	//RenderMesh(meshList[GEO_CURSOR], false);
	//modelStack.PopMatrix();

}

void SceneCell::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	mesh->Render();
}

void SceneCell::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

void SceneCell::HandleKeyPress() 
{
	//{
	//	if (Application::IsKeyPressed(0x31))
	//	{
	//		// Key press to enable culling
	//		glEnable(GL_CULL_FACE);
	//	}
	//	if (Application::IsKeyPressed(0x32))
	//	{
	//		// Key press to disable culling
	//		glDisable(GL_CULL_FACE);
	//	}
	//	if (Application::IsKeyPressed(0x33))
	//	{
	//		// Key press to enable fill mode for the polygon
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	//	}
	//	if (Application::IsKeyPressed(0x34))
	//	{
	//		// Key press to enable wireframe mode for the polygon
	//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
	//	}
	//}
	
	//Disabled the default wireframe/backface culling code for the cursor usage

	//if (Application::IsKeyPressed('R')) //Randomises the particle position
	//{
	//	for (int i = 0; i < MAX_PARTICLES; i++)
	//	{
	//		yellow_Obj[i]->pos = Vector3(Math::RandFloatMinMax(-WIDTH/2, WIDTH/2), Math::RandFloatMinMax(-HEIGHT/2, HEIGHT/2), 0.f);
	//		red_Obj[i]->pos = Vector3(Math::RandFloatMinMax(-WIDTH/2, WIDTH/2), Math::RandFloatMinMax(-HEIGHT/2, HEIGHT/2), 0.f);
	//		green_Obj[i]->pos = Vector3(Math::RandFloatMinMax(-WIDTH/2, WIDTH/2), Math::RandFloatMinMax(-HEIGHT/2, HEIGHT/2), 0.f);
	//		blue_Obj[i]->pos = Vector3(Math::RandFloatMinMax(-WIDTH/2, WIDTH/2), Math::RandFloatMinMax(-HEIGHT/2, HEIGHT/2), 0.f);
	//	}
	//}
	//if (Application::IsKeyPressed('Z')) //Randomises the particle behaviour
	//{
	//	randParticles();
	//}
	//if (Application::IsKeyPressed('B'))
	//{
	//	if (!saveParticles) {
	//		std::cout << "___Current Particle arrangement___" << std::endl;
	//		std::cout << "redMag.Set( " << redMag.red << ", " << redMag.green << ", " << redMag.blue << ", " << redMag.yellow << ");" << std::endl;
	//		std::cout << "greenMag.Set( " << greenMag.red << ", " << greenMag.green << ", " << greenMag.blue << ", " << greenMag.yellow << ");" << std::endl;
	//		std::cout << "blueMag.Set( " << blueMag.red << ", " << blueMag.green << ", " << blueMag.blue << ", " << blueMag.yellow << ");" << std::endl;
	//		std::cout << "yellowMag.Set( " << yellowMag.red << ", " << yellowMag.green << ", " << yellowMag.blue << ", " << yellowMag.yellow << ");" << std::endl;

	//		std::cout << std::endl;
	//		std::cout << "redRep.Set(" << redDist.red << ", " << redDist.green << ", " << redDist.blue << ", " << redDist.yellow << ");" << std::endl;
	//		std::cout << "greenRep.Set(" << greenDist.red << ", " << greenDist.green << ", " << greenDist.blue << ", " << greenDist.yellow << ");" << std::endl;
	//		std::cout << "blueRep.Set(" << blueDist.red << ", " << blueDist.green << ", " << blueDist.blue << ", " << blueDist.yellow << ");" << std::endl;
	//		std::cout << "yellowRep.Set(" << yellowDist.red << ", " << yellowDist.green << ", " << yellowDist.blue << ", " << yellowDist.yellow << ");" << std::endl << std::endl << std::endl;
	//		
	//		saveParticles = true;
	//	}
	//}
	//else
	//{
	//	saveParticles = false;
	//}

	////R G B Y = 1, 2, 3, 4
	//if (Application::IsKeyPressed(0x01)) { //Left mouse button for repulsion(based on radius)
	//	if (Application::IsKeyPressed(0x31)) //Number 1 for Red
	//	{
	//		Rule(red_Obj, mouse, cursorMagnitude, cursorRadius * 2, MAX_PARTICLES, 1); 
	//	}
	//	if (Application::IsKeyPressed(0x32)) //Number 2 for Green
	//	{
	//		Rule(green_Obj, mouse, cursorMagnitude, cursorRadius * 2, MAX_PARTICLES, 1);
	//	}
	//	if (Application::IsKeyPressed(0x33)) //Number 3 for Blue
	//	{
	//		Rule(blue_Obj, mouse, cursorMagnitude, cursorRadius * 2, MAX_PARTICLES, 1);
	//	}
	//	if (Application::IsKeyPressed(0x34)) //Number 4 for Yellow
	//	{
	//		Rule(yellow_Obj, mouse, cursorMagnitude, cursorRadius * 2, MAX_PARTICLES, 1);
	//	}
	//}
	//else if (Application::IsKeyPressed(0x02)) { //Right mouse button for Attraction(based on radius)
	//	if (Application::IsKeyPressed(0x31))
	//	{
	//		Rule(red_Obj, mouse, -cursorMagnitude, cursorRadius * 2, MAX_PARTICLES, 1);
	//	}
	//	if (Application::IsKeyPressed(0x32))
	//	{
	//		Rule(green_Obj, mouse, -cursorMagnitude, cursorRadius * 2, MAX_PARTICLES, 1);
	//	}
	//	if (Application::IsKeyPressed(0x33))
	//	{
	//		Rule(blue_Obj, mouse, -cursorMagnitude, cursorRadius * 2, MAX_PARTICLES, 1);
	//	}
	//	if (Application::IsKeyPressed(0x34))
	//	{
	//		Rule(yellow_Obj, mouse, -cursorMagnitude, cursorRadius * 2, MAX_PARTICLES, 1);
	//	}
	//}

}

void SceneCell::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SceneCell::Rule(GameObject* obj1[], GameObject* obj2[], float g, float range, int obj1ArrSize, int obj2ArrSize)
{
	//for (int i = 0; i < obj1ArrSize; i++)
	//{
	//	float fx = 0;
	//	float fy = 0;
	//	GameObject* a = nullptr;
	//	GameObject* b = nullptr;
	//	float value = 0.5;

	//	for (int j = 0; j < obj2ArrSize; j++)
	//	{
	//		a = obj1[i];
	//		b = obj2[j];

	//		float dx = a->pos.x - b->pos.x;
	//		float dy = a->pos.y - b->pos.y;

	//		float distSq = Math::Square(dx) + Math::Square(dy);

	//		if (distSq > Math::Square(particleSize) && distSq < Math::Square(range/2))
	//		{

	//			float Force = g * 1 * findSqrt(distSq);
	//			fx += (Force * dx);
	//			fy += (Force * dy);
	//		}
	//		else if (distSq <= Math::Square(particleSize))
	//		{
	//			float Force = 1 * findSqrt(distSq);
	//			fx += (Force * dx);
	//			fy += (Force * dy);
	//		}
	//	}

	//	// Check if a is not nullptr before accessing its members
	//	if (a)
	//	{
	//		a->vel.x = (a->vel.x + fx) * value;
	//		a->vel.y = (a->vel.y + fy) * value;
	//		/*if (a->pos.x <= -WIDTH/2 || a->pos.x >= WIDTH/2)
	//		{
	//			a->vel.x *= -5;
	//		}
	//		if (a->pos.y <= -HEIGHT/2 || a->pos.y >= HEIGHT/2)
	//		{
	//			a->vel.y *= -5;
	//		}*/
	//		a->pos.x += a->vel.x;
	//		a->pos.y += a->vel.y;
	//	}
	//}
}

void SceneCell::posWrap(GameObject* obj)
{
	//if (obj->pos.x < -WIDTH/2)
	//{
	//	obj->pos.x = WIDTH/2 - 1;
	//}
	//if (obj->pos.x > WIDTH/2)
	//{
	//	obj->pos.x = -WIDTH/2 + 1;
	//}
	//if (obj->pos.y < -HEIGHT/2)
	//{
	//	obj->pos.y = HEIGHT/2 - 1;
	//}
	//if (obj->pos.y > HEIGHT/2)
	//{
	//	obj->pos.y = -HEIGHT/2 + 1;
	//}
}

float SceneCell::findSqrt(float n) //quick inverse sqrt
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = n * 0.5F;
	y = n;
	i = *(long*)&y;           
	i = 0x5f3759df - (i >> 1);      
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y)); 
	// y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

inline void SceneCell::updateParticle()
{
	Rule(red_Obj, red_Obj, redMag.red, redDist.red);
	Rule(red_Obj, green_Obj, redMag.green, redDist.green);
	Rule(red_Obj, blue_Obj, redMag.blue, redDist.blue);
	Rule(red_Obj, yellow_Obj, redMag.yellow, redDist.yellow);

	Rule(green_Obj, red_Obj, greenMag.red, greenDist.red);
	Rule(green_Obj, green_Obj, greenMag.green, greenDist.green);
	Rule(green_Obj, blue_Obj, greenMag.blue, greenDist.blue);
	Rule(green_Obj, yellow_Obj, greenMag.yellow, greenDist.yellow);

	Rule(blue_Obj, red_Obj, blueMag.red, blueDist.red);
	Rule(blue_Obj, green_Obj, blueMag.green, blueDist.green);
	Rule(blue_Obj, blue_Obj, blueMag.blue, blueDist.blue);
	Rule(blue_Obj, yellow_Obj, blueMag.yellow, blueDist.yellow);

	Rule(yellow_Obj, green_Obj, yellowMag.green, yellowDist.green);
	Rule(yellow_Obj, red_Obj, yellowMag.red, yellowDist.red);
	Rule(yellow_Obj, blue_Obj, yellowMag.blue, yellowDist.blue);
	Rule(yellow_Obj, yellow_Obj, yellowMag.yellow, yellowDist.yellow);
}

void SceneCell::randParticles()
{
	yellowDist.Set(Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600));
	greenDist.Set(Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600));
	redDist.Set(Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600));
	blueDist.Set(Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600), Math::RandFloatMinMax(100, 600));

	yellowMag.Set(Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2));
	greenMag.Set(Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2));
	redMag.Set(Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2));
	blueMag.Set(Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2), Math::RandFloatMinMax(-2, 2));
}

//Saved codes that do not follow the layout
/*
Cool Arrangements:

___Current Particle arrangement___
Order: Red, Green, Blue, Yellow
Red Magnitude: -0.994476, -0.720054, -0.967986, -1.96289
Green Magnitude: -1.63414, -0.815027, -1.88525, 1.39354
Blue Magnitude: 1.61498, 1.98108, -0.474197, 1.7589
Yellow Magnitude: -0.534135, 1.82214, 1.96594, 1.0781

Red Range: 505.21, 443.059, 254.576, 534.69
Green Range: 300.552, 365.16, 456.883, 384.967
Blue Range: 523.505, 104.41, 485.342, 328.889
Yellow Range: 420.521, 220.334, 186.627, 528.007



___Current Particle arrangement___
Order: Red, Green, Blue, Yellow
Red Magnitude: 0.632893, -0.387036, 1.27744, -0.466628
Green Magnitude: 1.99609, -1.249, 1.082, 1.97302
Blue Magnitude: 1.07004, 0.292672, 0.193671, -1.18345
Yellow Magnitude: -1.80541, 1.78649, -1.44908, -0.119083

Red Range: 223.707, 274.917, 561.776, 512.748
Green Range: 173.611, 427.555, 328.05, 553.81
Blue Range: 447.972, 587.045, 191.8, 111.216
Yellow Range: 398.471, 593.896, 407.474, 133.998


___Current Particle arrangement___
Order: Red, Green, Blue, Yellow
Red Magnitude: 0.0681784, 1.85266, -0.726402, -1.61681
Green Magnitude: -1.03488, -0.618732, -0.381665, 1.75695
Blue Magnitude: -0.576739, -0.588824, -1.66137, -1.70995
Yellow Magnitude: -0.586993, -0.713828, 0.346019, -1.93139

Red Range: 529.853, 389.499, 294.983, 413.974
Green Range: 422.275, 401.111, 142.756, 342.363
Blue Range: 554.588, 191.784, 561.501, 424.335
Yellow Range: 443.745, 192.288, 478.46, 488.73


___Current Particle arrangement___
Order: Red, Green, Blue, Yellow
Red Magnitude: 0.100406, 0.064394, -0.22541, -1.32151
Green Magnitude: -0.760338, -0.478469, 1.4912, -0.21247
Blue Magnitude: 0.599323, -0.885586, 1.59069, 0.435011
Yellow Magnitude: -1.39341, -1.34764, 1.91223, -1.42747

Red Range: 472.585, 548.576, 513.678, 560.341
Green Range: 257.46, 191.311, 515.876, 420.872
Blue Range: 274.108, 534.324, 223.981, 213.01
Yellow Range: 542.961, 539.879, 216.962, 204.266

___Current Particle arrangement___
//Order: Red, Green, Blue, Yellow
//Red Magnitude: 1.48106, -0.880581, 0.634724, 0.356151
//Green Magnitude: -1.86096, 1.31028, -1.34739, 1.59337
//Blue Magnitude: 0.284493, 0.76278, -0.477859, -0.711875
//Yellow Magnitude: 1.65307, 0.337107, -0.996185, 1.29197
//
//Red Range: 203.473, 310.715, 113.23, 188.916
//Green Range: 407.52, 318.116, 416.431, 298.92
//Blue Range: 498.419, 418.598, 159.099, 485.189
//Yellow Range: 131.266, 229.948, 421.085, 408.13

___Current Particle arrangement___
//Order: Red, Green, Blue, Yellow
//Red Magnitude: -0.984344, 1.16269, -1.9386, 1.8977
//Green Magnitude: 0.196478, -1.57616, 1.74035, -0.443678
//Blue Magnitude: 0.452956, -0.828089, -1.92383, -0.300241
//Yellow Magnitude: -1.8529, -0.814539, -1.04172, 1.21555
//
//Red Range: 260.848, 343.477, 267.302, 273.757
//Green Range: 463.43, 263.259, 461.675, 588.937
//Blue Range: 234.816, 369.631, 524.207, 265.319
//Yellow Range: 508.704, 419.315, 156.383, 151.103

//___Current Particle arrangement___
//Order: Red, Green, Blue, Yellow
//Red Magnitude: 1.88049, 1.75304, -0.152776, 1.95007
//Green Magnitude: -1.15818, 1.69994, 1.87414, -1.87695
//Blue Magnitude: 0.0419323, 1.37523, -1.99951, -0.909879
//Yellow Magnitude: 0.279977, -1.16111, -1.051, 0.0820949
//
//Red Range: 595.56, 110.544, 497.046, 274.429
//Green Range: 592.141, 557.961, 291.443, 390.521
//Blue Range: 498.251, 167.385, 565.789, 143.413
//Yellow Range: 156.001, 169.414, 414.997, 556.359

___Current Particle arrangement___
//Order: Red, Green, Blue, Yellow
//Red Magnitude: 1.98767, -0.253487, 0.488846, -1.468
//Green Magnitude: -1.14658, -1.36204, 0.405713, -0.447218
//Blue Magnitude: -1.03305, -0.399731, 1.45067, -0.676595
//Yellow Magnitude: -1.60826, -0.0215461, 0.788415, 1.20042
//
//Red Range: 381.03, 505.347, 326.89, 427.387
//Green Range: 238.295, 499.884, 291.305, 270.003
//Blue Range: 517.524, 399.631, 174.48, 297.47
//Yellow Range: 421.924, 111.719, 260.359, 272.643


___Current Particle arrangement___
//Order: Red, Green, Blue, Yellow
//Red Magnitude: 1.8225, -0.732627, -1.78918, -1.78381
//Green Magnitude: -0.0982085, -0.898526, -0.472121, -0.232246
//Blue Magnitude: 1.47765, -0.129948, -1.25047, 1.02939
//Yellow Magnitude: -0.164983, 0.449416, 0.820521, -1.84887
//
//Red Range: 121.912, 534.416, 376.635, 425.907
//Green Range: 428.089, 497.259, 196.683, 597.772
//Blue Range: 173.214, 164.119, 449.391, 534.95
//Yellow Range: 537.056, 349.321, 385.485, 304.031

___Current Particle arrangement___
Order: Red, Green, Blue, Yellow
Red Magnitude: 0.457839, -1.82897, 0.841517, -1.2523
Green Magnitude: 1.61437, 0.344188, 1.28343, -1.56761
Blue Magnitude: 0.462111, 0.531327, -0.891812, 0.243965
Yellow Magnitude: -1.69726, -0.628132, 1.91516, -0.318674

Red Range: 151.79, 362.444, 417.835, 140.361
Green Range: 112.803, 494.574, 561.302, 467.977
Blue Range: 110.3, 433.598, 399.768, 317.78
Yellow Range: 267.028, 391.498, 398.151, 527.976
*/

/* 
Cool arrangements, uses the new layout for std::cout

___Current Particle arrangement___
redMag.Set( -1.96081, -0.986786, 0.0768456, 1.62084);
greenMag.Set( -0.800501, -0.499954, -1.79015, 0.268013);
blueMag.Set( -1.1467, -1.39695, -0.73983, 1.44053);
yellowMag.Set( 1.44908, -0.595538, 1.39, -1.73229);

redRep.Set(579.904, 489.828, 456.258, 510.092);
greenRep.Set(560.128, 409.656, 335.755, 251.25);
blueRep.Set(528.755, 117.655, 212.308, 507.086);
yellowRep.Set(365.328, 228.254, 369.921, 510.978);
*/

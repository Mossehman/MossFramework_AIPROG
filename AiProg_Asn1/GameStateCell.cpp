#include "GameStateCell.h"
#include <Application.h>
#include "Map2D.h"

#define WIDTH 1200 //corresponds to window width
#define HEIGHT 800 //corresponds to window height

bool GameStateCell::Init()
{
	RenderParameters::GetInstance()->Init(glm::vec4(0.f, 0.f, 0.0f, 1.0f), "Shader//comg.vertexshader", "Shader//comg.fragmentshader");
	camera.Init(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	camera.ToClamp = false;



	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		blue_Obj[i] = new GameObject();
		red_Obj[i] = new GameObject();
		yellow_Obj[i] = new GameObject();
		green_Obj[i] = new GameObject();

		blue_Obj[i]->setPosition(glm::vec2(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2)));
		red_Obj[i]->setPosition(glm::vec2(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2)));
		yellow_Obj[i]->setPosition(glm::vec2(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2)));
		green_Obj[i]->setPosition(glm::vec2(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2)));

		blue_Obj[i]->setScale(glm::vec2(2, 2));
		red_Obj[i]->setScale(glm::vec2(2, 2));
		yellow_Obj[i]->setScale(glm::vec2(2, 2));
		green_Obj[i]->setScale(glm::vec2(2, 2));


		blue_Obj[i]->SetMesh(MeshBuilder::GeneratePolygon("Blue circle", Color(0, 0, 1)));
		red_Obj[i]->SetMesh(MeshBuilder::GeneratePolygon("Red circle", Color(1, 0, 0)));
		green_Obj[i]->SetMesh(MeshBuilder::GeneratePolygon("Green circle", Color(0, 1, 0)));
		yellow_Obj[i]->SetMesh(MeshBuilder::GeneratePolygon("Blue circle", Color(1, 0.984f, 0)));
	}
	
	randParticles();

    return true;
}

bool GameStateCell::Update(double dt)
{
	
	if (Application::IsKeyPressed('R')) //Randomises the particle position
	{
		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			blue_Obj[i]->setPosition(glm::vec2(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2)));
			red_Obj[i]->setPosition(glm::vec2(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2)));
			yellow_Obj[i]->setPosition(glm::vec2(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2)));
			green_Obj[i]->setPosition(glm::vec2(Math::RandFloatMinMax(-WIDTH / 2, WIDTH / 2), Math::RandFloatMinMax(-HEIGHT / 2, HEIGHT / 2)));

		}
	}

	if (Application::IsKeyPressed('Z')) //Randomises the particle behaviour
	{
		randParticles();
	}

	updateParticle();

	for (int i = 0; i < MAX_PARTICLES; i++) //this sets the borders for the particles(causes them to wrap to the other side of the screen)
	{
		posWrap(red_Obj[i]);
		posWrap(blue_Obj[i]);
		posWrap(green_Obj[i]);
		posWrap(yellow_Obj[i]);
	}

    return true;
}

void GameStateCell::Render()
{
	RenderParameters::GetInstance()->Render(camera, WIDTH, HEIGHT);

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		red_Obj[i]->Render();
		blue_Obj[i]->Render();
		yellow_Obj[i]->Render();
		green_Obj[i]->Render();
	}

}

void GameStateCell::Destroy()
{
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		delete red_Obj[i];
		delete blue_Obj[i];
		delete green_Obj[i];
		delete yellow_Obj[i];
	}
}

void GameStateCell::Rule(GameObject* obj1[], GameObject* obj2[], float g, float range, int obj1Size, int obj2Size)
{
	for (int i = 0; i < obj1Size; i++)
	{
		float fx = 0;
		float fy = 0;
		GameObject* a = nullptr;
		GameObject* b = nullptr;
		float value = 0.5;

		for (int j = 0; j < obj2Size; j++)
		{
			a = obj1[i];
			b = obj2[j];

			float dx = a->getPosition().x - b->getPosition().x;
			float dy = a->getPosition().y - b->getPosition().y;

			float distSq = Math::Square(dx) + Math::Square(dy);

			if (distSq > Math::Square(1) && distSq < Math::Square(range/2))
			{

				float Force = g * 1 * findSqrt(distSq);
				fx += (Force * dx);
				fy += (Force * dy);
			}
			else if (distSq <= Math::Square(1))
			{
				float Force = 1 * findSqrt(distSq);
				fx += (Force * dx);
				fy += (Force * dy);
			}
		}

		// Check if a is not nullptr before accessing its members
		if (a)
		{
			a->vel.x = (a->vel.x + fx) * value;
			a->vel.y = (a->vel.y + fy) * value;
			/*if (a->pos.x <= -WIDTH/2 || a->pos.x >= WIDTH/2)
			{
				a->vel.x *= -5;
			}
			if (a->pos.y <= -HEIGHT/2 || a->pos.y >= HEIGHT/2)
			{
				a->vel.y *= -5;
			}*/
			a->setPosition(a->getPosition() + a->vel);
		}
	}
}

void GameStateCell::posWrap(GameObject* obj)
{
	if (obj->position.x < -WIDTH/2)
{
	obj->position.x = WIDTH/2 - 1;
}
if (obj->position.x > WIDTH/2)
{
	obj->position.x = -WIDTH/2 + 1;
}
if (obj->position.y < -HEIGHT/2)
{
	obj->position.y = HEIGHT/2 - 1;
}
if (obj->position.y > HEIGHT/2)
{
	obj->position.y = -HEIGHT/2 + 1;
}
}

float GameStateCell::findSqrt(float n)
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

inline void GameStateCell::updateParticle()
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

void GameStateCell::randParticles()
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

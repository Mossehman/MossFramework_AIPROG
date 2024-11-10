#include "BaseLevel.h"
#include "ImageLoader.h"

void BaseLevel::BindTexture(unsigned int ID, const char* textureFile)
{
	unsigned int TextureID = ImageLoader::GetInstance()->LoadTextureGetID(textureFile, true);

	if (TextureID == 0)
	{
		std::cout << "Unable to load texture: " << textureFile << std::endl;
		return;
	}

	textureMap.insert(std::pair<int, int>(ID, TextureID));
}

void BaseLevel::BindPassability(unsigned int ID, bool isPassable)
{
	passabilityMap.insert(std::pair<int, int>(ID, isPassable));
}

void BaseLevel::PerlinNoise1D(int width, float* seed, int octaves, float bias, float* Output)
{
	for (int x = 0; x < width; x++)
	{
		float Noise = 0.f;
		float Scale = 1.0f;
		float Accumulation = 0.f;

		for (int o = 0; o < octaves; o++)
		{
			int Pitch = width >> o;
			if (Pitch == 0) { break; }

			int FirstSample = (x / Pitch) * Pitch;
			int SecondSample = (FirstSample + Pitch) % width;

			float Blend = (float)(x - FirstSample) / (float)Pitch;
			float Sample = (1.0f - Blend) * seed[FirstSample] + Blend * seed[SecondSample];
			Noise += Sample * Scale;

			Accumulation += Scale;
			Scale = Scale / bias;
		}

		Output[x] = Noise / Accumulation;
	}
}

void BaseLevel::PerlinNoise2D(int width, int height, float* seed, int octaves, float bias, float* Output)
{
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			float Noise = 0.f;
			float Scale = 1.0f;
			float Accumulation = 0.f;

			for (int o = 0; o < octaves; o++)
			{
				int Pitch = width >> o;
				if (Pitch == 0) { break; }

				int FirstSampleX = (x / Pitch) * Pitch;
				int FirstSampleY = (y / Pitch) * Pitch;

				int SecondSampleX = (FirstSampleX + Pitch) % width;
				int SecondSampleY = (FirstSampleY + Pitch) % height;

				float BlendX = (float)(x - FirstSampleX) / (float)Pitch;
				float BlendY = (float)(y - FirstSampleY) / (float)Pitch;

				float SampleT = (1.0f - BlendX) * seed[FirstSampleY * width + FirstSampleX] + BlendX * seed[FirstSampleY * width + SecondSampleX];
				float SampleB = (1.0f - BlendX) * seed[SecondSampleY * width + FirstSampleX] + BlendX * seed[SecondSampleY * width + SecondSampleX];

				Noise += (BlendY * (SampleB - SampleT) + SampleT) * Scale;
				Accumulation += Scale;
				Scale = Scale / bias;
			}
			Output[y * width + x] = Noise / Accumulation;
		}
	}
}



void BaseLevel::GenerateSeed(std::vector<float>& seed, unsigned int count, bool overwrite)
{
	std::srand(static_cast<unsigned>(std::time(0)));
	if (overwrite) { seed.clear(); }

	for (int i = 0; i < count; i++)
	{
		seed.push_back(static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
	}
}

void BaseLevel::Init()
{
}

void BaseLevel::Update(float dt)
{
}

void BaseLevel::Render()
{
}

void BaseLevel::SetTileSize(glm::vec2 tileSize)
{
	this->tileSize = tileSize;
}

glm::vec2 BaseLevel::GetTileSize(void)
{
	return tileSize;
}

void BaseLevel::SetTileMesh(Mesh* mesh)
{
	if (mesh == nullptr) { return; }
	this->tileMesh = mesh;
}

Mesh* BaseLevel::GetTileMesh(void)
{
	return this->tileMesh;
}

#pragma once
struct ParticleVar
{
	float green;
	float blue;
	float red;
	float yellow;

	void setAll(float init = 120)
	{
		green = init;
		blue = init;
		red = init;
		yellow = init;
	}

	void Set(float r = 120, float g = 120, float b = 120, float y = 120)
	{
		green = g;
		blue = b;
		red = r;
		yellow = y;
	}
};
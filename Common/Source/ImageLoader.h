#pragma once

//Credit: Toh Da Jun, NYP DXGL Framework, CImageLoader 

#include "filesystem.h"
#include "DesignPatterns/SingletonTemplate.h"

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

class ImageLoader : public SingletonTemplate<ImageLoader>
{
	friend SingletonTemplate<ImageLoader>;

public:
	unsigned char* Load(const char* filename, int& width, int& height, int& nrChannels, const bool bInvert = false);
	unsigned int LoadTextureGetID(const char* filename, const bool bInvert);

protected:
	ImageLoader(void) {};
	virtual ~ImageLoader(void) {};

};


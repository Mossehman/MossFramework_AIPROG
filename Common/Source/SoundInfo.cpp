#include "SoundInfo.h"
#include <iostream>

SoundInfo::SoundInfo()
{
	//fileName = NULL;
	soundSource = nullptr;
	Volume = 1.f;
	isLooped = false;
	soundType = SOUND2D;
	soundPosition = vec3df(0.f, 0.f, 0.f);
}

SoundInfo::~SoundInfo()
{
	if (!soundSource) { return; }
	delete soundSource;
	soundSource = nullptr;
	
	//if (!fileName.c_str()) { return; }
	//free((void*)fileName);
	//fileName = NULL;
}

void SoundInfo::Init(ISoundSource* soundSource, const bool toLoop, SOUNDTYPE soundType, vec3df soundPos)
{
	this->soundSource = soundSource;
	this->isLooped = toLoop;
	this->soundType = soundType;
	this->soundPosition = soundPos;
}

void SoundInfo::setFileName(std::string fileName)
{
	this->fileName = fileName;
}

std::string SoundInfo::getFileName(void)
{
	std::string str(fileName);
	return str;
}

void SoundInfo::setLoopStatus(bool isLooped)
{
	this->isLooped = isLooped;
}

bool SoundInfo::getLoopStatus(void)
{
	return isLooped;
}

SoundInfo::SOUNDTYPE SoundInfo::getSoundType(void) const
{
	return soundType;
}

void SoundInfo::setVolumeValue(float Volume)
{
	this->Volume = Volume;
}

float SoundInfo::getVolumeValue(void)
{
	return Volume;
}

void SoundInfo::setPosition(float x, float y, float z)
{
	soundPosition = vec3df(x, y, z);
}

vec3df SoundInfo::getPosition(void)
{
	return soundPosition;
}

void SoundInfo::setPreload(bool toPreload)
{
	this->toPreload = toPreload;
}

bool SoundInfo::getPreload(void)
{
	return toPreload;
}

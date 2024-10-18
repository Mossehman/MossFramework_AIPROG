#pragma once
#include "DesignPatterns/SingletonTemplate.h"

#include <includes/irrKlang.h>

using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

#include "SoundInfo.h"
#include <map>
#include <vector>

class SoundManager2D : public SingletonTemplate<SoundManager2D>
{
	friend SingletonTemplate<SoundManager2D>;
	
public:

	void Init(void);
	void Update(void);

	void setListenerPosition (float x, float y);
	void setListenerDirection(float x, float y);

	void addSound(std::string songName, std::string fileName, bool toLoop, const bool preLoad = true);
	void playSound(std::string soundName, vec3df soundPos, int pitchPercentageMin = 100, int pitchPercentageMax = 100);

	void setSoundLoop(std::string soundName, bool toLoop);

protected:

	~SoundManager2D(void);

	float sfxVolumeMax;
	float sfxVolume;

	ISoundEngine* soundEngine;

	std::map<std::string, SoundInfo*> soundEffectMap;
	std::vector<ISound*> activeSFX;

	vec3df listenerPos;
	vec3df listenerDir;
};


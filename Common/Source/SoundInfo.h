#pragma once

#include <includes/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

#include <string>

class SoundInfo
{
public:

	enum SOUNDTYPE
	{
		SOUND2D,
		SOUND3D,
		NUM_SOUNDTYPES
	};

	SoundInfo();
	~SoundInfo();

	void Init(ISoundSource* soundSource, const bool toLoop = false, SOUNDTYPE soundType = SOUND2D, vec3df soundPos = vec3df(0.f, 0.f, 0.f));

	void setFileName(std::string fileName);
	std::string getFileName(void);
	
	void setLoopStatus(bool isLooped);
	bool getLoopStatus(void);
	SOUNDTYPE getSoundType(void) const;

	void setVolumeValue(float Volume);
	float getVolumeValue(void);

	void setPosition(float x, float y, float z);
	vec3df getPosition(void);

	void setPreload(bool toPreload);
	bool getPreload(void);

protected:
	ISoundSource* soundSource;
	std::string fileName;
	float Volume;
	bool isLooped;
	SOUNDTYPE soundType;
	vec3df soundPosition;
	bool toPreload;

};


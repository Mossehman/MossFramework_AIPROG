#include "SoundManager2D.h"
#include <iostream>
#include <random>

void SoundManager2D::Init(void)
{
	soundEngine = createIrrKlangDevice(ESOD_WIN_MM, ESEO_MULTI_THREADED);
}

void SoundManager2D::Update(void)
{
	//clear all sounds that have finished playing
	for (int i = 0; i < activeSFX.size(); i++)
	{
		if (activeSFX[i]->isFinished() && !activeSFX[i]->isLooped())
		{
			activeSFX[i]->drop();
			delete activeSFX[i];
			activeSFX.erase(activeSFX.begin() + i);
		}
	}
}

void SoundManager2D::setListenerPosition(float x, float y)
{
	listenerPos.X = x;
	listenerPos.Y = y;
}

void SoundManager2D::setListenerDirection(float x, float y)
{
	listenerDir.X = x;
	listenerDir.Y = y;
}

void SoundManager2D::addSound(std::string soundName, std::string fileName, bool toLoop, const bool preLoad)
{
	if (soundEffectMap.count(soundName) > 0) {
		std::cout << "Duplicate sound effect detected: " << soundName << std::endl;
		return;
	}

	ISoundSource* soundSource = soundEngine->addSoundSourceFromFile(fileName.c_str(), E_STREAM_MODE::ESM_AUTO_DETECT, preLoad);

	if (!soundSource)
	{
		std::cout << "Unable to load sound: " << fileName << ", file not found!" << std::endl;
		return;
	}

	soundSource->setForcedStreamingThreshold(-1);

	SoundInfo* newSound = new SoundInfo();
	newSound->Init(soundSource, toLoop);
	newSound->setFileName(fileName.c_str());
	newSound->setPreload(preLoad);

	soundEffectMap.insert(std::pair<std::string, SoundInfo*>(soundName, newSound));
}

void SoundManager2D::playSound(std::string soundName, vec3df soundPos, int pitchPercentageMin, int pitchPercentageMax)
{
	std::random_device rd;  // Obtain a random number from hardware
	std::default_random_engine generator(rd());

	if (soundEffectMap.count(soundName) <= 0) {
		std::cout << "Unable to find sound effect: " << soundName << std::endl;
		return;
	}
	int targetPitch = 100;

	if (pitchPercentageMax > pitchPercentageMin)
	{
		std::uniform_int_distribution<int> pitchGen(pitchPercentageMin, pitchPercentageMax);
		targetPitch = pitchGen(generator);
	}

	SoundInfo* soundInfo = soundEffectMap.at(soundName);
	soundInfo->setPosition(soundPos.X, soundPos.Y, 0); // Set the position for this instance

	ISound* newSfx = soundEngine->play3D(
		soundInfo->getFileName().c_str(),   // Ensure getFileName() returns a const char*
		soundInfo->getPosition(),
		soundInfo->getLoopStatus(), true
	);

	activeSFX.push_back(newSfx);

	if (!newSfx) {
		std::cout << "Failed to play sound effect: " << soundName << std::endl;
	}
	else {
		// Apply pitch and other settings to the sound
		float pitch = 1.0f + ((targetPitch) / 100.0f);
		newSfx->setPlaybackSpeed(pitch);
		newSfx->setIsPaused(false); // Unpause if needed
	}
}

void SoundManager2D::setSoundLoop(std::string soundName, bool toLoop)
{
	if (soundEffectMap.count(soundName) <= 0) {
		std::cout << "Unable to find sound effect: " << soundName << std::endl;
		return;
	}

	soundEffectMap.at(soundName)->setLoopStatus(toLoop);
}

SoundManager2D::~SoundManager2D(void)
{
	if (soundEngine)
	{
		soundEngine->stopAllSounds();
		soundEngine->drop();
		soundEngine = nullptr;
	}

	for (std::map<std::string, SoundInfo*>::iterator it = soundEffectMap.begin(); it != soundEffectMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	soundEffectMap.clear();

	for (int i = 0; i < activeSFX.size(); i++)
	{
		if (activeSFX[i])
		{
			activeSFX[i]->drop();
			delete activeSFX[i];
			activeSFX.erase(activeSFX.begin() + i);
		}
	}

	activeSFX.clear();
}

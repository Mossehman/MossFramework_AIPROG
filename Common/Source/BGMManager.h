#pragma once
#include "DesignPatterns/SingletonTemplate.h"
#include "SoundInfo.h"
#include <map>

#include <includes/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

#include <vector>
#include <string>
#include "Vertex.h"

class BGMManager: public SingletonTemplate<BGMManager>
{
	friend SingletonTemplate<BGMManager>;

public:
	void Init(void);
	void Update(double dt);

	void addSong(std::string songName, std::string fileName, const bool preLoad = true);
	void removeSong(std::string songName);

	void createPlaylist(std::string playlistName);
	void removePlaylist(std::string playlistName);
	void addSongToList(std::string songName, std::string playlistName);

	void playBGM(void);
	void pauseBGM(void);
	void clearQueue(void);

	void setFadeInOutTime(float fadeInTime, float fadeOutTime);

	void skipSong();

	void queueSong(std::string songName, bool clearCurrentPlaylist = false);
	void queuePlaylist(std::string playlistName, bool clearCurrentPlaylist = false);
	
	void shuffleList(std::string listToShuffle, unsigned int shuffleDepth = 0);
	void shuffleQueue(bool restart = false, unsigned int shuffleDepth = 0);
	
	bool loopQueue;
	bool loopCurrent;

private:

	~BGMManager();

	const float musicVolumeMax = 0.5f;
	float musicVolume = 0.f;

	ik_u32 songRemainingTime = 0.f;
	ik_u32 songLength = 0.f;
	ik_u32 songCurrentTime = 0.f;
	bool isSongFinished = false;

	unsigned int queueIndex = 0;

	bool isPlayingBGM;
	bool isPaused;

	bool toSkip;
	bool musicFadedIn;

	float fadeInTime;
	float fadeOutTime;

	ISoundEngine* soundEngine;

	ISound* currentSong;
	std::vector<SoundInfo*> songQueue;

	std::map<std::string, SoundInfo*> bgmMap;
	std::map<std::string, std::vector<SoundInfo*>> playlistMap;

	std::vector<std::vector<SoundInfo*>> playlists;
};


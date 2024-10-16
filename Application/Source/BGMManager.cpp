#include "BGMManager.h"
#include <iostream>
#include <random>
#include <ostream>
#include <fstream>

void BGMManager::Init(void)
{
	soundEngine = createIrrKlangDevice(ESOD_WIN_MM, ESEO_MULTI_THREADED);
	if (soundEngine == NULL)
	{
		std::cout << "Unable to initialise the IrrKlang sound engine" << std::endl;
		return;
	}

	musicVolume = musicVolumeMax;
	musicFadedIn = true;
	toSkip = false;
}

void BGMManager::Update(double dt)
{
	if (songQueue.size() <= 0) { return; } //no songs queued to play, do not play

	if (currentSong)
	{
		if (currentSong->isFinished()) { songRemainingTime = 0; } //if song has finished playing, hard set the remaining time to 0
		currentSong->setIsPaused(isPaused);

		if (!musicFadedIn)
		{
			float volumeIncrement = musicVolumeMax / fadeInTime;
			musicVolume += volumeIncrement * dt;
			if (musicVolume >= musicVolumeMax)
			{
				musicVolume = musicVolumeMax;
				musicFadedIn = true;
			}
		}
		currentSong->setVolume(musicVolume);
	}

	if (isPaused) { return; }

	if (!isPlayingBGM)
	{
		std::string filePath = songQueue[queueIndex]->getFileName();
		std::ifstream file(filePath);
		if (!file) {
			std::cerr << "File not found: " << filePath << std::endl;
			return;
		}

		queueIndex = 0;
		currentSong = soundEngine->play2D(songQueue[queueIndex]->getFileName().c_str(), false, false, songQueue[queueIndex]->getPreload());
		currentSong->setVolume(musicVolume);
		isPlayingBGM = true;
	}
	else if (isPlayingBGM && (songRemainingTime <= fadeOutTime || toSkip))
	{
		float volumeDecrement = musicVolumeMax / fadeOutTime; 
		musicVolume -= volumeDecrement * dt;

		// Ensure the volume does not go below zero
		if (musicVolume < 0.0f)
			musicVolume = 0.0f;
		if (musicVolume == 0)
		{
			if (!loopCurrent)
			{
				queueIndex++;
				if (queueIndex >= songQueue.size())
				{
					if (loopQueue)
					{
						queueIndex = 0;
					}
					else
					{
						currentSong->stop();
						currentSong = nullptr;
						isPlayingBGM = false;
						songQueue.clear();
					}
				}
			}

			currentSong->stop();
			currentSong = soundEngine->play2D(songQueue[queueIndex]->getFileName().c_str(), false, false, true);
			toSkip = false;
			musicFadedIn = false;
		}
		currentSong->setVolume(musicVolume);
	}

	songLength = currentSong->getPlayLength();
	songRemainingTime = (songLength - currentSong->getPlayPosition()) / 1000.f; //song remaining time in seconds
}

void BGMManager::addSong(std::string songName, std::string fileName, const bool preLoad)
{
	if (bgmMap.count(songName) > 0) 
	{ 
		std::cout << "Duplicate song name (" << songName << ") detected!" << std::endl;
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
	newSound->Init(soundSource);
	newSound->setFileName(fileName.c_str());
	newSound->setPreload(preLoad);

	bgmMap.insert(std::pair<std::string, SoundInfo*>(songName, newSound));
}

void BGMManager::removeSong(std::string songName)
{
	if (bgmMap.count(songName) <= 0)
	{
		std::cout << "Song to remove not found: " << songName << std::endl;
		return;
	}

	SoundInfo* soundToDelete = bgmMap.at(songName);
	bgmMap.erase(songName);

	//remove the deleted bgm from any playlists
	for (int i = 0; i < playlists.size(); i++)
	{
		for (int j = 0; j < playlists[i].size(); j++)
		{
			if (playlists[i][j] == soundToDelete)
			{
				delete playlists[i][j];
				playlists[i][j] = nullptr;
				playlists[i].erase(playlists[i].begin() + j);
			}
		}
	}

}

void BGMManager::createPlaylist(std::string playlistName)
{
	if (playlistMap.count(playlistName) > 0)
	{
		std::cout << "Duplicate playlist name detected: " << playlistName << std::endl;
		return;
	}
	std::vector<SoundInfo*> newPlaylist;
	playlists.push_back(newPlaylist);
	playlistMap.insert(std::pair<std::string, std::vector<SoundInfo*>>(playlistName, newPlaylist));
}

void BGMManager::removePlaylist(std::string playlistName)
{
	if (playlistMap.count(playlistName) <= 0)
	{
		std::cout << "Unable to delete playlist: " << playlistName << ", playlist not found!" << std::endl;
		return;
	}

	std::vector<SoundInfo*> playlistToDelete = playlistMap.at(playlistName);

	for (int i = 0; i < playlists.size(); i++)
	{
		if (playlists[i] == playlistToDelete)
		{
			playlistToDelete.clear();
			playlists.erase(playlists.begin() + i);
		}
	}

	playlistMap.erase(playlistName);
}

void BGMManager::addSongToList(std::string songName, std::string playlistName)
{
	if (bgmMap.count(songName) <= 0 || playlistMap.count(playlistName) <= 0)
	{
		std::cout << "Unable to add song: " << songName << " to playlist: " << playlistName << std::endl;
		return;
	}

	playlistMap.at(playlistName).push_back(bgmMap.at(songName));
}

void BGMManager::playBGM(void)
{
	isPaused = false;
}

void BGMManager::pauseBGM(void)
{
	isPaused = true;
}

void BGMManager::clearQueue(void)
{
	isPlayingBGM = false;
	currentSong = nullptr;
	songQueue.clear();
}

void BGMManager::setFadeInOutTime(float fadeInTime, float fadeOutTime)
{
	this->fadeInTime = fadeInTime;
	this->fadeOutTime = fadeOutTime;
}

void BGMManager::skipSong()
{
	toSkip = true;
}

void BGMManager::queueSong(std::string songName, bool clearCurrentPlaylist)
{
	if (bgmMap.count(songName) <= 0) {
		std::cout << songName << " not found! Unable to queue song!" << std::endl;
		return;
	}

	if (clearCurrentPlaylist) { clearQueue(); }

	songQueue.push_back(bgmMap.at(songName));
}

void BGMManager::queuePlaylist(std::string playlistName, bool clearCurrentPlaylist)
{
	if (playlistMap.count(playlistName) <= 0)
	{
		std::cout << playlistName << "not found! Unable to queue playlist!" << std::endl;
		return;
	}

	if (clearCurrentPlaylist) { clearQueue(); }
	for (int i = 0; i < playlistMap.at(playlistName).size(); i++)
	{
		songQueue.push_back(playlistMap.at(playlistName)[i]);
	}
}

void BGMManager::shuffleList(std::string listToShuffle, unsigned int shuffleDepth)
{
	// Seed the random engine
	std::random_device rd;  // Obtain a random number from hardware
	std::default_random_engine generator(rd());

	if (playlistMap.count(listToShuffle) <= 0) {
		std::cout << "Unable to shuffle list: " << listToShuffle << ", playlist not found!" << std::endl;
		return;
	}

	if (shuffleDepth == 0) { shuffleDepth = playlistMap.at(listToShuffle).size() - 1; }
	for (int i = 0; i < shuffleDepth; i++)
	{
		std::uniform_int_distribution<int> firstSongGen(0, playlistMap.at(listToShuffle).size() - 1);
		std::uniform_int_distribution<int> secondSongGen(0, playlistMap.at(listToShuffle).size() - 1);

		int firstSongIndex = firstSongGen(generator);
		int secondSongIndex = secondSongGen(generator);

		// Ensure that firstSongIndex and secondSongIndex are not the same
		while (secondSongIndex == firstSongIndex) { 
			secondSongIndex = secondSongGen(generator); 
		}

		// Swap the elements
		std::swap(playlistMap.at(listToShuffle)[firstSongIndex], playlistMap.at(listToShuffle)[secondSongIndex]);
	}
}

void BGMManager::shuffleQueue(bool restart, unsigned int shuffleDepth)
{
	if (songQueue.size() <= 0) { return; }

	// Seed the random engine
	std::random_device rd;  // Obtain a random number from hardware
	std::default_random_engine generator(rd()); // Seed generator

	// If shuffleDepth is 0 or greater than the queue size, shuffle the entire queue
	if (shuffleDepth == 0) {
		shuffleDepth = songQueue.size() - 1;
	}

	// Perform the shuffle
	for (unsigned int i = 0; i < shuffleDepth; ++i)
	{
		std::uniform_int_distribution<int> firstSongGen(0, songQueue.size() - 1);
		std::uniform_int_distribution<int> secondSongGen(0, songQueue.size() - 1);

		int firstSongIndex = firstSongGen(generator);
		int secondSongIndex = secondSongGen(generator);

		// Ensure that firstSongIndex and secondSongIndex are not the same
		while (secondSongIndex == firstSongIndex) {
			secondSongIndex = secondSongGen(generator);
		}

		// If we are not restarting the song on shuffle, do not swap the current song's position
		if (!restart && (firstSongIndex == queueIndex || secondSongIndex == queueIndex)) {
			continue;
		}

		// Swap the elements
		std::swap(songQueue[firstSongIndex], songQueue[secondSongIndex]);
	}

	if (restart)
	{
		queueIndex = 0;
		if (currentSong)
		{
			currentSong->stop();
			isPlayingBGM = false;
		}
	}
}

BGMManager::~BGMManager()
{
	if (soundEngine)
	{
		soundEngine->stopAllSounds();
		soundEngine->drop();
		soundEngine = nullptr;
	}
	if (currentSong)
	{
		currentSong->stop();
		currentSong->drop();
		currentSong = nullptr;
	}

	for (std::map<std::string, SoundInfo*>::iterator it = bgmMap.begin(); it != bgmMap.end(); ++it)
	{
		// If the value/second was not deleted elsewhere, then delete it here
		if (it->second != NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	bgmMap.clear();

	for (int i = 0; i < playlists.size(); i++)
	{
		for (int j = 0; j < playlists[i].size(); j++)
		{
			if (playlists[i][j])
			{
				delete playlists[i][j];
			}
		}
		playlists[i].clear();
	}
	
	for (int i = 0; i < songQueue.size(); i++)
	{
		if (songQueue[i])
		{
			delete songQueue[i];
			songQueue[i] = nullptr;
		}
	}

	playlists.clear();
	playlistMap.clear();
}

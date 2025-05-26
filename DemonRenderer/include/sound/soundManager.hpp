#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <SDL.h>
#include <SDL_mixer.h>
#include <core/randomiser.hpp>

class SoundManager
{
public:
	SoundManager();
	~SoundManager() {
		Mix_CloseAudio();
		SDL_Quit();
	};
	int playSound(Mix_Chunk* sound, int volume);
	int playRandomFromLibrary(int type);
	//int playSound_Safe(int channel, Mix_Chunk* sound, int volume);
	int playSoundAtPosition(Mix_Chunk* sound, int volume, glm::vec3 camForward, glm::vec3 offset, int channel = -1, int loops = 0);
	//int playSoundAtPosition_Safe(int channel, Mix_Chunk* sound, int volume, glm::vec3 camForward, glm::vec3 offset);

	void stopSound(int channel);

	int addNewSoundsToLibrary(std::string SoundName, int variants);

	std::vector<std::pair<std::shared_ptr<Mix_Chunk>,int>> m_musicLibrary;

	int MusicChannel;

	std::vector<std::vector<std::shared_ptr<Mix_Chunk>>> m_soundLibrary;
};

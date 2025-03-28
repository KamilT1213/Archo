#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <SDL.h>
#include <SDL_mixer.h>

class SoundManager
{
public:
	SoundManager();
	~SoundManager() {
		Mix_CloseAudio();
		SDL_Quit();
	};
	int playSound(Mix_Chunk* sound, int volume);
	//int playSound_Safe(int channel, Mix_Chunk* sound, int volume);
	int playSoundAtPosition(Mix_Chunk* sound, int volume, glm::vec3 camForward, glm::vec3 offset);
	//int playSoundAtPosition_Safe(int channel, Mix_Chunk* sound, int volume, glm::vec3 camForward, glm::vec3 offset);
};

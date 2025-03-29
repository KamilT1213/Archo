#include "sound/soundManager.hpp"
#include "core/log.hpp"
SoundManager::SoundManager()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cerr << "SDL Initialization Error: " << SDL_GetError() << "\n";
		return;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << "SDL_mixer Initialization Error: " << Mix_GetError() << "\n";
		SDL_Quit();
		return;
	}
}

int SoundManager::playSound(Mix_Chunk* sound, int volume)
{
	return Mix_PlayChannel(-1, sound, volume);
}

//int SoundManager::playSound_Safe(int channel, Mix_Chunk* sound, int volume)
//{
//	if (Mix_Playing(channel) == 0) {
//
//	}
//	return 0;
//}

int SoundManager::playSoundAtPosition(Mix_Chunk* sound, int volume, glm::vec3 camForward, glm::vec3 offset)
{
	glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), glm::normalize(glm::vec3(camForward.x, 0, camForward.z)));

	Sint16 angle = glm::round(glm::degrees(glm::dot(right,glm::normalize(glm::vec3(offset.x , 0 , offset.z)))));
	int dist = glm::distance(glm::vec3(0),offset);
	Uint8 distance = dist;

	//spdlog::info("Angle: {} | Distance: {}", angle, dist);
	
	if (dist <= 255) {
		int channel = Mix_PlayChannel(-1, sound, 1);
		Mix_SetPosition(channel, angle, distance);
		return channel;
	}
	
	return -1;

}

//int SoundManager::playSoundAtPosition_Safe(int channel, Mix_Chunk* sound, int volume, glm::vec3 camForward, glm::vec3 offset)
//{
//	return 0;
//}


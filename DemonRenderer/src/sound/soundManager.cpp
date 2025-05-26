#include "sound/soundManager.hpp"
#include "core/log.hpp"
#include "sound/soundManager.hpp"
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
	//(*sound).abuf
}

int SoundManager::playRandomFromLibrary(int type)
{
	if(m_soundLibrary.size() > type){
		int randomSound = Randomiser::uniformIntBetween(0,m_soundLibrary[type].size());
		//spdlog::info("Sound of type: {}, Playing variant: {}",type,randomSound);
		return Mix_PlayChannel(-1, m_soundLibrary[type][randomSound].get(), 0);
	}
    return 0;
}

//int SoundManager::playSound_Safe(int channel, Mix_Chunk* sound, int volume)
//{
//	if (Mix_Playing(channel) == 0) {
//
//	}
//	return 0;
//}

int SoundManager::playSoundAtPosition(Mix_Chunk* sound, int volume, glm::vec3 camForward, glm::vec3 offset, int channel, int loops)
{
	glm::vec3 right = glm::cross(glm::vec3(0, 1, 0), glm::normalize(glm::vec3(camForward.x, 0, camForward.z)));

	Sint16 angle = glm::round(glm::degrees(glm::dot(right,glm::normalize(glm::vec3(offset.x , 0 , offset.z)))));
	int dist = glm::distance(glm::vec3(0),offset);
	Uint8 distance = dist;

	//spdlog::info("Angle: {} | Distance: {}", angle, dist);
	
	if (dist <= 255 || true) {
		if(!Mix_Playing(channel) || channel < 0){
			channel = Mix_PlayChannel(-1, sound, loops);
		}

		Mix_SetPosition(channel, angle, distance);
		//spdlog::info("channel :{}",channel);
		return channel;
	}
	
	return -1;

}

void SoundManager::stopSound(int channel)
{
	Mix_FadeOutChannel(channel,50);
}

int SoundManager::addNewSoundsToLibrary(std::string SoundName, int variants) 
{
	std::vector<std::shared_ptr<Mix_Chunk>> sounds;
	for(int i = 0; i < variants; i++){
		std::string soundFile =	"./assets/sounds/" + SoundName + "_var" + std::to_string(i) + ".wav";
		std::shared_ptr<Mix_Chunk> sound = (std::shared_ptr<Mix_Chunk>)Mix_LoadWAV(soundFile.c_str());
		//spdlog::info(soundFile);
		if (!sound) {
			std::cerr << "Failed to load sound: " << Mix_GetError() << "\n";
			break;
		}
		sounds.push_back(sound);
	}

	if(sounds.size() > 0){
		m_soundLibrary.push_back(sounds);
		return m_soundLibrary.size() - 1;
	}
	return -1;

} // int SoundManager::playSoundAtPosition_Safe(int channel, Mix_Chunk* sound, int volume, glm::vec3 camForward, glm::vec3 offset)
//{
//	return 0;
//}


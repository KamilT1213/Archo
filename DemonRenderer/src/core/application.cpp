/** \file application.cpp */

#include "core/application.hpp"
#include "tracy/tracy.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <SDL.h>
#include <SDL_mixer.h>


void playSoundWithVolumeControl(const char* filePath, int initialVolume = MIX_MAX_VOLUME / 2) {
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

	// Load sound
	Mix_Chunk* sound = Mix_LoadWAV(filePath);
	if (!sound) {
		std::cerr << "Failed to load sound: " << Mix_GetError() << "\n";
		Mix_CloseAudio();
		SDL_Quit();
		return;
	}

	// Set initial volume
	Mix_VolumeChunk(sound, initialVolume);

	// Play the sound
	int channel = Mix_PlayChannel(-1, sound, -1);  // -1 selects first available channel
	
	if (channel == -1) {
		std::cerr << "Error playing sound: " << Mix_GetError() << "\n";
	}
	else {
		std::cout << "Playing sound. Use '+' to increase and '-' to decrease volume. Press 'q' to quit.\n";

		// Volume control loop
		char input;
		bool run = true;
		while (run) {
			std::cin >> input;

			//Mix_SetPosition()
			if (input == '+') {
				int newVolume = Mix_Volume(channel, -1) + 10;
				Mix_Volume(channel, std::min(newVolume, MIX_MAX_VOLUME));
			}
			else if (input == '-') {
				int newVolume = Mix_Volume(channel, -1) - 10;
				Mix_Volume(channel, std::max(newVolume, 0));
			}
			else if (input == 'l') {
				Mix_SetPanning(channel, 255, 0);
			}
			else if (input == 'r') {
				Mix_SetPanning(channel, 0, 255);
			}
			else if (input == 'q') {
				run = false;
				Mix_HaltChannel(channel);
			}
		}
	}

	// Clean up
	Mix_FreeChunk(sound);
	Mix_CloseAudio();
	SDL_Quit();
}

Application::Application(const WindowProperties& winProps)
{
	m_window.open(winProps);

	m_window.handler.onWinClose = [this](WindowCloseEvent& e) {onClose(e);};
	m_window.handler.onKeyPress = [this](KeyPressedEvent& e) {onKeyPressed(e);};
	m_window.handler.onKeyRelease = [this](KeyReleasedEvent& e) {onKeyReleased(e);};

	auto setupTime = m_timer.reset();
}

void Application::run() 
{
	//const char* soundFile = "./assets/sounds/Extraction_soft_var0.wav";  // Replace with your file path
	//playSoundWithVolumeControl(soundFile);

    spdlog::debug("Application running");

	glEnable(GL_DEPTH_TEST);

	while (m_running) {		
		auto timestep = m_timer.reset();

		onUpdate(timestep);
		if(m_window.isHostingImGui()) onImGuiRender();
		onRender();

		m_window.onUpdate(timestep);
	}
}

void Application::onUpdate(float timestep)
{
	if (m_layer) m_layer->onUpdate(timestep);
}

void Application::onRender() const
{
	if (m_layer) m_layer->onRender();
}

void Application::onImGuiRender()
{
    if (m_layer) m_layer->onImGUIRender();
}

void Application::onClose(WindowCloseEvent& e)
{
	m_running = false;
	if (m_layer) m_layer->onClose(e);
	e.handle();
}

void Application::onResize(WindowResizeEvent& e)
{
	if (m_layer) m_layer->onResize(e);
}

void Application::onKeyPressed(KeyPressedEvent& e)
{
	if (e.getKeyCode() == GLFW_KEY_ESCAPE) {
		spdlog::info("Exiting");
		m_running = false;
		e.handle();
		return;
	}

	else{
		if (e.getKeyCode() == GLFW_KEY_I) m_ImGuiOpen = true;
		if (m_layer) m_layer->onKeyPressed(e);
		e.handle();
	}

}

void Application::onKeyReleased(KeyReleasedEvent& e)
{
	if (m_layer) m_layer->onKeyReleased(e);
}

void Application::onMousePress(MouseButtonPressedEvent& e)
{
	if (m_layer) m_layer->onMousePress(e);
}

void Application::onMouseReleased(MouseButtonReleasedEvent& e)
{
	if (m_layer) m_layer->onMouseReleased(e);
}

void Application::onMouseMoved(MouseMovedEvent& e)
{
	if (m_layer) m_layer->onMouseMoved(e);
}

void Application::onMouseScrolled(MouseScrolledEvent& e)
{
	if (m_layer) m_layer->onMouseScrolled(e);
}

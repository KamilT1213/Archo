#include "app.hpp"
#include "MainScene.hpp"


App::App(const WindowProperties& winProps) : Application(winProps)
{
	m_window.m_ImGuiOpen = false;
	m_layer = std::unique_ptr<Layer>(new Archo(m_window));
}

Application* startApplication()
{
	WindowProperties props("Archo Digger", 1422, 800);
	props.isHostingImGui = false;
	props.isResizable = true;
	props.isFullScreen = true;
	return new App(props);
}
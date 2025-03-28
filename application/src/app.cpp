#include "app.hpp"
#include "GAMR3531.hpp"
#include "LOD.hpp"


App::App(const WindowProperties& winProps) : Application(winProps)
{
	m_layer = std::unique_ptr<Layer>(new Archo(m_window));
}

Application* startApplication()
{
	WindowProperties props("Asteroid Belt", 1422, 800);
	props.isHostingImGui = true;
	props.isResizable = false;
	return new App(props);
}
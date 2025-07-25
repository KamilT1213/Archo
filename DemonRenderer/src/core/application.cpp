﻿/** \file application.cpp */

#include "core/application.hpp"
//#include "tracy/tracy.hpp"

Application::Application(const WindowProperties& winProps)
{
	m_window.open(winProps);

	m_window.handler.onWinClose = [this](WindowCloseEvent& e) {onClose(e);};
	m_window.handler.onWinFocused = [this](WindowFocusEvent& e) {onFocus(e);};
	m_window.handler.onWinLostFocus = [this](WindowLostFocusEvent& e) {onLostFocus(e);};
	m_window.handler.onWinResized = [this](WindowResizeEvent& e) {onResize(e);};
	m_window.handler.onKeyPress = [this](KeyPressedEvent& e) {onKeyPressed(e);};
	m_window.handler.onKeyRelease = [this](KeyReleasedEvent& e) {onKeyReleased(e);};

	auto setupTime = m_timer.reset();
}

void Application::run() 
{

    spdlog::debug("Application running");

	glEnable(GL_DEPTH_TEST);

	while (m_running) {		
		auto timestep = m_timer.reset();

		onUpdate(timestep);
		if(m_window.isHostingImGui()) onImGuiRender();
		onRender();

		m_window.onUpdate(timestep);

		if (m_layer->state == GameState::Exit) {
			m_running = false;
		}
	}
}


void Application::onUpdate(float timestep)
{
	if (m_layer) m_layer->onUpdate(timestep);
	if (m_layer->state == GameState::Reset) {
		m_layer->onReset(m_window);
	}
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

void Application::onFocus(WindowFocusEvent& e) {
	m_layer->onFocus(e);
	e.handle();
}

void Application::onLostFocus(WindowLostFocusEvent& e) {
	m_layer->onLostFocus(e);
	e.handle();
}

void Application::onResize(WindowResizeEvent& e)
{
	if (m_layer) m_layer->onResize(e);
}

void Application::onKeyPressed(KeyPressedEvent& e)
{
	if (m_layer) m_layer->onKeyPressed(e);
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

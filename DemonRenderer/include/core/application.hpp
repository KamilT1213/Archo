/** \file application.hpp */

#pragma once
#include <glad/gl.h>

#include "core/log.hpp"
#include "core/timer.hpp"
#include "core/layer.hpp"
#include "windows/GLFWSystem.hpp"
#include "windows/GLFWWindowImpl.hpp"

#include "buffers/VAO.hpp"
#include "buffers/UBOmanager.hpp"
#include "buffers/FBO.hpp"
#include "rendering/material.hpp"
#include "rendering/camera.hpp"
#include "rendering/renderPass.hpp"
#include "rendering/renderer.hpp"

//class Arena;
/** \class Application
 *  \brief Provides an application with a window, OpenGL context, logger (spdlog) and a timer.
 */
class Application
{
public:
	explicit Application(const WindowProperties& winProps); //!< User defined constructor
	void run();	//!< Run the application
	glm::ivec2 getWindowSize() { return m_window.getSize(); }
	//inline static std::shared_ptr<Arena> memArena{ nullptr };

protected:
	GLFWWindowImpl m_window; //!< GLFW Window
private:
	void onUpdate(float timestep); //!< Update everything
	void onRender() const; //!< Do all drawing
	void onImGuiRender(); //!< Draw all ImGui wigdets
	void onClose(WindowCloseEvent& e); //!< Run when the window is closed
	void onFocus(WindowFocusEvent& e);
	void onLostFocus(WindowLostFocusEvent& e);
	void onResize(WindowResizeEvent& e); //!< Run when the window is resized
	void onKeyPressed(KeyPressedEvent& e); //!< Run when a key is press and the window is focused
	void onKeyReleased(KeyReleasedEvent& e); //!< Run when a key is released and the window is focused
	void onMousePress(MouseButtonPressedEvent& e); //!< Run when a mouse button is pressed
	void onMouseReleased(MouseButtonReleasedEvent& e); //!< Run when a Mouse Button is released
	void onMouseMoved(MouseMovedEvent& e); //!< Run when the mouse is moved
	void onMouseScrolled(MouseScrolledEvent& e); //!<< Run when the mouse wheel is scrolled

	Timer m_timer;	//!< Basic std::chrono timer with seconds or milliseconds
protected:
	std::unique_ptr<Layer> m_layer; //!< Application layer
private:
	LogSystem m_logSystem;	//!< System which initialises the logger
	GLFWSystem m_windowsSystem; //!< System which initialises and terminated GLFW
	bool m_running{ true }; //!< Controls whether or not the application is running
	bool m_ImGuiOpen{ true }; //!< Boolean for IMGui window
public:
	inline static bool MainLoop{ false };
};

// To be defined in users code
Application* startApplication(); //!< Function definition which provides an entry hook

//void* operator new (std::size_t count);
//
//
//void operator delete (void* ptr) noexcept;


// To be defined in users code
//Application* startApplication(); //!< Function definition which provides an entry hook

//struct block {
//	void* ptr;
//	size_t length;
//};
//
//
////Blk malloc(size_t size);
////void free(Blk block)
//
//class Arena
//{
//
//public:
//	Arena();
//	void* get(size_t count);
//	bool release(void* ptr);
//	void clear();
//
//private:
//	static const size_t sections = 19;
//	std::unique_ptr<void, decltype([](void* ptr) {free(ptr); }) > m_base{ nullptr };
//	//std::unique_ptr<void> m_base;
//	std::array<void*, sections> m_basePtrs = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
//	std::array<void*, sections> m_currentPtrs = { nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
//
//	const std::array<size_t, sections> m_sizes = { 16,32,64,128, 256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304 };
//	const std::array<size_t, sections> m_counts = { 128,64,128,128,256,128, 256,128,32,8,16,8,8,8,8,8,8,8,8 };
//	//std::array<size_t, sections> m_used = { 0,0,0,0,0,0,0,0,0,0,0,0 };
//	std::array<void*, sections> m_availability;
//	size_t n_allocCount{ 0 };
//	size_t n_size{ 0 };
//};
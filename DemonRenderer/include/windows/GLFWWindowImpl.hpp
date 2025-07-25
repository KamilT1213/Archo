#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "windows/window.hpp"
#include "windows/GLFW_GL_GC.hpp"


using GLFWWinDeleter = decltype(
	[](GLFWwindow* window) {
		// Probably temp to be moved
		if (false) {
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
		// End temp
		glfwDestroyWindow(window);
	}
	);

/** \class GLFWWindowImpl
 * \brief A GLFW window implementation.
 */
class GLFWWindowImpl : public IWindow<GLFWwindow, GLFWWinDeleter, ModernGLFW_GL_GC>
{
public:
	GLFWWindowImpl() = default; //!< Default constructor. Does not create a window but remains in a valid state.
	void doOpen(const WindowProperties& properties) override; //!< Open a window with properties
	void doOnUpdate(float timestep) override; //!< Update the window
	void doSetVSync(bool VSync) override; //!< Set the VSync
	[[nodiscard]] virtual bool doIsKeyPressed(int32_t keyCode) const override; //!< Function acts on is key pressed
	[[nodiscard]] virtual bool doIsMouseButtonPressed(int32_t mouseButton) const override; //!< Function acts on is mouse button pressed
	[[nodiscard]] virtual glm::vec2 doGetMousePosition() const override; //!< Function acts on get mouse position
	[[nodiscard]] virtual glm::vec2 doGetMouseVector() const;
	[[nodiscard]] virtual glm::ivec2 doGetSize() const override; //!< Virtual function acts on get the window size
	[[nodiscard]] virtual void doSwitchInput();	
	[[nodiscard]] virtual void doSwitchInputTo(bool b);
	[[nodiscard]] virtual int isFocus();

	bool m_ImGuiOpen{ true }; //!< Boolean for IMGui window
	bool w_mouseEnabled{ false };
	bool m_Resizing{ false };
	bool m_isFullScreen{ true };
private:
	
};


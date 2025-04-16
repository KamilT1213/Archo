#pragma once
//#include "windows/GLFWWindowImpl.hpp"
#include "DemonRenderer.hpp"

//#include "core/log.hpp"
//#include "components/script.hpp"
//#include "components/transform.hpp"
//#include <entt/entt.hpp>

class ButtonScript : public Script
{
public:
	ButtonScript(entt::entity entity, std::shared_ptr<Scene> scene, GLFWWindowImpl& win, glm::vec2& mousePos, float inscale, Transform& trans, Material& mat,const std::function<void()>& func) :
		Script(entity, scene),
		m_winRef(win),
		m_mousePosRef(mousePos),
		initialScreenHeight(inscale),
		m_active(false),
		m_hovered(false),
		m_transRef(trans),
		m_matRef(mat),
		m_assignedFunc(func)
	{

	}
	void onUpdate(float timestep) override;
	void onKeyPress(KeyPressedEvent& e) override {};
	void onKeyRelease(KeyReleasedEvent& e) override {};
	void onImGuiRender() override;

private:
	GLFWWindowImpl& m_winRef;
	Transform& m_transRef;
	Material& m_matRef;
	glm::vec2& m_mousePosRef;
	std::function<void()> m_assignedFunc;
	float initialScreenHeight;
	bool m_active{ false };
	bool m_hovered{ false };
};

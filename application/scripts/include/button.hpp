#pragma once
#include "windows/GLFWWindowImpl.hpp"
#include <glm/glm.hpp>

#include "core/log.hpp"
#include "components/script.hpp"
#include "components/transform.hpp"
#include <entt/entt.hpp>

class ButtonScript : public Script
{
public:
	ButtonScript(entt::entity entity, std::shared_ptr<Scene> scene, GLFWWindowImpl& win, Transform& trans) :
		Script(entity, scene),
		m_winRef(win),
		m_active(false),
		m_transRef(trans)
	{

	}
	void onUpdate(float timestep) override;
	void onKeyPress(KeyPressedEvent& e) override {};
	void onKeyRelease(KeyReleasedEvent& e) override {};
	void onImGuiRender() override;

private:
	GLFWWindowImpl& m_winRef;
	Transform& m_transRef;
	bool m_active{ false };
};

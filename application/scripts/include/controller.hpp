#pragma once
#include "windows/GLFWWindowImpl.hpp"
#include <glm/glm.hpp>

#include "core/log.hpp"
#include "components/script.hpp"
#include <entt/entt.hpp>

class ControllerScript : public Script
{
public:
	ControllerScript(entt::entity entity, std::shared_ptr<Scene> scene, GLFWWindowImpl& win, entt::entity& followCamera, const glm::vec3& movementSpeed, const glm::vec3& offset, float* speed) :
		Script(entity, scene),
		m_winRef(win),
		m_followCamera(followCamera),
		m_movementSpeed(movementSpeed),
		m_offset(offset),
		m_speedOut(speed)
	{
		*m_speedOut = m_movementSpeed.z;
		onUpdate(0.f); // Initialise camera
		//TestMemory();
	}
	void onUpdate(float timestep) override;
	void onKeyPress(KeyPressedEvent& e) override {};
	void onKeyRelease(KeyReleasedEvent& e) override {};
	void onImGuiRender() override;

	void TestMemory() {
		spdlog::info("ControllerScript\t\t\tsizeof {}\talignof {}", sizeof(ControllerScript), alignof(ControllerScript));
		spdlog::info("m_registry\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(entt::registry*), alignof(entt::registry&), offsetof(ControllerScript, Script::m_registry));
		spdlog::info("m_entity\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(ControllerScript::m_entity), alignof(entt::entity), offsetof(ControllerScript, Script::m_entity));
		spdlog::info("m_movementSpeed\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(ControllerScript::m_movementSpeed), alignof(glm::vec3), offsetof(ControllerScript, m_movementSpeed));
		spdlog::info("m_offset\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(ControllerScript::m_offset), alignof(glm::vec3), offsetof(ControllerScript, m_offset));
		spdlog::info("m_winRef\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(GLFWWindowImpl*), alignof(GLFWWindowImpl&), offsetof(ControllerScript, m_winRef));
		spdlog::info("m_followCamera\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(ControllerScript::m_followCamera), alignof(entt::entity&), offsetof(ControllerScript, m_followCamera));
		spdlog::info("m_speedOut\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(ControllerScript::m_speedOut), alignof(float*), offsetof(ControllerScript, m_speedOut));
		spdlog::info("m_minSpeed\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(ControllerScript::m_minSpeed), alignof(const float), offsetof(ControllerScript, m_minSpeed));
		spdlog::info("m_maxSpeed\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(ControllerScript::m_maxSpeed), alignof(const float), offsetof(ControllerScript, m_maxSpeed));
	};

private:
	glm::vec3 m_movementSpeed{ 0.f, 0.f, 0.f };
	glm::vec3 m_offset{ 0.f, 0.f, 0.f };
	GLFWWindowImpl& m_winRef;
	entt::entity& m_followCamera;
	float* m_speedOut{ nullptr };
	const float m_minSpeed{ -0.5f };
	const float m_maxSpeed{ -5.5f };
};

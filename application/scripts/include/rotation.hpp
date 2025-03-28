#pragma once

#include "DemonRenderer.hpp"
#include <entt/entt.hpp>
#include "core/log.hpp"
class RotationScript : public Script
{
public:
	RotationScript(entt::entity entity, std::shared_ptr<Scene> scene, const glm::vec3& rotSpeed, uint32_t pauseKeyMapping) :
		Script(entity, scene),
		m_rotSpeed(rotSpeed),
		m_pauseKey(pauseKeyMapping)
	{}
	virtual void onUpdate(float timestep) override;
	virtual void onKeyPress(KeyPressedEvent& e) override;
	virtual void onKeyRelease(KeyReleasedEvent& e) override {};
	void onImGuiRender();

	void TestMemory() {

		spdlog::info("RotationScript\t\t\tsizeof {}\talignof {}", sizeof(RotationScript), alignof(RotationScript));
		spdlog::info("m_registry\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(RotationScript::m_registry), alignof(entt::registry&), offsetof(RotationScript, m_registry));
		spdlog::info("m_entity\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(RotationScript::m_entity), alignof(entt::entity), offsetof(RotationScript, m_entity));
		spdlog::info("m_rotSpeed\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(RotationScript::m_rotSpeed), alignof(glm::vec3), offsetof(RotationScript, m_rotSpeed));
		spdlog::info("eulerAngles\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(RotationScript::eulerAngles), alignof(glm::vec3), offsetof(RotationScript, eulerAngles));
		spdlog::info("m_pauseKey\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(RotationScript::m_pauseKey), alignof(uint32_t), offsetof(RotationScript, m_pauseKey));
		spdlog::info("m_paused\t\t\tsizeof {}\talignof {}\toffsetof {}", sizeof(RotationScript::m_paused), alignof(bool), offsetof(RotationScript, m_paused));

	};

private:
	glm::vec3 m_rotSpeed{0.f, 0.f, 0.f };
	glm::vec3 eulerAngles{ 1.f,2.f,3.f };
	uint32_t m_pauseKey{ GLFW_KEY_P };
	bool m_paused{ false };
};



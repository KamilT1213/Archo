#include "scripts/include/rotation.hpp"


void RotationScript::onUpdate(float timestep)
{
	ZoneScoped;
	if (!m_paused) {
		auto& transformComp = m_registry.get<Transform>(m_entity);
		transformComp.rotation *= glm::quat(m_rotSpeed * timestep);
		transformComp.recalc();
	}
}

void RotationScript::onKeyPress(KeyPressedEvent& e)
{
	if (e.getKeyCode() == m_pauseKey) m_paused = !m_paused;
}

void RotationScript::onImGuiRender()
{
	if (ImGui::DragFloat3("Rotation", &eulerAngles[0], 1.0f, 0.f, 360.f))
	{
		auto& transformComp = m_registry.get<Transform>(m_entity);
		transformComp.rotation = glm::quat(glm::radians(eulerAngles));
		transformComp.recalc();
	}
}
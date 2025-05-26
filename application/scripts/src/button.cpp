#include "scripts/include/button.hpp"
//#include "core/log.hpp"
#include "MainScene.hpp"

void ButtonScript::onUpdate(float timestep)
{
	glm::vec2 mousePos = m_mousePosRef;
	mousePos.y = initialScreenHeight - mousePos.y;
	std::pair<glm::vec2, glm::vec2> boundingBox;
	boundingBox.first = glm::vec2(m_transRef.translation - glm::abs(m_transRef.scale));
	boundingBox.second = glm::vec2(m_transRef.translation + glm::abs(m_transRef.scale));


	//auto& matRef = m_scene->m_entities.get<Material>(m_entity);

	if (mousePos.x > boundingBox.first.x && mousePos.x < boundingBox.second.x && mousePos.y > boundingBox.first.y && mousePos.y < boundingBox.second.y) {
		m_hovered = true;
		m_matRef.setValue("Hovered", 1.0f);
		//spdlog::info("Button Active at; x:{} | y:{}", mousePos.x, mousePos.y);
	}
	else {
		m_matRef.setValue("Hovered", 0.0f);
		m_hovered = false;
	}

	if (m_hovered && m_winRef.doIsMouseButtonPressed(0)) {
		m_active = true;
		m_matRef.setValue("Pressed", 1.0f);
	}
	else {
		//m_active = false;
		m_matRef.setValue("Pressed", 0.0f);
	}

	if (m_active && m_hovered && !m_winRef.doIsMouseButtonPressed(0)) {

		//spdlog::info("Released");

		//_sleep(1000);
		m_soundManager.playSound(m_sound.get(),0);
		m_assignedFunc();
		m_active = false;
	}

	if (!m_hovered) {
		m_active = false;
	}
}

void ButtonScript::onImGuiRender()
{

}

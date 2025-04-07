#include "scripts/include/button.hpp"
#include "core/log.hpp"
#include "components/transform.hpp"

void ButtonScript::onUpdate(float timestep)
{
	glm::vec2 screenNorm = (glm::vec2(m_winRef.doGetSize()) / 2.0f);
	glm::vec2 mousePos = m_winRef.doGetMousePosition() - screenNorm;
	std::pair<glm::vec2, glm::vec2> boundingBox;
	boundingBox.first = glm::vec2(m_transRef.translation - m_transRef.scale) * screenNorm;
	boundingBox.second = glm::vec2(m_transRef.translation + m_transRef.scale) * screenNorm;

	if (mousePos.x > boundingBox.first.x && mousePos.x < boundingBox.second.x && mousePos.y > boundingBox.first.y && mousePos.y < boundingBox.second.y) {
		m_active = true;
		spdlog::info("Button Active at; x:{} | y:{}", mousePos.x, mousePos.y);
	}
	else {
		m_active = false;
	}
}

void ButtonScript::onImGuiRender()
{

}

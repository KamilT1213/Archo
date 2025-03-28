#include "scripts/include/controller.hpp"
#include "core/log.hpp"
#include "components/transform.hpp"

void ControllerScript::onUpdate(float timestep)
{
	auto& transformComp = m_registry.get<Transform>(m_entity);

	glm::vec3 forward = { -transformComp.transform[2][0], -transformComp.transform[2][1], -transformComp.transform[2][2] };

	transformComp.translation += forward * m_movementSpeed.z * timestep;

	if (m_winRef.isKeyPressed(GLFW_KEY_W)) {
		float theta = m_movementSpeed.x * timestep;
		glm::quat delta(glm::vec3(theta, 0.f, 0.f));
		transformComp.rotation *= delta;
	}
	if (m_winRef.isKeyPressed(GLFW_KEY_S)) {
		float theta = m_movementSpeed.x * -timestep;
		glm::quat delta(glm::vec3(theta, 0.f, 0.f));
		transformComp.rotation *= delta;
	}

	if (m_winRef.isKeyPressed(GLFW_KEY_D)) {
		float theta = m_movementSpeed.y * -timestep;
		glm::quat delta(glm::vec3(0.f, theta, 0.f));
		transformComp.rotation *= delta;
	}
	if (m_winRef.isKeyPressed(GLFW_KEY_A)) {
		float theta = m_movementSpeed.y * timestep;
		glm::quat delta(glm::vec3(0.f, theta, 0.f));
		transformComp.rotation *= delta;
	}

	transformComp.recalc();
	
	if (m_winRef.isKeyPressed(GLFW_KEY_UP)) {
		m_movementSpeed.z = std::clamp(m_movementSpeed.z - timestep, m_maxSpeed, m_minSpeed);
		*m_speedOut = m_movementSpeed.z;
	}

	if (m_winRef.isKeyPressed(GLFW_KEY_DOWN)) {
		m_movementSpeed.z = std::clamp(m_movementSpeed.z + timestep, m_maxSpeed, m_minSpeed);
		*m_speedOut = m_movementSpeed.z;
	}

	// Follow
	glm::vec3 targetRight = { transformComp.transform[0][0], transformComp.transform[0][1], transformComp.transform[0][2] };
	glm::vec3 targetUp = { transformComp.transform[1][0], transformComp.transform[1][1], transformComp.transform[1][2] };
	glm::vec3 targetForward = { -transformComp.transform[2][0], -transformComp.transform[2][1], -transformComp.transform[2][2] };

	auto& cameraTransformComp = m_registry.get<Transform>(m_followCamera);

	cameraTransformComp.translation = transformComp.translation;

	cameraTransformComp.translation += targetRight * m_offset.x;
	cameraTransformComp.translation += targetUp * m_offset.y;
	cameraTransformComp.translation += targetForward * m_offset.z;

	cameraTransformComp.rotation = transformComp.rotation * glm::quat(glm::vec3(0.f, glm::pi<float>(), 0.f)); // Hack as model is backward to start with

	cameraTransformComp.recalc();
}

void ControllerScript::onImGuiRender()
{
	ImGui::DragFloat3("Follow cam offset", &m_offset[0], 0.1f, 0.f, 10.f);
	ImGui::DragFloat("Speed", &m_movementSpeed[2], 0.1, -10.f, 10.f);
}

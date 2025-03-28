/** \file planeSweep.cpp */
#include "core/planeSweep.hpp"

const std::vector<entt::entity> PlaneSweep::getCloseEntities(const AABB& ship)
{
	std::set<entt::entity> closeEntities;

	float near = glm::dot(m_planeDirection, ship.first);
	float far = glm::dot(m_planeDirection, ship.second);
	float size = far - near;
	
	//spdlog::info("Ship Start: {}, End: {}", near, far);
	for (int i = 0; i < m_startPoints.size(); i++) {
		if (m_startPoints[i].first.first + m_startPoints[i].first.second >= near && m_startPoints[i].first.first <= far) {
			closeEntities.emplace(m_startPoints[i].second);
		}
	}	
	//for (int i = 0; i < m_endPoints.size(); i++) {
	//	if (m_endPoints[i].first >= near && m_endPoints[i].first <= far) {
	//		closeEntities.emplace(m_endPoints[i].second);
	//	}
	//}

	//spdlog::info("Number Of Objects In Slice: {}", closeEntities.size());

	return std::vector<entt::entity>(closeEntities.begin(),closeEntities.end());
}

void PlaneSweep::addEntity(entt::entity entity, const AABB& entityAABB)
{
	if (m_startPoints.size() == 0) {
		float alongAxisN = glm::dot(m_planeDirection, entityAABB.first);
		float alongAxisF = glm::dot(m_planeDirection, entityAABB.second);
		m_startPoints.push_back(std::pair<std::pair<float, float>, entt::entity>(std::pair<float,float>(alongAxisN, alongAxisF - alongAxisN), entity));
		//m_endPoints.push_back(std::pair<float, entt::entity>(glm::dot(m_planeDirection, entityAABB.second), entity));
		return;
	}

	for (int i = 0; i < m_startPoints.size(); i++) {
		float alongAxisN = glm::dot(m_planeDirection, entityAABB.first);
		float alongAxisF = glm::dot(m_planeDirection, entityAABB.second);
		if (m_startPoints[i].first.first - m_startPoints[i].first.first > alongAxisN) {
			m_startPoints.insert(m_startPoints.begin() + i, std::pair<std::pair<float, float>, entt::entity>(std::pair<float, float>(alongAxisN, alongAxisF - alongAxisN), entity));
			break;
		}
		if (i == m_startPoints.size() - 1) {
			m_startPoints.insert(m_startPoints.begin() + i, std::pair<std::pair<float, float>, entt::entity>(std::pair<float, float>(alongAxisN, alongAxisF - alongAxisN), entity));
			break;
		}

	}
	//for (int i = 0; i < m_endPoints.size(); i++) {
	//	float alongAxis = glm::dot(m_planeDirection, entityAABB.second);
	//	if (m_endPoints[i].first > alongAxis) {
	//		m_endPoints.insert(m_endPoints.begin() + i, std::pair<float, entt::entity>(alongAxis, entity));
	//		break;
	//	}
	//	if (i == m_endPoints.size() - 1) {
	//		m_endPoints.push_back(std::pair<float, entt::entity>(alongAxis, entity));
	//		break;
	//	}
	//}

	//spdlog::info("m_startPoints total: {}, m_endPoints total: {}", m_startPoints.size(), m_endPoints.size());
}

void PlaneSweep::eraseEntity(entt::entity entity)
{
	
	for (int i = 0; i < m_startPoints.size(); i++) {
		if (m_startPoints[i].second == entity) {
			m_startPoints.erase(m_startPoints.begin() + i);
			break;
		}
	}

	//for (int i = 0; i < m_endPoints.size(); i++) {
	//	if (m_endPoints[i].second == entity) {
	//		m_endPoints.erase(m_endPoints.begin() + i);
	//		break;
	//	}
	//}
}

AABB PlaneSweep::getAABBforPlane(OBBCollider Collider, glm::vec3 Position)
{
	glm::vec3 far = glm::sign(m_planeDirection);
	glm::vec3 farPoint = Position + (Collider.halfExtents * far);
	glm::vec3 closePoint = Position + (Collider.halfExtents * -far);

	return AABB(closePoint,farPoint);
}

AABB PlaneSweep::getAABBforPlane(SphereCollider Collider, glm::vec3 Position)
{
	glm::vec3 far = glm::sign(m_planeDirection);
	glm::vec3 scale = glm::vec3(Collider.radius * 1.5f);
	glm::vec3 farPoint = Position + (scale * far);
	glm::vec3 closePoint = Position + (scale * -far);

	return AABB(closePoint, farPoint);
}

#pragma once
#include <glm/glm.hpp>
#include <entt/entt.hpp>

/** \struct OBBCollider
*	\brief Properties of an oriented bounding box collider
*/
struct OBBCollider
{
	/** Constructor which takes half extents and entity */
	OBBCollider(const glm::vec3& he, entt::entity entt) : halfExtents(he), entity(entt) {}
	glm::vec3 halfExtents{ glm::vec3(0.f) }; //!< Half extents of the collider i.e. half height, width and depth
	entt::entity entity; //!< Entity which collider is attached to

	std::vector<glm::vec3> getLocalBoundingPoints() {
		std::vector<glm::vec3> Points;

		Points.push_back(glm::vec3(halfExtents.x, halfExtents.y, halfExtents.z));
		Points.push_back(glm::vec3(halfExtents.x, halfExtents.y, -halfExtents.z));
		Points.push_back(glm::vec3(halfExtents.x, -halfExtents.y, halfExtents.z));
		Points.push_back(glm::vec3(halfExtents.x, -halfExtents.y, -halfExtents.z));
		Points.push_back(glm::vec3(-halfExtents.x, halfExtents.y, halfExtents.z));
		Points.push_back(glm::vec3(-halfExtents.x, halfExtents.y, -halfExtents.z));
		Points.push_back(glm::vec3(-halfExtents.x, -halfExtents.y, halfExtents.z));
		Points.push_back(glm::vec3(-halfExtents.x, -halfExtents.y, -halfExtents.z));

		return Points;
	}

	glm::vec3 getPointClosestToFront(glm::vec3 VPDir, glm::vec3 CamPos, glm::vec3 ThisPos) {
		glm::vec3 Local = ThisPos - CamPos;
		float dist = glm::dot(Local, VPDir);
		Local = (dist * VPDir) - Local;
		Local = glm::vec3(glm::clamp(Local.x, -halfExtents.x, halfExtents.x), glm::clamp(Local.y, -halfExtents.y, halfExtents.y), glm::clamp(Local.z, -halfExtents.z, halfExtents.z));
		return Local + ThisPos;
	}

};

/** \struct SphereCollider
*	\brief Properties of an sphere collider
*/
struct SphereCollider
{
	/** Constructor which takes radii and entity */
	SphereCollider(float rad, entt::entity entt) : radius(rad), entity(entt) {}
	float radius{ 0.f }; //!< Radius of sphere
	entt::entity entity; //!< Entity which collider is attached to

	std::vector<glm::vec3> getLocalBoundingPoints() {
		std::vector<glm::vec3> Points;

		Points.push_back(glm::vec3(radius, radius, radius));
		Points.push_back(glm::vec3(radius, radius, -radius));
		Points.push_back(glm::vec3(radius, -radius, radius));
		Points.push_back(glm::vec3(radius, -radius, -radius));
		Points.push_back(glm::vec3(-radius, radius, radius));
		Points.push_back(glm::vec3(-radius, radius, -radius));
		Points.push_back(glm::vec3(-radius, -radius, radius));
		Points.push_back(glm::vec3(-radius, -radius, -radius));

		return Points;
	}

	glm::vec3 getPointClosestToFront(glm::vec3 VPDir, glm::vec3 CamPos, glm::vec3 ThisPos) {
		glm::vec3 Local = ThisPos - CamPos;
		float dist = glm::dot(Local, VPDir);
		Local = (dist * VPDir) - Local;
		Local = glm::normalize(Local) * radius * 1.5f;
		return Local + ThisPos;
	}

};
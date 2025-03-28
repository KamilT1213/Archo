/** \file physics.cpp */

#include "core/physics.hpp"
#include "components/transform.hpp"
#include "entt/entt.hpp"

namespace Physics
{
	/* Implementation is based Real - Time Collision Detection by Christer Ericson pp 133
	*  Calculate the closest point on or inside the OBB.
	*  Distance is calculate from that.
	*/
	float DistanceOBBToPoint(Scene* scene, const OBBCollider& obb, const glm::vec3& point)
	{

		auto& obbTransform = scene->m_entities.get<Transform>(obb.entity);

		// ClosestPoint is the closest point inside or on the interior of the OBB
		glm::vec3 closestPoint = obbTransform.translation; // Set closest point to actor position
		glm::vec3 centre = point - obbTransform.translation; // Move the problem so actor is at world centre

		// Get axis of OBB entity
		glm::vec3 OBBRight = { obbTransform.transform[0][0], obbTransform.transform[0][1], obbTransform.transform[0][2] };
		glm::vec3 OBBUp = { obbTransform.transform[1][0], obbTransform.transform[1][1], obbTransform.transform[1][2] };
		glm::vec3 OBBForward = { -obbTransform.transform[2][0], -obbTransform.transform[2][1], -obbTransform.transform[2][2] };

		// Project along each axis, moving closest point as we go

		// Project along X
		float distX = glm::dot(centre, OBBRight);
		// Clamp to OBB half extents
		distX = std::clamp(distX, -obb.halfExtents.x, obb.halfExtents.x);
		// Move closest point
		closestPoint += OBBRight * distX;

		// Project along Y
		float distY = glm::dot(centre, OBBUp);
		// Clamp to OBB half extents
		distY = std::clamp(distY, -obb.halfExtents.y, obb.halfExtents.y);
		// Move closest point
		closestPoint += OBBUp * distY;

		// Project along Z
		float distZ = glm::dot(centre, OBBForward);
		// Clamp to OBB half extents
		distZ = std::clamp(distZ, -obb.halfExtents.z, obb.halfExtents.z);
		// Move closest point
		closestPoint += OBBUp * distZ;

		// Find the distance squared
		float distanceSquared = glm::dot(closestPoint - point, closestPoint - point);

		return sqrtf(distanceSquared);

	}
	float DistanceOBBToSphere(Scene* scene, const OBBCollider& obb, const SphereCollider& sphere)
	{
		auto& sphereCentre = scene->m_entities.get<Transform>(sphere.entity).translation;
		return DistanceOBBToPoint(scene, obb, sphereCentre) - sphere.radius;
	}
}
/** \file Camera.hpp */
#pragma once

#include <glm/glm.hpp>
#include "core/log.hpp"

/** \struct Camera
*	\brief Basic camera with a view and a projection */
struct Camera
{
	glm::mat4 view{ glm::mat4(1.f) }; //!< View matrix
	glm::mat4 projection{ glm::mat4(1.f) }; //!Projection Matrix
	glm::vec3 position{ glm::vec3(0) };
	glm::vec3 forward{ glm::vec3(0,0,1) };

	void updateView(const glm::mat4& cameraTransform) //!< Update the view matrix based on the camera transform
	{
		position = glm::vec3(cameraTransform[3][0], cameraTransform[3][1], cameraTransform[3][2]);
		forward = glm::normalize(glm::vec3(-cameraTransform[2][0], -cameraTransform[2][1], -cameraTransform[2][2]));
		view = glm::inverse(cameraTransform);
	}
};

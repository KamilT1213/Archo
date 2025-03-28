/** \file scene.hpp */
#pragma once

#include "rendering/lights.hpp"
#include <entt/entt.hpp>


/** \struct Scene
*	\brief Holds everything which makes up a scene
*/
struct Scene
{
	entt::registry m_entities; //!< Actors
	std::vector<DirectionalLight> m_directionalLights; //!< Directional lights
	std::vector<PointLight> m_pointLights; //!< Point lights
	std::vector<SpotLight> m_spotLights; //!< Spot lights


};
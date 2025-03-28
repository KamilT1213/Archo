/** \class PlaneSweep
*	\brief A broad phase approach which looks at everything with a window, and interval along the negative z-axis
*/
#include <DemonRenderer.hpp>

using AABB = std::pair<glm::vec3, glm::vec3>;

class PlaneSweep
{
public:
	PlaneSweep() = default;
	PlaneSweep(glm::vec2 windowSize, glm::vec3 direction) : m_window(windowSize) , m_planeDirection(direction) {}; //!< Constructor which takes in window
	const std::vector<entt::entity> getCloseEntities(const AABB& ship); //!< Gets entities in the interval
	void addEntity(entt::entity entity, const AABB& entityAABB); //!< Add entity to the data structures
	void eraseEntity(entt::entity entity); //!< Erase from the data structures
	AABB getAABBforPlane(OBBCollider Collider, glm::vec3 Position);
	AABB getAABBforPlane(SphereCollider Collider, glm::vec3 Position);
private:
	std::vector<std::pair<std::pair<float,float>, entt::entity>> m_startPoints; //!< Points when entities start in the z-axis
	//std::vector<std::pair<float, entt::entity>> m_endPoints; //!< Points when entities end in the z-axis
	std::vector<entt::entity> m_closeEntities; //!< Vector of entities currently in the window
	//std::vector<std::pair<float, entt::entity>>::iterator m_startPosition{ m_startPoints.begin() }; //!< Position in start points
	//std::vector<std::pair<float, entt::entity>>::iterator m_endPosition{ m_endPoints.begin() }; //!< Position in end points
	glm::vec2 m_window{ glm::vec2(0.f) };
	glm::vec3 m_planeDirection{ glm::vec3(0) };
};
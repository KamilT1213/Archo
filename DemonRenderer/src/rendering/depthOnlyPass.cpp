#include "rendering/depthOnlyPass.hpp"
#include "components/render.hpp"

void DepthPass::parseScene()
{
	auto renderView = scene->m_entities.view<Render>(); // Get all entities with a render component

	for (auto entity : renderView) // For each entity with a render component
	{
		auto& renderComp = renderView.get<Render>(entity); // Get the render component by reference.

		if (renderComp.material) {
			for (auto& UBOlayout : renderComp.material->m_shader->m_UBOLayouts) {
				UBOmanager.addUBO(UBOlayout);
			}
		}
	}
}

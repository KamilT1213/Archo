#pragma once
#include <DemonRenderer.hpp>
//#include <entt/entt.hpp>

class LOD : public Layer
{
public:
	LOD(GLFWWindowImpl& win);
	void onRender() override;
	void onUpdate(float timestep) override;
	void onKeyPressed(KeyPressedEvent& e) override;
	void onImGUIRender() override;
private:
	std::shared_ptr<Scene> m_mainScene;
	Renderer m_mainRenderer;
	bool m_wireframe{ false };

	std::vector<uint32_t> m_indices;
	std::vector<float> m_vertices;



};
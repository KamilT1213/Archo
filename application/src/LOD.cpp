#include "LOD.hpp"
#include "scripts/include/rotation.hpp"
#include "scripts/include/camera.hpp"
#include "meshoptimizer.h"

LOD::LOD(GLFWWindowImpl& win) : Layer(win)
{
	m_mainScene.reset(new Scene);

	DirectionalLight dl;
	dl.direction = glm::normalize(glm::vec3(1.f, -2.5f, -3.5f));
	m_mainScene->m_directionalLights.push_back(dl);

	ShaderDescription pbrShaderDesc;
	pbrShaderDesc.type = ShaderType::rasterization;
	pbrShaderDesc.vertexSrcPath = "./assets/shaders/PBR/pbrVertex.glsl";
	pbrShaderDesc.fragmentSrcPath = "./assets/shaders/PBR/pbrFrag.glsl";

	std::shared_ptr<Shader> pbrShader;
	pbrShader = std::make_shared<Shader>(pbrShaderDesc);

	VBOLayout modelLayout = {
				{GL_FLOAT, 3},
				{GL_FLOAT, 3},
				{GL_FLOAT, 2},
				{GL_FLOAT, 3}
	};

	uint32_t attributeTypes = Model::VertexFlags::positions |
		Model::VertexFlags::normals |
		Model::VertexFlags::uvs |
		Model::VertexFlags::tangents;

	std::array<std::shared_ptr<VAO>, 5> asteroidVAOs;
	std::shared_ptr<Material> asteroidMaterials;

	//	Asteroid Model
	std::shared_ptr<Texture> asteroid_albedo = std::make_shared<Texture>("./assets/models/asteroid1/albedo.jpg");
	std::shared_ptr<Texture> asteroid_normal = std::make_shared<Texture>("./assets/models/asteroid1/normal.jpg");
	std::shared_ptr<Texture> asteroid_metal = std::make_shared<Texture>("./assets/models/asteroid1/metallic.jpg");
	std::shared_ptr<Texture> asteroid_rough = std::make_shared<Texture>("./assets/models/asteroid1/roughness.jpg");
	std::shared_ptr<Texture> asteroid_AO = std::make_shared<Texture>("./assets/models/asteroid1/AO.png");

	asteroidMaterials = std::make_shared<Material>(pbrShader);
	asteroidMaterials->setValue("albedoTexture", asteroid_albedo);
	asteroidMaterials->setValue("normalTexture", asteroid_normal);
	asteroidMaterials->setValue("roughTexture", asteroid_rough);
	asteroidMaterials->setValue("metalTexture", asteroid_metal);
	asteroidMaterials->setValue("aoTexture", asteroid_AO);

	Model asteroidModel("./assets/models/asteroid1/asteroid.obj", attributeTypes);

	//std::vector

	asteroidVAOs[0] = std::make_shared<VAO>(asteroidModel.m_meshes[0].indices);
	asteroidVAOs[0]->addVertexBuffer(asteroidModel.m_meshes[0].vertices, modelLayout);

	const size_t vertexComponents = (3 + 3 + 2 + 3);
	const size_t vertexStride = sizeof(float) * vertexComponents;
	const size_t vertexCountOnLoad = (asteroidModel.m_meshes[0].vertices.size()) / vertexComponents;
	const size_t indexCountOnLoad = (asteroidModel.m_meshes[0].indices.size());

	std::vector<uint32_t> m_indicesLOD1;
	std::vector<uint32_t> m_indicesLOD2;
	std::vector<uint32_t> m_indicesLOD3;


	{
		size_t size = std::max(asteroidModel.m_meshes[0].indices.size(), asteroidModel.m_meshes[0].vertices.size());
		std::vector<uint32_t> remap(size);

		const size_t vertexCount = meshopt_generateVertexRemap(
			remap.data(),
			asteroidModel.m_meshes[0].indices.data(),
			indexCountOnLoad,
			asteroidModel.m_meshes[0].vertices.data(),
			vertexCountOnLoad,
			vertexStride
		);

		std::vector<uint32_t> remappedIndices;
		remappedIndices.resize(asteroidModel.m_meshes[0].indices.size());
		std::vector<float> remappedVertices(vertexCount * vertexComponents);

		meshopt_remapIndexBuffer(
			remappedIndices.data(),
			asteroidModel.m_meshes[0].indices.data(),
			indexCountOnLoad,
			remap.data());


		meshopt_remapVertexBuffer(
			remappedVertices.data(),
			asteroidModel.m_meshes[0].vertices.data(),
			vertexCountOnLoad,
			vertexStride,
			remap.data()
		);

		meshopt_optimizeVertexCache(
			remappedIndices.data(),
			remappedIndices.data(),
			indexCountOnLoad,
			vertexCount
		);

		meshopt_optimizeOverdraw(
			remappedIndices.data(),
			remappedIndices.data(),
			indexCountOnLoad,
			remappedVertices.data(),
			vertexCount,
			vertexStride,
			1.05f); // Docs suggest 1.05 is generally a good threshold

		meshopt_optimizeVertexFetch(
			remappedVertices.data(),
			remappedIndices.data(),
			indexCountOnLoad,
			remappedVertices.data(),
			vertexCount,
			vertexStride
		);

		const float threshold1 = 0.5f;	// LOD1 threshold, the lower the number the fewer vertices
		const float threshold2 = 0.125f; // LOD2 threshold
		const float threshold3 = 0.03125f; // LOD2 threshold
		size_t LOD1_target_index_count = size_t(remappedIndices.size() * threshold1); // Ideal LOD1 index count
		size_t LOD2_target_index_count = size_t(remappedIndices.size() * threshold2); // Ideal LOD2 index count
		size_t LOD3_target_index_count = size_t(remappedIndices.size() * threshold3);
		const float LOD1_target_error = 0.01f; // Error allowed, the higher the number greater deviation allowed
		const float LOD2_target_error = 0.32f; // Error allowed, the higher the number greater deviation allowed
		const float LOD3_target_error = 40.96f;

		m_indicesLOD1.resize(remappedIndices.size());

		uint32_t LOD1IndexCount = meshopt_simplify(
			m_indicesLOD1.data(),
			remappedIndices.data(),
			remappedIndices.size(),
			remappedVertices.data(),
			vertexCount,
			vertexStride,
			LOD1_target_index_count,
			LOD1_target_error
		);

		m_indicesLOD2.resize(remappedIndices.size());

		uint32_t LOD2IndexCount = meshopt_simplify(
			m_indicesLOD2.data(),
			remappedIndices.data(),
			remappedIndices.size(),
			remappedVertices.data(),
			vertexCount,
			vertexStride,
			LOD2_target_index_count,
			LOD2_target_error
		);

		m_indicesLOD3.resize(remappedIndices.size());

		uint32_t LOD3IndexCount = meshopt_simplify(
			m_indicesLOD3.data(),
			remappedIndices.data(),
			remappedIndices.size(),
			remappedVertices.data(),
			vertexCount,
			vertexStride,
			LOD3_target_index_count,
			LOD3_target_error
		);

		m_indices = remappedIndices;
		m_vertices = remappedVertices;
		m_indicesLOD1.erase(m_indicesLOD1.begin() + LOD1IndexCount, m_indicesLOD1.end()); // Cut down to actual size
		m_indicesLOD2.erase(m_indicesLOD2.begin() + LOD2IndexCount, m_indicesLOD2.end()); // Cut down to actual size
		m_indicesLOD3.erase(m_indicesLOD3.begin() + LOD3IndexCount, m_indicesLOD3.end());

		m_indices = remappedIndices;
		m_vertices = remappedVertices;

		asteroidVAOs[1] = std::make_shared<VAO>(m_indices);
		asteroidVAOs[1]->addVertexBuffer(m_vertices, modelLayout);

		asteroidVAOs[2] = std::make_shared<VAO>(m_indicesLOD1);
		asteroidVAOs[2]->addVertexBuffer(m_vertices, modelLayout);

		asteroidVAOs[3] = std::make_shared<VAO>(m_indicesLOD2);
		asteroidVAOs[3]->addVertexBuffer(m_vertices, modelLayout);

		asteroidVAOs[4] = std::make_shared<VAO>(m_indicesLOD3);
		asteroidVAOs[4]->addVertexBuffer(m_vertices, modelLayout);

		spdlog::info("Verts on load: {} indices on load: {} ", vertexCountOnLoad, indexCountOnLoad);
		spdlog::info("Optimised verts: {} indices: {}", remappedVertices.size() / vertexComponents, remappedIndices.size());
	}

	{
		entt::entity optimised = m_mainScene->m_entities.create();

		auto& renderComp = m_mainScene->m_entities.emplace<Render>(optimised);
		renderComp.geometry = asteroidVAOs[2];
		renderComp.material = asteroidMaterials;

		auto& transformComp = m_mainScene->m_entities.emplace<Transform>(optimised);
		transformComp.translation = glm::vec3(2.f, 2.f, -6.f);
		transformComp.scale = glm::vec3(1.0);
		transformComp.recalc();

		auto& scriptComp = m_mainScene->m_entities.emplace<ScriptComp>(optimised);
		scriptComp.attachScript<RotationScript>(optimised, m_mainScene, glm::vec3(0.4f, 0.25f, -0.6f), GLFW_KEY_SPACE);
	}

	{
		entt::entity optimised = m_mainScene->m_entities.create();

		auto& renderComp = m_mainScene->m_entities.emplace<Render>(optimised);
		renderComp.geometry = asteroidVAOs[4];
		renderComp.material = asteroidMaterials;

		auto& transformComp = m_mainScene->m_entities.emplace<Transform>(optimised);
		transformComp.translation = glm::vec3(-2.f, -2.f, -6.f);
		transformComp.scale = glm::vec3(1.0);
		transformComp.recalc();

		auto& scriptComp = m_mainScene->m_entities.emplace<ScriptComp>(optimised);
		scriptComp.attachScript<RotationScript>(optimised, m_mainScene, glm::vec3(0.4f, 0.25f, -0.6f), GLFW_KEY_SPACE);
	}


	{
		entt::entity optimised = m_mainScene->m_entities.create();

		auto& renderComp = m_mainScene->m_entities.emplace<Render>(optimised);
		renderComp.geometry = asteroidVAOs[3];
		renderComp.material = asteroidMaterials;

		auto& transformComp = m_mainScene->m_entities.emplace<Transform>(optimised);
		transformComp.translation = glm::vec3(2.f, -2.f, -6.f);
		transformComp.scale = glm::vec3(1.0);
		transformComp.recalc();

		auto& scriptComp = m_mainScene->m_entities.emplace<ScriptComp>(optimised);
		scriptComp.attachScript<RotationScript>(optimised, m_mainScene, glm::vec3(0.4f, 0.25f, -0.6f), GLFW_KEY_SPACE);
	}

	{
		entt::entity raw = m_mainScene->m_entities.create();

		auto& renderComp = m_mainScene->m_entities.emplace<Render>(raw);
		renderComp.geometry = asteroidVAOs[0];
		renderComp.material = asteroidMaterials;

		auto& transformComp = m_mainScene->m_entities.emplace<Transform>(raw);
		transformComp.translation = glm::vec3(-2.f, 2.f, -6.f);
		transformComp.scale = glm::vec3(1.0);
		transformComp.recalc();

		auto& scriptComp = m_mainScene->m_entities.emplace<ScriptComp>(raw);
		scriptComp.attachScript<RotationScript>(raw, m_mainScene, glm::vec3(0.4f, 0.25f, -0.6f), GLFW_KEY_SPACE);
	}

	RenderPass mainPass;

	mainPass.scene = m_mainScene;
	mainPass.parseScene();
	mainPass.target = std::make_shared<FBO>();
	mainPass.camera.projection = glm::perspective(45.f, m_winRef.getWidthf() / m_winRef.getHeightf(), 0.1f, 2000.f);
	mainPass.viewPort = { 0, 0, m_winRef.getWidth(), m_winRef.getHeight() };

	mainPass.UBOmanager.setCachedValue("b_camera", "u_view", mainPass.camera.view);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_projection", mainPass.camera.projection);
	mainPass.UBOmanager.setCachedValue("b_camera", "u_viewPos", glm::vec3(0.f));

	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.colour", m_mainScene->m_directionalLights.at(0).colour);
	mainPass.UBOmanager.setCachedValue("b_lights", "dLight.direction", m_mainScene->m_directionalLights.at(0).direction);

	m_mainRenderer.addRenderPass(mainPass);

	glClearColor(1.f, 0.f, 1.f, 1.f);

}

void LOD::onRender()
{
	m_mainRenderer.render();
}

void LOD::onUpdate(float timestep)
{
	timestep = std::clamp(timestep, 0.f, 0.1f); // Clamp to be no more than a 10th of a second for physics
	// Update scripts
	auto view = m_mainScene->m_entities.view<ScriptComp>();
	for (auto entity : view)
	{
		auto& script = view.get<ScriptComp>(entity);
		script.onUpdate(timestep);
	}
}

void LOD::onImGUIRender()
{
	// Scripts
	auto view = m_mainScene->m_entities.view<ScriptComp>();
	for (auto entity : view)
	{
		auto& script = view.get<ScriptComp>(entity);
		script.onImGuiRender();
	}
}

void LOD::onKeyPressed(KeyPressedEvent& e)
{
	if (e.getKeyCode() == GLFW_KEY_W)
	{
		if (m_wireframe) {
			m_wireframe = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			m_wireframe = true;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
}
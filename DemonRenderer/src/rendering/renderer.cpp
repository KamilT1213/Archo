#pragma once

#include "rendering/renderer.hpp"
#include "tracy/Tracy.hpp"
#include "tracy/TracyOpenGL.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/collidable.hpp"


struct RenderHasher
{
	size_t operator()(const InstacedRender& rc) const
	{
		auto g = reinterpret_cast<std::uintptr_t>(rc.geometry.get());
		auto a = reinterpret_cast<std::uintptr_t>(rc.material.get());
		return g >> 1 & a >> 1;
	}
};

struct RenderEquals
{
	bool operator()(const InstacedRender& a, const InstacedRender& b) const
	{
		if (a.geometry.get() == b.geometry.get()) {
			return a.material.get() == b.material.get();
		}
		return false;
	}
};

void refreshLOD(Render& render, float distClipSpace) {
	//distClipSpace = glm::clamp(distClipSpace,0.f, 1.f);
	//if (render.enabledLOD) {
	//	int index = glm::floor(distClipSpace * (render.LODGeometry->size() - 1));
	//	render.geometry = (*render.LODGeometry)[index];
	//}
}

void Renderer::addRenderPass(const RenderPass& pass)
{
	m_renderOrder.push_back(std::pair<PassType, size_t>(PassType::render, m_renderPasses.size()));
	m_renderPasses.push_back(pass);
}

void Renderer::addDepthPass(const DepthPass& depthPass)
{
	m_renderOrder.push_back(std::pair<PassType, size_t>(PassType::depth, m_depthPasses.size()));
	m_depthPasses.push_back(depthPass);
}

void Renderer::addComputePass(const ComputePass& pass)
{
	m_renderOrder.push_back(std::pair<PassType, size_t>(PassType::compute, m_computePasses.size()));
	m_computePasses.push_back(pass);
}

void Renderer::onResize(WindowResizeEvent& e, int scale)
{
	for (auto& Rpass : m_renderPasses) {
		Rpass.target->onResize(e,scale);
	}
	for (auto& Rpass : m_depthPasses) {
		Rpass.target->onResize(e, scale);
	}
}


RenderPass& Renderer::getRenderPass(size_t index)
{
	auto& [passType, passIdx] = m_renderOrder[index];
	return m_renderPasses[passIdx];
}

DepthPass& Renderer::getDepthPass(size_t index)
{
	auto& [passType, passIdx] = m_renderOrder[index];
	return m_depthPasses[passIdx];
}

ComputePass& Renderer::getComputePass(size_t index)
{
	auto& [passType, passIdx] = m_renderOrder[index];
	return m_computePasses[passIdx];
}

void Renderer::updateRenderAndDepthPassSize(glm::ivec2 newSize)
{
	for (auto& pass : m_renderPasses) {
		if (pass.isScreen) {
			pass.viewPort = { 0,0,(unsigned)newSize.x, (unsigned)newSize.y };
			pass.camera.projection = glm::ortho(0.f, (float)newSize.x, (float)newSize.y, 0.f);
			pass.UBOmanager.setCachedValue("b_menuCamera", "u_menuProjection", pass.camera.projection);
			pass.UBOmanager.setCachedValue("b_relicCamera2D", "u_relicProjection2D", pass.camera.projection);
			pass.UBOmanager.setCachedValue("b_camera2D", "u_projection2D", pass.camera.projection);
		}
	}
}

void Renderer::render() const
{
	for (auto& [passType, idx] : m_renderOrder) {
		if (passType == PassType::render) {
			auto& renderPass = m_renderPasses[idx];
			renderPass.UBOmanager.uploadCachedValues();
		}
		else if (passType == PassType::render) {
			auto& depthPass = m_renderPasses[idx];
			depthPass.UBOmanager.uploadCachedValues();
		}
	}
	int rendered = 0;
	for (auto& [passType, idx] : m_renderOrder)
	{
		
		if (passType == PassType::render)
		{
			
			ZoneScopedN("RPass");
			TracyGpuZone("RPass");
			auto& renderPass = m_renderPasses[idx];
			renderPass.target->use();
			
			glm::mat4 VP = renderPass.camera.projection * renderPass.camera.view;
			ViewPort VPort = renderPass.viewPort;
			updateViewPort(VPort);

			if (renderPass.clearDepth && renderPass.clearColour) {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
			else if (renderPass.clearDepth) {
				glClear(GL_DEPTH_BUFFER_BIT);
			}
			else if (renderPass.clearColour) {
				glClear(GL_COLOR_BUFFER_BIT);
			}

			

			auto view = renderPass.scene->m_entities.view<Render, Transform>();
			auto obbs = renderPass.scene->m_entities.view<OBBCollider>();
			auto sphereColls = renderPass.scene->m_entities.view<SphereCollider>();
			glm::vec3 camPos = renderPass.camera.position;
			glm::vec3 camForward = renderPass.camera.forward;

			// Iterate camPos all actors in the scene



			for (auto entity : view) {
				Render& renderComp = renderPass.scene->m_entities.get<Render>(entity);
				Transform& transformComp = renderPass.scene->m_entities.get<Transform>(entity);
				//
				
				//ZoneScoped;
				ZoneScopedN("Actor");
				TracyGpuZone("Actor");
				if (renderComp.material) {

					renderComp.material->apply();
					if (renderComp.material->getTransformUniformName().length() > 0) renderComp.material->m_shader->uploadUniform(renderComp.material->getTransformUniformName(), transformComp.transform);

					if (renderComp.geometry) { // All geometry is indexed i.e. it uses IBOs

						float dist = glm::distance(transformComp.translation, camPos);
						refreshLOD(renderComp, dist / 500.0f);
						ZoneScopedN("Draw");
						TracyGpuZone("Draw");
						bindVertexArrays(renderComp.geometry->getID());
						glDrawElements(renderComp.material->getPrimitive(), renderComp.geometry->getDrawCount(), GL_UNSIGNED_INT, NULL);
						//rendered++;
					}
					else if (renderComp.SSBOgeometry) {
						ZoneScopedN("SSBO Draw");
						TracyGpuZone("SSBO Draw");
						glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, renderComp.SSBOgeometry->getID());  // binding point from material
						glDrawArrays(renderComp.material->getPrimitive(), 0, renderComp.SSBOgeometry->getElementCount());
						//rendered++;

					}
				}
			}

			std::unordered_map<InstacedRender, std::vector<glm::mat4>, RenderHasher, RenderEquals> instances;

			auto view2 = renderPass.scene->m_entities.view<InstacedRender, Transform>();

			for (auto& entity : view2) {
				auto& renderComp = renderPass.scene->m_entities.get<InstacedRender>(entity);
				auto& transformComp = renderPass.scene->m_entities.get<Transform>(entity);

				auto it = instances.find(renderComp);
				if (it != instances.end()) it->second.push_back(transformComp.transform);
				else instances[renderComp] = { transformComp.transform };
			}

			for (auto it = instances.begin(); it != instances.end(); ++it) {
				auto& renderComp = it->first;
				auto& models = it->second;
				//
				if (renderComp.material != nullptr) {
					renderComp.material->apply();
					bindVertexArrays(renderComp.geometry->getID());

					// Send data to SSBO
					m_modelsSSBO->edit(0, sizeof(glm::mat4) * models.size(), models.data());

					auto drawCount = renderComp.geometry->getDrawCount();
					glDrawElementsInstanced(renderComp.material->getPrimitive(), drawCount, GL_UNSIGNED_INT, NULL, models.size());
				}
			}
		}
		else if (passType == PassType::depth)
		{
			ZoneScopedN("DPass");
			TracyGpuZone("DPass");
			glCullFace(GL_FRONT);
			auto& depthPass = m_depthPasses[idx];
			depthPass.target->use();
			
			glm::mat4 VP = depthPass.camera.projection * depthPass.camera.view;
			ViewPort VPort = depthPass.viewPort;
			updateViewPort(VPort);

			if (depthPass.clearDepth) {
				glClear(GL_DEPTH_BUFFER_BIT);
			}


			

			auto view = depthPass.scene->m_entities.view<Render, Transform>();
			auto obbs = depthPass.scene->m_entities.view<OBBCollider>();
			auto sphereColls = depthPass.scene->m_entities.view<SphereCollider>();
			glm::vec3 camPos = depthPass.camera.position;
			glm::vec3 camForward = depthPass.camera.forward;
			//spdlog::info("Forward x:{} y:{} z:{}", camForward.x, camForward.y, camForward.z);
			// Iterate camPos all actors in the scene



			for (auto entity : view) {
				Render& renderComp = depthPass.scene->m_entities.get<Render>(entity);
				Transform& transformComp = depthPass.scene->m_entities.get<Transform>(entity);
				//

				bool InFrustrum = true;


				if (obbs.contains(entity)) {
					OBBCollider& obbComp = depthPass.scene->m_entities.get<OBBCollider>(entity);
					InFrustrum = false;
					glm::vec3 a = obbComp.getPointClosestToFront(camForward, camPos, transformComp.translation);
					glm::vec4 clipSpacePos = VP * glm::vec4(a, 1.0f);
					if (clipSpacePos.w > 0.0f && clipSpacePos.w < 1000.0f) {
						glm::vec2 b = glm::vec2(clipSpacePos) / clipSpacePos.w;
						b += glm::vec2(1);
						b *= 0.5f;
						if (b.x > 0 && b.x < 1 && b.y > 0 && b.y < 1) {
							InFrustrum = true;
						}
					}
				};

				if (sphereColls.contains(entity)) {
					SphereCollider& sphereCollComp = depthPass.scene->m_entities.get<SphereCollider>(entity);
					InFrustrum = false;
					glm::vec3 a = sphereCollComp.getPointClosestToFront(camForward, camPos, transformComp.translation);
					glm::vec4 clipSpacePos = VP * glm::vec4(a, 1.0f);
					if (clipSpacePos.w > 0.0f && clipSpacePos.w < 1000.0f) {
						glm::vec2 b = glm::vec2(clipSpacePos) / clipSpacePos.w;
						b += glm::vec2(1);
						b *= 0.5f;
						if (b.x > 0 && b.x < 1 && b.y > 0 && b.y < 1) {
							//spdlog::info("Projected: x: {} y:{}", b.x, b.y);
							InFrustrum = true;
						}
					}

				};
				
				//ZoneScoped;
				ZoneScopedN("Actor");
				TracyGpuZone("Actor");
				if (renderComp.depthMaterial && InFrustrum) {

					renderComp.depthMaterial->apply();
					if (renderComp.depthMaterial->getTransformUniformName().length() > 0) renderComp.depthMaterial->m_shader->uploadUniform(renderComp.depthMaterial->getTransformUniformName(), transformComp.transform);

					if (renderComp.depthGeometry) { // All geometry is indexed i.e. it uses IBOs

						float dist = glm::distance(transformComp.translation, camPos);
						refreshLOD(renderComp, dist / 500.0f);
						ZoneScopedN("Draw");
						TracyGpuZone("Draw");
						bindVertexArrays(renderComp.depthGeometry->getID());
						glDrawElements(renderComp.depthMaterial->getPrimitive(), renderComp.depthGeometry->getDrawCount(), GL_UNSIGNED_INT, NULL);
						//rendered++;
					}
					else if (renderComp.SSBOgeometry) {
						ZoneScopedN("SSBO Draw");
						TracyGpuZone("SSBO Draw");
						glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, renderComp.SSBOgeometry->getID());  // binding point from material
						glDrawArrays(renderComp.depthMaterial->getPrimitive(), 0, renderComp.SSBOgeometry->getElementCount());
						//rendered++;

					}
				}



			}

			std::unordered_map<InstacedRender, std::vector<glm::mat4>, RenderHasher, RenderEquals> instances;

			auto view2 = depthPass.scene->m_entities.view<InstacedRender, Transform>();

			for (auto& entity : view2) {
				auto& renderComp = depthPass.scene->m_entities.get<InstacedRender>(entity);
				auto& transformComp = depthPass.scene->m_entities.get<Transform>(entity);

				auto it = instances.find(renderComp);
				if (it != instances.end()) it->second.push_back(transformComp.transform);
				else instances[renderComp] = { transformComp.transform };
			}

			for (auto it = instances.begin(); it != instances.end(); ++it) {
				auto& renderComp = it->first;
				auto& models = it->second;
				//
				if (renderComp.depthMaterial != nullptr) {
					renderComp.depthMaterial->apply();
					bindVertexArrays(renderComp.depthGeometry->getID());

					// Send data to SSBO
					m_modelsSSBO->edit(0, sizeof(glm::mat4) * models.size(), models.data());

					auto drawCount = renderComp.depthGeometry->getDrawCount();
					glDrawElementsInstanced(renderComp.depthMaterial->getPrimitive(), drawCount, GL_UNSIGNED_INT, NULL, models.size());
				}
			}

			glCullFace(GL_BACK);
		}
		else if (passType == PassType::compute)
		{
			ZoneScopedN("CPass");
			TracyGpuZone("CPass");
			auto& computePass = m_computePasses[idx];

			// Bind images -- Note: Could consider an image unit manager if this becomes a hot path
			for (auto& img : computePass.images) {
				GLboolean layered = img.layer ? GL_TRUE : GL_FALSE;
				
				GLenum access = 0;
				switch (img.access)
				{
				case TextureAccess::ReadOnly:
					access = GL_READ_ONLY; 
					break;
				case TextureAccess::WriteOnly:
					access = GL_WRITE_ONLY;
					break;
				case TextureAccess::ReadWrite:
					access = GL_READ_WRITE;
					break;
				}

				GLenum fmt = 0; // Ignoring depth for now
				if (img.texture->isHDR()) { 
					if (img.texture->getChannels() == 3) fmt = GL_RGB16F;
					else fmt = GL_RGBA16F;
				}
				else {
					if (img.texture->getChannels() == 3) fmt = GL_RGB8;
					else fmt = GL_RGBA8;
				}

				// Need to deal with layers for cubemap
				glBindImageTexture(img.imageUnit, img.texture->getID(), img.mipLevel, layered, 0, access, fmt);
			}

			computePass.material->apply();
			auto& wg = computePass.workgroups;
			glDispatchCompute(wg.x, wg.y, wg.z);
			if(computePass.barrier != MemoryBarrier::None) glMemoryBarrier(static_cast<GLbitfield>(computePass.barrier));
		}
		
	}
	//if(rendered > 4) spdlog::info("Total Rendered: {}", rendered);
}

void Renderer::updateViewPort(ViewPort newVP)
{
	static ViewPort LastVP;

	if (LastVP != newVP) {
		LastVP = newVP;
		glViewport(LastVP.x, LastVP.y, LastVP.width, LastVP.height);

	}
}

void Renderer::bindVertexArrays(uint32_t newVAO)
{
	static uint32_t LastVAO;

	if (LastVAO != newVAO) {
		LastVAO = newVAO;
		glBindVertexArray(newVAO);
	}
}



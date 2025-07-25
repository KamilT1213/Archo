#include "rendering/material.hpp"
#include "tracy/TracyOpenGL.hpp"

Material::Material(std::shared_ptr<Shader> shader, const std::string& transformUniformName) : 
	m_shader(shader),
	m_transformUniformName(transformUniformName)
{
	auto& infoCache = shader->m_uniformInfoCache;

	for (auto it = infoCache.begin(); it != infoCache.end(); ++it)
	{
		auto& name = it->first;
		auto& info = it->second;

		if (info.size > 1) {
			int location = name.find("[0");
			for (int i = 0; i < info.size; i++) {
				dataCache[name.substr(0, location + 1) + std::to_string(i) + "]"] = UniformData(info.type, 1);
			}
		}
		else {
			dataCache[name] = UniformData(info.type, info.size);
		}


	}




}

void Material::unsetValue(const std::string& name)
{
	auto it = dataCache.find(name);
	if (it != dataCache.end()) {
		auto& data = dataCache[name];
		data.enabled = false;
	}
	else { spdlog::error("Could not set material value: {}. Not found in shader data cache.", name); }	
}

void Material::apply()
{
	ZoneScopedN("Material");
	TracyGpuZone("Material");
	// Bind shader
	useShader(m_shader->getID());

	// Upload uniforms
	for (auto& dataPair : dataCache)
	{
		auto& name = dataPair.first;
		auto& matInfo = dataPair.second;

		if (matInfo.enabled)
		{
			auto& dispatchFunc = UniformConsts::UDT.at(matInfo.type);

			dispatchFunc(m_shader, name, matInfo);
			
			/*
			switch (matInfo.type)
			{
			case GL_FLOAT_VEC3:
				m_shader->uploadUniform(name, std::get<glm::vec3>(matInfo.data));
				break;
			case GL_SAMPLER_2D:
				m_shader->uploadUniform(name, std::get<std::shared_ptr<Texture>>(matInfo.data)->getUnit());
				break;
			}*/
		}
	}
}

void Material::useShader(uint32_t shader_ID)
{
	static uint32_t last_shader_ID;
	if (last_shader_ID != shader_ID) {
		last_shader_ID = shader_ID;
		glUseProgram(shader_ID);
	}
}


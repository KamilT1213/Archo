#include "buffers/UBOmanager.hpp"
#include "tracy/TracyOpenGL.hpp"

bool UBOManager::addUBO(const UBOLayout& layout)
{
	bool result = false;
	if (layout.getBindingPoint() < m_UBOs.size())
	{
		bool sameUBO = m_UBOs[layout.getBindingPoint()]->getLayout() == layout;
		if (!sameUBO) result = false;
		// Same UBO, we've already added it!
	}
	else
	{
		m_UBOs.push_back(std::make_shared<UBO>(layout));
		m_UBOMap[layout.getBlockName()] = m_UBOs.size() - 1;
	}
	return result;
}

void UBOManager::uploadCachedValues() const
{
	ZoneScopedN("UBO");
	TracyGpuZone("UBO");

	for (auto uboID : m_UBOMap) {
		auto& ubo = m_UBOs[uboID.second];

		ubo->uploadBulkData();
	}
}

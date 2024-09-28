#include "VirtualGeometryStreamer.h"

#include "NRIFramework.h"
#include "MeshletStructs.h"

/*
#include <string.h>

#include "../../Assets/exts.h"

#include "../../Assets/AssetManager.h"
#include "../../Common/utils.h"

void VirtualGeometryStreamer::Init()
{
	UnInit();

	AssetMap& map = AssetManager::GetMap();

	for (auto& asset : map)
	{
		if (asset.second->type != AssetType::VirtualModel) {
			continue;
		}
		VModel* m = dynamic_cast<VModel*>(asset.second);
		if (!m) {
			continue;
		}

		std::string path = utils::GetCFullPath(m->name, utils::CustomFolder::VIRTUALMESH) + VModelExt;
		FILE* modelData = fopen(path.c_str(), "r");

		if (!modelData) {
			continue;
		}
		
		m->ID = models.size();

		ModelDesc mD;

		mD.meshletOffset = meshletDescs.size();
		mD.meshletCount = m->meshlets.size();

		meshletDescs.reserve(m->meshlets.size());
		for (MeshletDesc& mlD : m->meshlets)
		{
			meshletDescs.push_back(mlD);
		}

		modelDescs.push_back(mD);

		models.push_back(modelData);
	}
}


void VirtualGeometryStreamer::Load(uint32_t modelID, void* buffer, uint32_t clusterOffset, uint32_t clusterlenght)
{
	if (modelID >= models.size()|| modelID < 0) {
		return;
	}

	FILE* f = models[modelID];

	if (f == NULL) {
		models.erase(models.begin() + modelID);
		return;
	}

	fseek(f, clusterOffset, SEEK_SET);
	fread(buffer, clusterlenght, 1, f);
}

void VirtualGeometryStreamer::UnInit() {
	for (auto& file : models)
	{
		fclose(file);
	}
	models.clear();
	modelDescs.clear();
	meshletDescs.clear();

	// needs to clear bindles buffers on GPU
}
*/
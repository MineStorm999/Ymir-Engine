#include "VirtualGeometryStreamer.h"

#include "NRIFramework.h"
#include "../../../Shaders/MeshletStructs.h"

#include <unordered_map>
#include <string.h>



using ModelMap = std::unordered_map<uint32_t, FILE*>;

ModelMap models;

void VirualGeometryStreamer::Init()
{

}


void VirualGeometryStreamer::Load(uint32_t modelID, void* buffer, uint32_t clusterOffset, uint32_t clusterlenght)
{
	if (models.find(modelID) == models.end()) {
		return;
	}

	FILE* f = models[modelID];

	if (f == NULL) {
		models.erase(modelID);
		return;
	}

	fseek(f, clusterOffset, SEEK_SET);
	fread(buffer, clusterlenght, 1, f);
}

void VirualGeometryStreamer::UnInit() {
	for (auto const& [key, val] : models)
	{
		fclose(val);
	}
}

#pragma once
#include <memory>
class VGeomLoader {

	static void Init();

	static void Load(uint32_t modelID, void* buffer, uint32_t clusterOffset, uint32_t clusterlenght);
};
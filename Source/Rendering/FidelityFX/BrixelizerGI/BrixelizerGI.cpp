#include "BrixelizerGI.h"
#include "include/FidelityFX/host/ffx_brixelizer.h"
#include <vector>
BrixelizerGIInterFace::BrixelizerGIInterFace(const BrixelizerGIInitDesc& desc)
{
	FfxBrixelizerContextDescription brixDesc = {};
	brixDesc.sdfCenter[0] = 0.f;
	brixDesc.sdfCenter[1] = 0.f;
	brixDesc.sdfCenter[2] = 0.f;
	brixDesc.numCascades = desc.cascades;

	std::vector<FfxBrixelizerCascadeDescription> cascadeDescs(desc.cascades);

	float vxSize = desc.voxelSize;
	uint32_t numCascadeResources = 0;
	for (uint32_t i = 0; i < brixDesc.numCascades; ++i) {
		FfxBrixelizerCascadeDescription& cascadeDesc = cascadeDescs[i];
		cascadeDesc.flags = (FfxBrixelizerCascadeFlag)(FFX_BRIXELIZER_CASCADE_STATIC | FFX_BRIXELIZER_CASCADE_DYNAMIC);	// Flags for cascade creation. See <c><i>FfxBrixelizerCascadeFlag</i></c>.
		cascadeDesc.voxelSize = vxSize;										// The edge size of voxels in world space for the cascade.
		numCascadeResources += 3;
		vxSize *= desc.voxelCascadeRatio;
	}

	//brixDesc.backendInterface = 
}


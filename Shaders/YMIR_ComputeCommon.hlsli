#pragma once

#include "YMIR_cpu_gpu_shared.h"
#include "YMIR_MeshletStructs.hlsli"

NRI_RESOURCE(SamplerState, AnisotropicSampler, s, 0, 0 );

// structured buffer
NRI_RESOURCE(StructuredBuffer<ModelDesc>, Models, t, 0, 0);
NRI_RESOURCE(StructuredBuffer<MeshletDesc>, Meshlets, t, 1, 0);

NRI_RESOURCE(StructuredBuffer<uint32_t>, RenderDataO, t, 2, 0);
NRI_RESOURCE(StructuredBuffer<uint32_t>, RenderDataI, t, 3, 0);

NRI_RESOURCE(StructuredBuffer<InstanceDesc>, Instances, t, 4, 0);
NRI_RESOURCE(StructuredBuffer<uint>, InstancesLastFrame, t, 5, 0);

NRI_RESOURCE(RWBuffer<RenderCmd>, RenderCMDO, u, 0, 0);
NRI_RESOURCE(RWBuffer<RenderCmd>, RenderCMDI, u, 1, 0);
NRI_RESOURCE(RWBuffer<MeshletLoadDesc>, ReadBack, u, 2, 0);



//NRI_RESOURCE

groupshared uint32_t s_drawID;
groupshared uint32_t s_drawLength;
groupshared uint32_t s_loadID;
groupshared uint32_t s_stopp;

#define FATALERROR 2
#define ERROR 1
#define SUCSESS 0

InstanceDesc GetLastFrameInstance(uint lastFrameID){
	InstanceDesc ret = (InstanceDesc)0;
	
	if(lastFrameID < 0 || lastFrameID >= instanceCountOld){
		return ret;
	}
	
	if(InstancesLastFrame[lastFrameID] == 0xffffffff){
		return ret;
	}
	
	return Instances[InstancesLastFrame[lastFrameID]];
}



uint32_t Render(uint32_t modelID, uint32_t clusterID, uint32_t instanceID){
	if(clusterID == 0xffffffff && modelID == 0xffffffff && instanceID > MAXMESHINSTANCES){
		return ERROR;
	}
	
	MeshletDesc meshlet = Meshlets[Models[modelID].meshletOffset + clusterID];
	
	
	uint32_t drawID = 0;
	InterlockedAdd(s_drawID, 1, drawID);
	
	if(drawID > RENDERCMDS){
		InterlockedAdd(s_stopp, 1);
		return FATALERROR;
	}
	
	uint32_t rClusterID = Models[modelID].meshletOffset + clusterID;
	
	uint32_t offset = 0;
	
	bool found = false;
	if(bufferToUse == 0){		
		for (uint i = 0; i < drawID; i++){
			if(rClusterID == RenderCMDO[i].realClusterID){
				offset = RenderCMDO[i].offset;
			}
		}
	}else{
		for (uint i = 0; i < drawID; i++){
			if(rClusterID == RenderCMDI[i].realClusterID){
				offset = RenderCMDI[i].offset;
			}
		}
	}
	
	if(!found){
		InterlockedAdd(s_drawLength, meshlet.clusterLenght, offset);
		offset -= meshlet.clusterLenght;
		
		// meshlet load desc
		MeshletLoadDesc mLDesc = (MeshletLoadDesc)0;
		mLDesc.clusterID = meshlet.clusterOffset;
		mLDesc.modelID = meshlet.modelID;
		mLDesc.clusterOffset = offset;
		mLDesc.clusterLenght = meshlet.clusterLenght;
		
		uint32_t loadID = 0;
		if(loadID > READBACKS-1){
			InterlockedAdd(s_stopp, 1);
			return FATALERROR;
		}
		InterlockedAdd(s_loadID, 1, loadID);
		// readback[loadID] = mLDesc; 
	}
	
	// render cmd
	RenderCmd rCmd = (RenderCmd)0;
	rCmd.realClusterID = rClusterID;
	rCmd.offset = offset;
	rCmd.vertTriCount = meshlet.vertTriCount;
	rCmd.instanceID = instanceID;
	
	
	
	if(bufferToUse==0){
		RenderCMDO[drawID] = rCmd;
		//todo readback
	}else{
		RenderCMDO[drawID] = rCmd;
		//todo readback
	}
	
	return SUCSESS;
}
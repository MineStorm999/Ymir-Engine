#pragma once

#include "NRIFramework.h"
#include "entt/entt.hpp"

#include "Assets/AssetManager.h"

struct Transform {
	float4x4 localMat;
	
	float3 localPos{ 0,0,0 };
	float3 localScale{ 1,1,1 };
	float3 localRot{ 0,0,0 };

	bool changed;
};

struct MeshInstance {
	uint32_t materialID{ 0xffffffff };
	AssetID modelID;
};

struct Root{};

struct Identity {
	std::string name{"New Entity"};

	entt::entity parent{entt::null};
	std::vector<entt::entity> childs{};
};


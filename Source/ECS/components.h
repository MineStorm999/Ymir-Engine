#pragma once

#include "NRIFramework.h"
#include "entt/entt.hpp"

#include "Assets/AssetManager.h"

struct TransformComponent {
	float4x4 localMat{ float4x4::Identity() };
	
	float3 localPos{ 0,0,0 };
	float3 localScale{ 1,1,1 };
	float3 localRot{ 0,0,0 };

	float4x4 localToWorldMat{float4x4::Identity()};
};

struct MeshInstanceComponent {
	AssetID materialID{ INVALID_ASSET_ID };
	AssetID modelID{ INVALID_ASSET_ID };
};

struct Root{};

struct IdentityComponent {
	std::string name{"New Entity"};

	RenderID instanceGPUID{ INVALID_RENDER_ID };

	entt::entity parent{entt::null};
	std::vector<entt::entity> childs{};
};

struct CameraComponent {

};

struct Dirty{};

struct RigidBody {

};
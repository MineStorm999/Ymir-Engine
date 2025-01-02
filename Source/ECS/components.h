#pragma once

#include "NRIFramework.h"
#include "entt/entt.hpp"
#include "..//World/PhysicsWorld.h"
#include "Assets/AssetManager.h"

struct FActive {};
struct FInActive {};

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

struct FRoot{};

struct IdentityComponent {
	std::string name{"New Entity"};

	RenderID instanceGPUID{ INVALID_RENDER_ID };

	entt::entity parent{entt::null};
	std::vector<entt::entity> childs{};
};

struct ColliderComponent {
	entt::entity connectedRigidBody;
	JPH::Ref<JPH::Shape> shape;
};

struct RigidBodyComponent {
	JPH::BodyID id;
};

struct CameraComponent {

};

enum class DirtyFlags : uint8_t {
	None = 0,
	Transform = 1,
	RigidBody = 1 << 1,
	Collider = 1 << 2,

	All = 0xff
};

struct FDirty{
	DirtyFlags flags{DirtyFlags::None};
};


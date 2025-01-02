#pragma once


#include <entt/entt.hpp>

#include "../JSON/json.hpp"
#include "Assets/assetTypes.h"
#include "components.h"
#include "../World/SceneManager.h"

using ECSWorld = entt::registry;


using ViewID = uint32_t;
using CallBack = std::function<ViewID(ECSWorld&)>;

struct TransformComponent;



class EntityManager {
public:
	static ECSWorld& GetWorld();
	static entt::entity GetRoot();

	static void SetWorld(ECSWorld* w);

	static ECSWorld& LoadWorld(nlohmann::json& json);
	static nlohmann::json SerializeWorld(ECSWorld& w);

	static void UpdateMatrizes();

	static void Init();

	// todo
	static void DeleteEntity(entt::entity e);

	static entt::entity CreateEntity(std::string name, entt::entity parent = entt::null);
	static entt::entity CreateEntity(std::string name, AssetID assetOriginal, entt::entity parent = entt::null);

	static void SetDirty(entt::entity e, DirtyFlags flags = DirtyFlags::All);

	static void DestroyEntity(entt::entity e);


	static void AddChild(entt::entity parent, entt::entity child);

	static bool IsValid(entt::entity e) {
		return GetWorld().valid(e);
	};

	template<typename C>
	static bool HasComponent(entt::entity e) {
		return GetWorld().try_get<C>(e) != nullptr;
	};

	template<typename C>
	static C& AddComponent(entt::entity e) {
		SetDirty(e);
		return GetWorld().emplace_or_replace<C>(e);
	};

	template<typename C>
	static C& GetComponent(entt::entity e) {
		return GetWorld().get<C>(e);
	};

	template<typename C>
	static void RemoveComponent(entt::entity e) {
		SetDirty(e);
		GetWorld().remove<C>(e);
	};

	template<>
	static void RemoveComponent<RigidBodyComponent>(entt::entity e) {
		if (SceneManager::GetPhysicsWorld() != nullptr) {
			GetWorld().remove<RigidBodyComponent>(e);
			return;
		}
		
		if (HasComponent<ColliderComponent>(e)) {
			SceneManager::GetPhysicsWorld()->GetPhysicsSystem().GetBodyInterface().DeactivateBody(GetComponent<RigidBodyComponent>(e).id);
		}
		else {
			SceneManager::GetPhysicsWorld()->DestroyBody(GetComponent<RigidBodyComponent>(e).id);
		}

		GetWorld().remove<RigidBodyComponent>(e);
	};
};


namespace Components{

	class Transform {
	public:
		static void SetPosition(entt::entity e, const float3& pos, TransformComponent& t);
		static void SetRotation(entt::entity e, const float3& rot, TransformComponent& t);
		static void SetScale(entt::entity e, const float3& scale, TransformComponent& t);
	};

	class RigidBody {
	public:
		static float3 GetVelocity(entt::entity e, RigidBodyComponent& rb);
		static float3 GetAngularVelocity(entt::entity e, RigidBodyComponent& rb);
		static float3 GetCenterOfMass(entt::entity e, RigidBodyComponent& rb);
		static float GetMass(entt::entity e, RigidBodyComponent& rb);

		static void SetVelocity(entt::entity e, RigidBodyComponent& rb, const float3& velocity);
		static void SetAngularVelocity(entt::entity e, RigidBodyComponent& rb, const float3& angularVelocity);

		static void AddForce(entt::entity e, RigidBodyComponent& rb, const float3& force);
		static void AddTorque(entt::entity e, RigidBodyComponent& rb, const float3& torque);
	};
};
#pragma once


#include <entt/entt.hpp>

#include "../JSON/json.hpp"
#include "Assets/assetTypes.h"

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
	static void DeleteEntity(entt::entity e) {};

	static entt::entity CreateEntity(std::string name, entt::entity parent = entt::null);
	static entt::entity CreateEntity(std::string name, AssetID assetOriginal, entt::entity parent = entt::null);

	class Transform {
	public:

		static void SetPosition(entt::entity e, float3 pos, TransformComponent& t);
		static void SetRotation(entt::entity e, float3 rot, TransformComponent& t);
		static void SetScale(entt::entity e, float3 scale, TransformComponent& t);
	};
};
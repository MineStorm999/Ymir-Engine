#pragma once


#include <entt/entt.hpp>

#include "../JSON/json.hpp"

using ECSWorld = entt::registry;

using ViewID = uint32_t;
using CallBack = std::function<ViewID(ECSWorld&)>;


class EntityManager {
public:
	static ECSWorld& GetWorld();
	static entt::entity GetRoot();

	static void SetWorld(ECSWorld* w);

	static ECSWorld& LoadWorld(nlohmann::json& json);
	static nlohmann::json SerializeWorld(ECSWorld& w);

	static void UpdateMatrizes();

	static void Init();

	static entt::entity CreateEntity(std::string name);
	static entt::entity CreateEntity(std::string name, entt::entity parent);
};
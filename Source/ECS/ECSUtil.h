#pragma once

#include <string>
#include "ECSManager.h"
class ECSUtil {
public:
	entt::entity CreateEntity(std::string name);
	entt::entity CreateEntity(std::string name, entt::entity parent);

	entt::entity CreateMeshInstance(std::string name, std::string assetName);
	entt::entity CreateMeshInstance(std::string name, std::string assetName, entt::entity parent);
};
#pragma once

#include "flecs.h"

#include "../JSON/json.hpp"

class EntityManager {
public:
	static flecs::world& GetWorld();

	static void SetWorld(flecs::world w);

	static flecs::world& LoadWorld(nlohmann::json& j);
	static nlohmann::json SerializeWorld(flecs::world& w);

	
};
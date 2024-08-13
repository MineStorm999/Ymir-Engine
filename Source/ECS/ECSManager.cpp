#include "ECSManager.h"

flecs::world curWorld;

flecs::world& EntityManager::GetWorld()
{
    return curWorld;
}

void EntityManager::SetWorld(flecs::world w)
{
    curWorld = w;
}

flecs::world& EntityManager::LoadWorld(nlohmann::json& j)
{
    flecs::world ret;
    //ret.from_json();
    return ret;
}

nlohmann::json EntityManager::SerializeWorld(flecs::world& w)
{
    flecs::string val = w.to_json();
    return nlohmann::json(val);
}

#include "ECSUtil.h"
#include "components.h"
entt::entity ECSUtil::CreateEntity(std::string name)
{
    return EntityManager::CreateEntity(name);
}

entt::entity ECSUtil::CreateEntity(std::string name, entt::entity parent)
{
    return EntityManager::CreateEntity(name, parent);
}

entt::entity ECSUtil::CreateMeshInstance(std::string name, std::string assetName)
{
    entt::entity e = EntityManager::CreateEntity(name);
    EntityManager::GetWorld().emplace<MeshInstanceComponent>(e);
    return e;
}

entt::entity ECSUtil::CreateMeshInstance(std::string name, std::string assetName, entt::entity parent)
{
    return entt::entity();
}

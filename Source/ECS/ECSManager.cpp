#include "ECSManager.h"


#include <functional>
#include "components.h"
#include <unordered_map>


ECSWorld* curWorld;
entt::entity root;

ECSWorld& EntityManager::GetWorld()
{
    return *curWorld;
}

entt::entity EntityManager::GetRoot()
{
    return root;
}

void EntityManager::SetWorld(ECSWorld* w)
{
    curWorld = w;
    if (w->group<Root>().size() <= 0) {
        root = CreateEntity("Root");
    }
    else {
        root = w->group<Root>()[0];
    }
    Init();
}

ECSWorld& EntityManager::LoadWorld(nlohmann::json& json)
{
    ECSWorld* ret = new ECSWorld();

    // temp!!!!
    json.is_binary();
    //ret.from_json();
    return *ret;
}

nlohmann::json EntityManager::SerializeWorld(ECSWorld& w)
{
    // temp
    entt::entity e = w.create();
    w.emplace<Transform>(e);
    //flecs::string val = w.to_json();
    return nlohmann::json();
}

void EntityManager::UpdateMatrizes()
{
    auto t = GetWorld().group<Transform>();
    t.each([](Transform& t){
        if (t.changed) {
            t.localMat.SetupByRotationYPR(t.localRot.x, t.localRot.y, t.localRot.z);
            
            t.localMat.SetTranslation(t.localPos);
            
            t.localMat.AddScale(t.localScale);
        }
        t.changed = false;
        });
}


void EntityManager::Init()
{
    if (!curWorld) {
        return;
    }

    auto transforms = curWorld->group<Transform>();
    auto renderer = curWorld->group<Transform, MeshInstance>();
}

entt::entity EntityManager::CreateEntity(std::string name)
{
    entt::entity e = curWorld->create();
    Identity& id = curWorld->emplace<Identity>(e);
    id.name = name;

    curWorld->emplace<Transform>(e);
    return e;
}

entt::entity EntityManager::CreateEntity(std::string name, entt::entity parent)
{
    entt::entity e = curWorld->create();
    Identity& id = curWorld->emplace<Identity>(e);
    id.name = name;
    id.parent = parent;

    curWorld->emplace<Transform>(e);
    return e;
}




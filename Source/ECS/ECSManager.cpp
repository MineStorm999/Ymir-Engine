#include "ECSManager.h"


#include <functional>
#include "components.h"
#include <unordered_map>
#include "World/SceneManager.h"
#include "Log/Log.h"
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
        w->emplace<Root>(root);
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
    if(t.size() < 1){
        return;
    }
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
        ECSWorld* world = new ECSWorld();
        SetWorld(world);
    }

    curWorld->group<Transform>();
    curWorld->group<MeshInstance>();
    curWorld->group<Identity>();
}

entt::entity EntityManager::CreateEntity(std::string name, entt::entity parent)
{
    Log::Message("EntityManager", "Create Entity: " + name);
    entt::entity e = curWorld->create();
    Identity& id = curWorld->emplace<Identity>(e);
    id.name = name;
    curWorld->emplace<Transform>(e);

    if (name == "Root") {
        return e;
    }
    if (parent == entt::null) {
        id.parent = GetRoot();
        Identity& id2 = GetWorld().get<Identity>(GetRoot());
        id2.childs.push_back(e);
        return e;
    }
    id.parent = parent;
    Identity& id2 = GetWorld().get<Identity>(parent);
    id2.childs.push_back(e);
    return e;
}

entt::entity EntityManager::CreateEntity(std::string name, AssetID assetOriginal, entt::entity parent)
{
    if (!AssetManager::IsValid(assetOriginal)) {
        return entt::null;
    }

    entt::entity e = GetWorld().valid(parent) ? CreateEntity(name, parent) : CreateEntity(name);

    AssetBase* asset = AssetManager::GetAsset(assetOriginal);
    if (asset->type == AssetType::None) {
        return e;
    }
    if (asset->type == AssetType::Scene) {
        return e;
    }
    if (asset->type == AssetType::Audio) {
        return e;
    }
    if (asset->type == AssetType::Material) {
        return e;
    }
    if (asset->type == AssetType::Texture) {
        return e;
    }
    if (asset->type == AssetType::Model) {
        AModel* m = dynamic_cast<AModel*>(asset);
        if (!m) {
            return e;
        }
        AScene* s = SceneManager::GetSceneAsset();
        if (!s) {
            return e;
        }

        MeshInstance& mInst = GetWorld().emplace<MeshInstance>(e);
        mInst.modelID = assetOriginal;
        mInst.materialID = m->DefaultMaterialID;
        return e;
    }

    return e;
}




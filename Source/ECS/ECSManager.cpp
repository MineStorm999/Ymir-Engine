#include "ECSManager.h"


#include <functional>
#include "components.h"
#include <unordered_map>
#include "World/SceneManager.h"
#include "Log/Log.h"
#include "MathLib/ml.h"

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
    if (w->group<FRoot>().size() <= 0) {
        root = CreateEntity("Root");
        w->emplace<FRoot>(root);
    }
    else {
        root = w->group<FRoot>()[0];
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
    w.emplace<TransformComponent>(e);
    //flecs::string val = w.to_json();
    return nlohmann::json();
}

void EntityManager::UpdateMatrizes()
{}


void EntityManager::Init()
{
    if (!curWorld) {
        ECSWorld* world = new ECSWorld();
        SetWorld(world);
    }
}

void EntityManager::DeleteEntity(entt::entity e)
{
    Log::Message("Entity Manager", "Deleting Entities is not Supported!!!");
}

entt::entity EntityManager::CreateEntity(std::string name, entt::entity parent)
{

    //Log::Message("EntityManager", "Create Entity: " + name);
    entt::entity e = curWorld->create();
    IdentityComponent& id = curWorld->emplace<IdentityComponent>(e);
    id.name = name;
    curWorld->emplace<TransformComponent>(e);
    //curWorld->emplace<Dirty>(e);
    if (name == "Root") {
//        GetWorld().emplace<Dirty>(e);
        return e;
    }
    if (parent == entt::null) {
        id.parent = GetRoot();
        IdentityComponent& id2 = curWorld->get<IdentityComponent>(GetRoot());
        id2.childs.push_back(e);
        GetWorld().emplace<FDirty>(e);
        return e;
    }
    id.parent = parent;
    IdentityComponent& id2 = curWorld->get<IdentityComponent>(parent);
    id2.childs.push_back(e);
    GetWorld().emplace<FDirty>(e);
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

        MeshInstanceComponent& mInst = GetWorld().emplace<MeshInstanceComponent>(e);
        mInst.modelID = assetOriginal;
        mInst.materialID = m->DefaultMaterialID;
        return e;
    }

    return e;
}

void EntityManager::Transform::SetPosition(entt::entity e, const float3& pos, TransformComponent& t)
{
    if (t.localPos.x == pos.x && t.localPos.y == pos.y && t.localPos.z == pos.z) {
        return;
    }
    t.localPos = pos;
    GetWorld().emplace<FDirty>(e);
}

void EntityManager::Transform::SetRotation(entt::entity e, const float3& rot, TransformComponent& t)
{
    if (t.localRot.x == rot.x && t.localRot.y == rot.y && t.localRot.z == rot.z) {
        return;
    }
    t.localRot = rot;
    GetWorld().emplace<FDirty>(e);
}

void EntityManager::Transform::SetScale(entt::entity e, const float3& scale, TransformComponent& t)
{
    if (t.localScale.x == scale.x && t.localScale.y == scale.y && t.localScale.z == scale.z) {
        return;
    }
    t.localScale = scale;
    GetWorld().emplace<FDirty>(e);
}

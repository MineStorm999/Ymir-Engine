#include "ECSManager.h"


#include <functional>
#include "components.h"
#include <unordered_map>
#include "World/SceneManager.h"

#include "MathLib/ml.h"
#include "Assets/AssetHelper.h"

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


nlohmann::json EntityManager::SerializeWorld()
{
    nlohmann::json j;
    
    std::unordered_map<entt::entity, uint32_t> entSerMap;
    uint32_t i = 0;
    
    auto view = GetWorld().view<IdentityComponent, TransformComponent>();

    for (auto e : view)
    {
        entSerMap[e] = i;
        i++;
    }

    j["EntityCount"] = i;

    i = 0;
    for (auto [e, id, t] : view.each()) {
        nlohmann::json eJ;
        eJ["Name"] = id.name;
        eJ["Parent"] = (entSerMap.find(id.parent) != entSerMap.end()) ? entSerMap[id.parent] : entt::null;
        {
            //TransformComponent& t = GetWorld().get<TransformComponent>(e);
            nlohmann::json tJ;
            tJ["Position"] = JSONHelper::TJson(t.localPos);
            tJ["Scale"] = JSONHelper::TJson(t.localScale);
            tJ["Rotation"] = JSONHelper::TJson(t.localRot);
            tJ["Static"] = JSONHelper::TJson(t.isStatic);
            eJ["Components"]["Transform"] = tJ;
        }
        if (auto* mesh = GetWorld().try_get<MeshInstanceComponent>(e)) {
            nlohmann::json mJ;
            mJ["MaterialID"] = mesh->materialID;
            mJ["MeshID"] = mesh->modelID;
            //mJ["Has"] = "True";
            eJ["Components"]["MeshInstance"] = mJ;
        }
        else {
            //eJ["Components"]["MeshInstance"]["Has"] = "False";
        }
        
        eJ["Active"] = HasComponent<FInActive>(e) ? "False" : "True";

        j["Entities"][std::to_string(i)] = eJ;
        i++;
    }
    j["EntityCount"] = i;

    j["Root"] = entSerMap[GetRoot()];
    return j;
}

void EntityManager::UpdateMatrizes()
{}

void EntityManager::Init(nlohmann::json& json, bool save)
{
    if (json.empty()) {
        return;
    }

    if (curWorld) {
        if (save) {
            AssetManager::Save();
        }
        delete curWorld;
        curWorld = nullptr;
    }
    
    //std::cout << json << std::endl;
    delete curWorld;
    curWorld = nullptr;
    ECSWorld* w = new ECSWorld();

    curWorld = w;

    root = CreateEntity("Root");
    w->emplace<FRoot>(root);

    std::unordered_map<uint32_t, entt::entity> entDeSerMap;

    uint32_t entCount = json["EntityCount"];

    // create entites
    for (size_t i = 0; i < entCount; i++)
    {
        nlohmann::json eJ = json["Entities"][std::to_string(i)];
        if (eJ["Name"] == "Root") {
            entDeSerMap[i] = entt::null;
            continue;
        }
        entt::entity e = CreateEntity(eJ["Name"]);
        if (!IsValid(e)) {
            entDeSerMap[i] = entt::null;
            continue;
        }
        Log::Message("Entity Manager", "Loaded Entity " + std::string(eJ["Name"]) + std::string("   ") + std::to_string(i + 1) + std::string("/") + std::to_string(entCount));
        entDeSerMap[i] = e;
        {
            nlohmann::json tJ = eJ["Components"]["Transform"];
            TransformComponent& t = GetWorld().get<TransformComponent>(e);

            Components::Transform::SetPosition(e, JSONHelper::FrmJsonf3(tJ["Position"]), t);
            Components::Transform::SetRotation(e, JSONHelper::FrmJsonf3(tJ["Rotation"]), t);
            Components::Transform::SetScale(e, JSONHelper::FrmJsonf3(tJ["Scale"]), t);
            t.isStatic = JSONHelper::FrmJsonb(tJ["Static"]);
        }
        if (eJ["Components"].contains("MeshInstance")) {
            nlohmann::json mJ = eJ["Components"]["MeshInstance"];
            auto meshComp = AddComponent<MeshInstanceComponent>(e);
            meshComp.modelID = mJ["MeshID"];
            meshComp.materialID = mJ["MaterialID"];
        }
        
    }

    // relations
    for (size_t i = 0; i < entCount; i++)
    {
        nlohmann::json eJ = json["Entities"][std::to_string(i)];
        
        SetParent(entDeSerMap[eJ["Parent"]], entDeSerMap[i]);
    }
}

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

void EntityManager::SetDirty(entt::entity e, DirtyFlags flags)
{
    GetWorld().emplace_or_replace<FDirty>(e).flags = flags;
}

void EntityManager::DestroyEntity(entt::entity e)
{
    GetWorld().destroy(e);
}

void EntityManager::AddChild(entt::entity parent, entt::entity child)
{
    if (!IsValid(child)) {
        return;
    }
    if (!IsValid(parent)) {
        return;
    }
    IdentityComponent& parentId = GetComponent<IdentityComponent>(parent);
    IdentityComponent& childId = GetComponent<IdentityComponent>(child);
    
    if (IsValid(childId.parent)) {
        if (childId.parent == parent) {
            return;
        }
        IdentityComponent& oldParentId = GetComponent<IdentityComponent>(childId.parent);
        auto index = std::find(oldParentId.childs.begin(), oldParentId.childs.end(), child);
        if (index != oldParentId.childs.end()) {
            oldParentId.childs.erase(index);
        }
    }

    parentId.childs.push_back(child);
    childId.parent = parent;
}

void EntityManager::SetParent(entt::entity parent, entt::entity child)
{
    if (IsValid(parent)) {
        AddChild(parent, child);
    }
}

void Components::Transform::SetPosition(entt::entity e, const float3& pos, TransformComponent& t)
{
    if (t.localPos.x == pos.x && t.localPos.y == pos.y && t.localPos.z == pos.z) {
        return;
    }
    t.localPos = pos;
    EntityManager::SetDirty(e);
}

void Components::Transform::SetRotation(entt::entity e, const float3& rot, TransformComponent& t)
{
    if (t.localRot.x == rot.x && t.localRot.y == rot.y && t.localRot.z == rot.z) {
        return;
    }
    t.localRot = rot;
    EntityManager::SetDirty(e);
}

void Components::Transform::SetScale(entt::entity e, const float3& scale, TransformComponent& t)
{
    if (t.localScale.x == scale.x && t.localScale.y == scale.y && t.localScale.z == scale.z) {
        return;
    }
    t.localScale = scale;
    EntityManager::SetDirty(e);
}
/*
float3 Components::RigidBody::GetVelocity(entt::entity e, RigidBodyComponent& rb)
{
    physx::PxRigidBody* rigidBody = dynamic_cast<physx::PxRigidBody*>(rb.body);
    if (!rigidBody) {
        return float3((float)INVALID_ASSET_ID);
    }
    physx::PxVec3 ret = rigidBody->getLinearVelocity();
    return { ret.x, ret.y, ret.z };
}

float3 Components::RigidBody::GetAngularVelocity(entt::entity e, RigidBodyComponent& rb)
{
    physx::PxRigidBody* rigidBody = dynamic_cast<physx::PxRigidBody*>(rb.body);
    if (!rigidBody) {
        return float3((float)INVALID_ASSET_ID);
    }
    physx::PxVec3 ret = rigidBody->getAngularVelocity();
    return { ret.x, ret.y, ret.z };
}

float3 Components::RigidBody::GetCenterOfMass(entt::entity e, RigidBodyComponent& rb)
{
        
    return float3((float)INVALID_ASSET_ID);
}

float Components::RigidBody::GetMass(entt::entity e, RigidBodyComponent& rb)
{
    physx::PxRigidBody* rigidBody = dynamic_cast<physx::PxRigidBody*>(rb.body);
    if (!rigidBody) {
        return (float)INVALID_ASSET_ID;
    }
    return rigidBody->getMass();
}

void Components::RigidBody::SetVelocity(entt::entity e, RigidBodyComponent& rb, const float3& velocity)
{
    physx::PxRigidDynamic* rigidBody = dynamic_cast<physx::PxRigidDynamic*>(rb.body);
    if (!rigidBody) {
        return;
    }
    rigidBody->setLinearVelocity(*(physx::PxVec3*)&velocity);
}

void Components::RigidBody::SetAngularVelocity(entt::entity e, RigidBodyComponent& rb, const float3& angularVelocity)
{
    physx::PxRigidDynamic* rigidBody = dynamic_cast<physx::PxRigidDynamic*>(rb.body);
    if (!rigidBody) {
        return;
    }
    rigidBody->setAngularVelocity(*(physx::PxVec3*)&angularVelocity);
}

void Components::RigidBody::AddForce(entt::entity e, RigidBodyComponent& rb, const float3& force, physx::PxForceMode::Enum forceMode)
{
    physx::PxRigidDynamic* rigidBody = dynamic_cast<physx::PxRigidDynamic*>(rb.body);
    if (!rigidBody) {
        return;
    }
    rigidBody->addForce(*(physx::PxVec3*)&force, forceMode);
}

void Components::RigidBody::AddTorque(entt::entity e, RigidBodyComponent& rb, const float3& torque, physx::PxForceMode::Enum forceMode)
{
    physx::PxRigidDynamic* rigidBody = dynamic_cast<physx::PxRigidDynamic*>(rb.body);
    if (!rigidBody) {
        return;
    }
    rigidBody->addTorque(*(physx::PxVec3*)&torque, forceMode);
}*/


/*
Entity::Entity(std::string name)
{
    mID = EntityManager::CreateEntity(name);
}

Entity::Entity(std::string name, Entity& parent)
{
    mID = EntityManager::CreateEntity(name, parent.GetID());
}

Entity::Entity(std::string name, AssetID asset)
{
    mID = EntityManager::CreateEntity(name, asset);
}

Entity::Entity(std::string name, AssetID asset, Entity& parent)
{
    mID = EntityManager::CreateEntity(name, asset, parent.GetID());
}
*/
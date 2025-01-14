#include "SceneManager.h"
#include "ECS/ECSManager.h"
#include "Log/Log.h"

AScene* curSceneAsset{nullptr};
AssetID curSceneAssetID{INVALID_ASSET_ID};

RenderScene* renderScene{nullptr};

#ifdef PHYSX_PHYSICS
PhyisicsWorld* physicsWorld{ nullptr };
#endif // PHYSX_PHYSICS

AScene* SceneManager::GetSceneAsset()
{
	if (curSceneAssetID == INVALID_ASSET_ID) {
		curSceneAsset = nullptr;
		return nullptr;
	}
	return curSceneAsset;
}

RenderScene* SceneManager::GetRenderScene()
{
	return renderScene;
}

AssetID SceneManager::GetSceneID()
{
	return curSceneAssetID;
}

void SceneManager::UseScene(AssetID id, bool save)
{
	if (id == curSceneAssetID) {
		Log::Message("SceneManager", "Scene already taken " + std::to_string(id));
		return;
	}
	AScene* scene = dynamic_cast<AScene*>(AssetManager::GetAsset(id));
	if (!scene) {
		Log::Error("SceneManager", "Scene not valid " + std::to_string(id));
		return;
	}

	if (save) {
		AssetManager::Save();
	}

	curSceneAssetID = id;
	curSceneAsset = scene;

	delete renderScene;

	//std::cout << scene->worldData["ECS"] << std::endl;

	EntityManager::Init(scene->worldData["ECS"], false);
	renderScene = new RenderScene(id);
#ifdef PHYSX_PHYSICS
	physicsWorld = new PhyisicsWorld(id);
#endif // PHYSX_PHYSICS
}

#ifdef PHYSX_PHYSICS
PhyisicsWorld* SceneManager::GetPhysicsWorld()
{
	return physicsWorld;
}
#endif // PHYSX_PHYSICS

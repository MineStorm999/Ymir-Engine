#include "SceneManager.h"
#include "Log/Log.h"

AScene* curSceneAsset{nullptr};
AssetID curSceneAssetID{INVALID_ASSET_ID};

RenderScene* renderScene{nullptr};

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

void SceneManager::UseScene(AssetID id)
{
	if (id == curSceneAssetID) {
		Log::Message("SceneManager", "Scene already taken " + std::to_string(id));
		return;
	}
	AssetBase* assetB = AssetManager::GetAsset(id);
	if (!assetB) {
		Log::Error("SceneManager", "Scene not valid " + std::to_string(id));
		return;
	}
	AScene* scene = dynamic_cast<AScene*>(assetB);
	if (!scene) {
		Log::Error("SceneManager", "Scene not valid " + std::to_string(id));
		return;
	}

	curSceneAssetID = id;
	curSceneAsset = scene;

	delete renderScene;

	renderScene = new RenderScene(id);
}

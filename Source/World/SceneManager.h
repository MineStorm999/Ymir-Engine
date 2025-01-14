#pragma once

#include "Assets/AssetManager.h"
#include "RenderScene.h"
#include "PhysicsWorld.h"
class SceneManager {
public:
	static AScene* GetSceneAsset();
	static RenderScene* GetRenderScene();
	static AssetID GetSceneID();
	static void UseScene(AssetID id, bool save = true);
#ifdef PHYSX_PHYSICS
	static PhyisicsWorld* GetPhysicsWorld();
#endif // PHYSX_PHYSICS
};
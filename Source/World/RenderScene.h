#pragma once

#include "Assets/assetTypes.h"
#include "Rendering/RenderTypes.h"

#include "NRIFramework.h"
#include "../Shaders/SceneViewerBindlessStructs.h"
class RenderScene {
public:
	RenderScene(AssetID id);


	std::unordered_map<AssetID, RenderID> renderIds;

	RenderID GetRenderID(AssetID id);

	void Add(AssetID id); // temporary
	void Remove(AssetID id); // temporary

	void Load();
	void UnLoad();

	bool rel;
	bool relMeshes;
	bool relTextures;
	bool relMaterials;

	uint32_t modelCount;
public:
	std::vector<unsigned int> indicesCPU;		// done
	std::vector<utils::Vertex> verticesCPU;		// done

	std::vector<MeshData> meshesCPU;			// done
	std::vector<utils::Texture*> texturesCPU;
	std::vector<MaterialData> materialsCPU;


	void LoadMeshes();
	void LoadTextures();
	void LoadMaterials();
private:

	bool IsLoaded(AssetID id) {
		return renderIds.find(id) != renderIds.end();
	}

	void LoadTexture(AssetID texID);
	void LoadMaterial(AssetID matID);

	RenderID lastMaterialRenderID = 0;
	RenderID lastTextureRenderID = 0;

	AScene* sceneAsset;
	AssetID sceneAssetID;
};
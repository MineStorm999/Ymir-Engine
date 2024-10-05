#pragma once

#include "Assets/assetTypes.h"
#include "Rendering/RenderTypes.h"

#include "NRIFramework.h"
#include "../Shaders/SceneViewerBindlessStructs.h"
class RenderScene {
public:
	RenderScene(AssetID id);


	std::unordered_map<AssetID, RenderID> renderIds;



	void Add(AssetID id); // temporary
	void Remove(AssetID id); // temporary

	void Load();
	void UnLoad();

	bool rel;
	bool relMeshes;
	bool relTextures;
	bool relMaterials;

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

	AScene* sceneAsset;
	AssetID sceneAssetID;
};
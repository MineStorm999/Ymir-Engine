#include "RenderScene.h"
#include "Assets/AssetManager.h"
#include "Log/Log.h"
RenderScene::RenderScene(AssetID id)
{
	if (id == INVALID_ASSET_ID) {
		return;
	}
	AssetBase* asset = AssetManager::GetAsset(id);
	if (!asset) {
		return;
	}
	sceneAsset = dynamic_cast<AScene*>(asset);
	if (sceneAsset) {
		return;
	}
	sceneAssetID = id;
	rel = true;
}

void RenderScene::Add(AssetID id)
{
	Log::Message("RenderScene", "Adding " + std::to_string(id));
	if (!AssetManager::IsValid(id)) {
		return;
	}

	AssetBase* asset = AssetManager::GetAsset(id);

	rel = true;
	if (asset->type == AssetType::Model) {
		if (std::find(sceneAsset->usedMeshes.begin(), sceneAsset->usedMeshes.end(), id) == sceneAsset->usedMeshes.end()) {
			sceneAsset->usedMeshes.push_back(id);
			relMeshes = true;
		}
	}

	if (asset->type == AssetType::Material) {
		if (std::find(sceneAsset->usedMaterials.begin(), sceneAsset->usedMaterials.end(), id) == sceneAsset->usedMaterials.end()) {
			sceneAsset->usedMaterials.push_back(id);
			relMaterials = true;
		}
	}

	if (asset->type == AssetType::Texture) {
		if (std::find(sceneAsset->usedTextures.begin(), sceneAsset->usedTextures.end(), id) == sceneAsset->usedTextures.end()) {
			sceneAsset->usedTextures.push_back(id);
			relTextures = true;
		}
	}

	
}

void RenderScene::Remove(AssetID id)
{
	if (!AssetManager::IsValid(id)) {
		return;
	}

	AssetBase* asset = AssetManager::GetAsset(id);
	rel = true;
	if (asset->type == AssetType::Model) {
		if (std::find(sceneAsset->usedMeshes.begin(), sceneAsset->usedMeshes.end(), id) != sceneAsset->usedMeshes.end()) {
			sceneAsset->usedMeshes.erase(std::find(sceneAsset->usedMeshes.begin(), sceneAsset->usedMeshes.end(), id));
			relMeshes = true;
		}
	}

	if (asset->type == AssetType::Material) {
		if (std::find(sceneAsset->usedMaterials.begin(), sceneAsset->usedMaterials.end(), id) != sceneAsset->usedMaterials.end()) {
			sceneAsset->usedMaterials.erase(std::find(sceneAsset->usedMaterials.begin(), sceneAsset->usedMaterials.end(), id));
			relMaterials = true;
		}
	}

	if (asset->type == AssetType::Texture) {
		if (std::find(sceneAsset->usedTextures.begin(), sceneAsset->usedTextures.end(), id) != sceneAsset->usedTextures.end()) {
			sceneAsset->usedTextures.erase(std::find(sceneAsset->usedTextures.begin(), sceneAsset->usedTextures.end(), id));
			relTextures = true;
		}
	}
}

void RenderScene::Load()
{
	UnLoad(); // temporary
	
	LoadTextures();
	LoadMaterials();
	LoadMeshes();
}

void RenderScene::UnLoad()
{
	for (auto& tex : texturesCPU)
		delete tex;

	texturesCPU.clear();
	texturesCPU.shrink_to_fit();

	indicesCPU.clear();
	indicesCPU.shrink_to_fit();
	verticesCPU.clear();
	verticesCPU.shrink_to_fit();
	materialsCPU.clear();
	materialsCPU.shrink_to_fit();
	meshesCPU.clear();
	meshesCPU.shrink_to_fit();
}

void RenderScene::LoadMeshes()
{
	Log::Message("RenderScene", "Load Meshes 0/" + std::to_string(sceneAsset->usedMeshes.size()));
	RenderID lastRenderID = 0;
	uint32_t i = 0;
	for (AssetID id : sceneAsset->usedMeshes)
	{
		AModel* m = dynamic_cast<AModel*>(AssetManager::GetAsset(id));
		if (!m) {
			sceneAsset->usedMeshes.erase(std::find(sceneAsset->usedMeshes.begin(), sceneAsset->usedMeshes.end(), id));
			continue;
		}


		uint32_t vertexOffSet = verticesCPU.size();
		uint32_t indexOffSet = indicesCPU.size();

		bool result = m->Load(indicesCPU, verticesCPU);
		if (!result) {
			sceneAsset->usedMeshes.erase(std::find(sceneAsset->usedMeshes.begin(), sceneAsset->usedMeshes.end(), id));
			continue;
		}

		meshesCPU.reserve(meshesCPU.size() + m->lods.size());
		

		for (LOD& lod : m->lods)
		{
			MeshData data;
			// vertices
			data.vtxOffset = vertexOffSet;
			data.vtxCount = m->vertCount;

			// indices
			data.idxOffset = indexOffSet;
			data.idxCount = lod.lenght;

			indexOffSet += lod.lenght;

			meshesCPU.push_back(data);
		}

		renderIds[id] = lastRenderID;
		lastRenderID++;

		Log::Message("RenderScene", "Load Meshes " + std::to_string((i++ + 1)) + "/" + std::to_string(sceneAsset->usedMeshes.size()));
	}
	modelCount = meshesCPU.size();
}

void RenderScene::LoadTextures()
{
	RenderID lastRenderID = 0;

	texturesCPU.reserve(sceneAsset->usedTextures.size());
	for (AssetID id : sceneAsset->usedTextures)
	{
		ATexture* texAsset = dynamic_cast<ATexture*>(AssetManager::GetAsset(id));
		if (!texAsset) {
			continue;
		}

		// temporary
		utils::Texture* tex = texAsset->GetTexture();
		if (!tex) {
			continue;
		}
		texturesCPU.push_back(tex);

		renderIds[id] = lastRenderID;
		lastRenderID++;
	}
	texturesCPU.shrink_to_fit();
}

void RenderScene::LoadMaterials()
{
	RenderID lastRenderID = 0;

	materialsCPU.reserve(sceneAsset->usedMaterials.size());
	for (AssetID id : sceneAsset->usedTextures)
	{
		AMaterial* matAsset = dynamic_cast<AMaterial*>(AssetManager::GetAsset(id));
		if (!matAsset) {
			continue;
		}

		MaterialData data;
		data.baseColorAndMetallic = matAsset->baseColorAndMetallic;
		data.emissiveColorAndRoughness = matAsset->emissiveColorAndRoughness;

		data.baseColorTexIndex = renderIds[matAsset->baseColorTex];
		data.normalTexIndex = renderIds[matAsset->normalTex];
		data.emissiveTexIndex = renderIds[matAsset->emissiveTex];
		data.roughnessMetalnessTexIndex = renderIds[matAsset->roughnessMetalnessTex];

		materialsCPU.push_back(data);

		renderIds[id] = lastRenderID;
		lastRenderID++;
	}
}

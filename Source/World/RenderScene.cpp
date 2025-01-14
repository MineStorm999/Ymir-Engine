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

RenderID RenderScene::GetRenderID(AssetID id)
{
	if (!IsLoaded(id)) {
		return INVALID_RENDER_ID;
	}
	return renderIds[id];
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
	AssetManager::Save();
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


		size_t vertexOffSet = verticesCPU.size();
		size_t indexOffSet = indicesCPU.size();

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
			data.vtxOffset = (uint32_t)vertexOffSet;
			data.vtxCount = m->vertCount;

			// indices
			data.idxOffset = (uint32_t)indexOffSet;
			data.idxCount = lod.lenght;

			indexOffSet += lod.lenght;

			meshesCPU.push_back(data);
		}

		renderIds[id] = lastRenderID;
		lastRenderID++;

		Log::Message("RenderScene", "Load Meshes " + std::to_string((i++ + 1)) + "/" + std::to_string(sceneAsset->usedMeshes.size()));
	}
	modelCount = (uint32_t)meshesCPU.size();
}

void RenderScene::LoadTextures()
{
	lastTextureRenderID = 0;

	texturesCPU.reserve(sceneAsset->usedTextures.size());
	for (AssetID id : sceneAsset->usedTextures)
	{
		LoadTexture(id);
	}
	texturesCPU.shrink_to_fit();
}

void RenderScene::LoadMaterials()
{
	lastMaterialRenderID = 0;

	materialsCPU.reserve(sceneAsset->usedMaterials.size());
	for (AssetID id : sceneAsset->usedMaterials)
	{
		LoadMaterial(id);
	}
}

void RenderScene::LoadTexture(AssetID texID)
{
	ATexture* texAsset = dynamic_cast<ATexture*>(AssetManager::GetAsset(texID));
	if (!texAsset) {
		return;
	}
	Log::Message("RenderScene", std::string("Load Texture: ") + texAsset->name);
	// temporary
	utils::Texture* tex = texAsset->GetTexture();
	if (!tex) {
		Log::Error("RenderScene", std::string("Load Texture: ") + std::to_string(texID) + std::string(" Failed"));
		return;
	}
	texturesCPU.push_back(tex);


	renderIds[texID] = lastTextureRenderID;
	lastTextureRenderID++;
}

void RenderScene::LoadMaterial(AssetID matID)
{
	AMaterial* matAsset = dynamic_cast<AMaterial*>(AssetManager::GetAsset(matID));
	if (!matAsset) {
		Log::Error("RenderScene", std::string("Load Material: ") + std::to_string(matID) + std::string(" Failed"));
		return;
	}
	Log::Message("RenderScene", std::string("Load Material: ") + matAsset->name);
	MaterialData data;
	data.baseColorAndMetallic = matAsset->baseColorAndMetallic;
	data.emissiveColorAndRoughness = matAsset->emissiveColorAndRoughness;

	if (!IsLoaded(matAsset->baseColorTex)) {
		LoadTexture(matAsset->baseColorTex);
		Add(matAsset->baseColorTex);
	}
	if (!IsLoaded(matAsset->normalTex)) {
		LoadTexture(matAsset->normalTex);
		Add(matAsset->normalTex);
	}
	if (!IsLoaded(matAsset->emissiveTex)) {
		LoadTexture(matAsset->emissiveTex);
		Add(matAsset->emissiveTex);
	}
	if (!IsLoaded(matAsset->roughnessMetalnessTex)) {
		LoadTexture(matAsset->roughnessMetalnessTex);
		Add(matAsset->roughnessMetalnessTex);
	}

	data.baseColorTexIndex = renderIds[matAsset->baseColorTex];
	data.normalTexIndex = renderIds[matAsset->normalTex];
	data.emissiveTexIndex = renderIds[matAsset->emissiveTex];
	data.roughnessMetalnessTexIndex = renderIds[matAsset->roughnessMetalnessTex];

	materialsCPU.push_back(data);

	renderIds[matID] = lastMaterialRenderID;
	lastMaterialRenderID++;
}

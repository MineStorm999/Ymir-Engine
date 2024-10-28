#include "assetTypes.h"
#include "AssetManager.h"
#include "meshoptimizer.h"

#include "AssetHelper.h"

#include "World/SceneManager.h"

bool AModel::Load(std::vector<uint32_t>& indexes, std::vector<utils::Vertex>& vertices)
{
	
	std::string actualPath = GetActualPath();

	if (actualPath.find_last_of(".") == std::string::npos) {
		return false;
	}
	if (path.find_last_of(std::string(".") + ASSET_SHORT) == std::string::npos) {
		return false;
	}

	FILE* f = fopen(actualPath.c_str(), "rb");
	if (!f) {
		return false;
	}

	fseek(f, 0, SEEK_SET);
	uint8_t head[2];

	if (fread(head, 2, 1, f) == 0) {
		return false;
	}
	if (head[0] != A_DISC_VERI0) {
		return false;
	}
	if (head[1] != A_DISC_VERI1) {
		return false;
	}

	uint8_t flags = 0;
	fread(&flags, 1, 1, f);

	
	fseek(f, off, SEEK_SET);
	
	

	uint32_t offI = indexes.size();
	indexes.resize(indexCount + indexes.size());

	uint32_t offV = vertices.size();
	vertices.resize(vertCount + vertices.size());

	
	if (flags == 0) {
		if (1 != fread(&indexes[offI], iBuffLenght, 1, file)) {
			for (size_t i = 0; i < indexCount; i++)
			{
				indexes.pop_back();
			}
			for (size_t i = 0; i < vertCount; i++)
			{
				vertices.pop_back();
			}
			fclose(f);
			return false;
		}
		if (1 != fread(&vertices[offV], lenght - iBuffLenght, 1, file)) {
			for (size_t i = 0; i < indexCount; i++)
			{
				indexes.pop_back();
			}
			for (size_t i = 0; i < vertCount; i++)
			{
				vertices.pop_back();
			}
			fclose(f);
			return false;
		}
		fclose(f);
		return true;
	}

	unsigned char* idxRaw;
	unsigned char* vertRaw;
	idxRaw = (unsigned char*)malloc(iBuffLenght);
	vertRaw = (unsigned char*)malloc(lenght - iBuffLenght);
	fseek(f, off, SEEK_SET);
	if (iBuffLenght != fread(idxRaw, 1, iBuffLenght, f)) {
		for (size_t i = 0; i < indexCount; i++)
		{
			indexes.pop_back();
		}
		for (size_t i = 0; i < vertCount; i++)
		{
			vertices.pop_back();
		}
		free(idxRaw);
		free(vertRaw);
		fclose(f);
		return false;
	}

	if (lenght - iBuffLenght != fread(vertRaw, 1, lenght - iBuffLenght, f)) {
		for (size_t i = 0; i < indexCount; i++)
		{
			indexes.pop_back();
		}
		for (size_t i = 0; i < vertCount; i++)
		{
			vertices.pop_back();
		}
		free(idxRaw);
		free(vertRaw);
		fclose(f);
		return false;
	}

	int resib = meshopt_decodeIndexBuffer(&indexes[offI], indexCount, idxRaw, iBuffLenght);
	free(idxRaw);
	idxRaw = nullptr;
	int resvb = meshopt_decodeVertexBuffer(&vertices[offV], vertCount, sizeof(utils::Vertex), vertRaw, lenght - iBuffLenght);
	free(vertRaw);
	vertRaw = nullptr;

	fclose(f);
	return true;
}

nlohmann::json AModel::Save()
{
	nlohmann::json j = AssetBase::Save();
	
	j["LODCount"] = lods.size();
	j["IndexCount"] = indexCount;
	j["VertexCount"] = vertCount;
	j["IBuffLenght"] = iBuffLenght;

	j["DefaultMat"] = DefaultMaterialID;
	
	for (uint32_t i = 0; i < lods.size(); i++)
	{
		nlohmann::json lod;
		lod["IndexOffset"] = lods[i].indexOffset;
		lod["IndexCount"] = lods[i].lenght;
		lod["Distance"] = lods[i].distance;
		j["Lods"][std::to_string(i)] = lod;
	}

	return j;
}

RenderID AModel::GetRenderID(float disFromCam, AssetID id)
{
	RenderScene* rScene = SceneManager::GetRenderScene();
	if (!rScene) {
		return INVALID_RENDER_ID;
	}

	RenderID ret = rScene->renderIds[id];
	return ret;
	/*if (ret == INVALID_RENDER_ID) {
		return INVALID_RENDER_ID;
	}

	
	for (LOD& lod : lods)
	{
		if (lod.distance > disFromCam) {
			return ret;
		}
		ret++;
	}
	return rScene->renderIds[id];*/
}

AssetID AssetUtils::GetAssetIDFromImported(std::filesystem::path path)
{
	AssetID ret = 0xffffffff;
	if (path.string().find_last_of(".") == std::string::npos) {
		//Log::Message("AssetManager", "Cur Asset is invalid, continuing..." + std::to_string(curAsset));
		return ret;
	}
	if (path.string().find_last_of(std::string(".") + ASSET_SHORT) == std::string::npos) {
		return ret;
	}
	
	FILE* file = fopen(path.string().c_str(), "rb");
	if (!file) {
		return ret;
	}

	uint8_t head[2];
	fseek(file, 0, SEEK_SET);
	if (fread(&head[0], 2, 1, file) == 0) {
		fclose(file);
		return ret;
	}
	if (head[0] != A_DISC_VERI0) {
		fclose(file);
		return ret;
	}
	if (head[1] != A_DISC_VERI1) {
		fclose(file);
		return ret;
	}

	fseek(file, 3, SEEK_SET);
	fread(&ret, 4, 1, file);
	fclose(file);
	return ret;
}

nlohmann::json AssetBase::Save()
{
	nlohmann::json j;
	j["Type"] = std::to_string(type);
	j["Name"] = name;
	j["Path"] = path;
	j["OriginalPath"] = originalPath;
	j["Offset"] = off;
	j["Lenght"] = lenght;
	return j;
}

std::string AssetBase::GetActualPath()
{
	std::filesystem::path pt0(path);
	pt0 /= (name + '.' + ASSET_SHORT);
	return pt0.string();
}


nlohmann::json AScene::Save()
{
	nlohmann::json j = AssetBase::Save();

	j["MeshCount"] = usedMeshes.size();
	for (size_t i = 0; i < usedMeshes.size(); i++)
	{
		j["Meshes"][std::to_string(i)] = usedMeshes[i];
	}
	j["MaterialCount"] = usedMaterials.size();
	for (size_t i = 0; i < usedMaterials.size(); i++)
	{
		j["Materials"][std::to_string(i)] = usedMaterials[i];
	}
	j["TextureCount"] = usedTextures.size();
	for (size_t i = 0; i < usedTextures.size(); i++)
	{
		j["Textures"][std::to_string(i)] = usedTextures[i];
	}
	return j;
}

nlohmann::json ATexture::Save()
{
	nlohmann::json j = AssetBase::Save();
	return j;
}

utils::Texture* ATexture::GetTexture()
{
	utils::Texture* texture = new utils::Texture();
	std::filesystem::path pt(path);
	pt /= (name + ".ytex");

	if (utils::LoadTexture(pt.string(), *texture)) {
		return texture;
	}
	
	delete texture;
	return nullptr;
}

nlohmann::json AMaterial::Save()
{
	nlohmann::json j = AssetBase::Save();

	j["BaseColorAndMetallic"] = JSONHelper::TJson(baseColorAndMetallic);
	j["EmissiveColorAndRoughness"] = JSONHelper::TJson(emissiveColorAndRoughness);
	j["AlphaMode"] = alphaMode;

	j["BaseTexture"] = baseColorTex;
	j["RoughnessTexture"] = roughnessMetalnessTex;
	j["NormalTexture"] = normalTex;
	j["EmissiveTexture"] = emissiveTex;

	return j;
}

nlohmann::json AMeshCollider::Save()
{
	return nlohmann::json();
}

bool AMeshCollider::Load(std::vector<uint8_t>& indexes, std::vector<float3>& vertices, std::vector<OdCollider>& colls, std::vector<OdCluster>& cluster)
{
	return false;
}

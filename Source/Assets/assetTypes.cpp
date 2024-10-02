#include "assetTypes.h"
#include "AssetManager.h"
#include "meshoptimizer.h"

std::vector<uint8_t> AModel::Load()
{
	std::vector<unsigned char> ret;
	std::string actualPath = GetActualPath();

	if (actualPath.find_last_of(".") == std::string::npos) {
		return ret;
	}
	if (!(actualPath.substr(path.find_last_of(".")) == ASSET_SHORT || actualPath.substr(actualPath.find_last_of(".")) == '.' + ASSET_SHORT)) {
		return ret;
	}

	if (!file) {
		file = fopen(actualPath.c_str(), "rb");
		if (!file) {
			return ret;
		}
	}

	fseek(file, 0, SEEK_SET);
	uint8_t head[2];

	if (fread(head, 2, 1, file) == 0) {
		return ret;
	}
	if (head[0] != A_DISC_VERI0) {
		return ret;
	}
	if (head[1] != A_DISC_VERI1) {
		return ret;
	}

	uint8_t flags = 0;
	fread(&flags, 1, 1, file);

	std::vector<uint8_t> data(lenght);
	fseek(file, off, SEEK_SET);
	
	fread(data.data(), lenght, 1, file);

	fclose(file);
	if (flags == 0) {
		return ret;
	}

	std::vector<uint8_t> outData(indexCount * sizeof(uint32_t) + (vertCount * sizeof(utils::Vertex)));

	int resib = meshopt_decodeIndexBuffer((uint32_t*)&outData[0], indexCount, &data[0], iBuffLenght);
	int resvb = meshopt_decodeVertexBuffer(&outData[indexCount], vertCount, sizeof(utils::Vertex), &data[iBuffLenght], data.size() - iBuffLenght);
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

AssetID AssetUtils::GetAssetIDFromImported(std::filesystem::path path)
{
	AssetID ret = 0xffffffff;
	if (path.string().find_last_of(".") == std::string::npos) {
		//Log::Message("AssetManager", "Cur Asset is invalid, continuing..." + std::to_string(curAsset));
		return ret;
	}
	if (path.string().find_last_of('.' + ASSET_SHORT) == std::string::npos) {
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

void AScene::Add(AssetID id)
{
	if (!AssetManager::IsValid(id)) {
		return;
	}
	switch (AssetManager::GetAsset(id)->type)
	{
	case AssetType::Model:
		AddModel(id);
	case AssetType::Texture:
		AddTexture(id);
	case AssetType::Material:
		AddMaterial(id);
	case AssetType::Audio:
		break;
	case AssetType::Scene:
		break;
	default:
		break;
	}
}

void AScene::AddModel(AssetID id)
{

}

void AScene::AddMaterial(AssetID id)
{

}

void AScene::AddTexture(AssetID id)
{

}

nlohmann::json AScene::Save()
{
	nlohmann::json j = AssetBase::Save();
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
	return j;
}

#include "assetTypes.h"

std::vector<uint8_t> AModel::Load()
{
	std::vector<uint8_t> ret;
	if (path.find_last_of(".") == std::string::npos) {
		return ret;
	}
	if (!(path.substr(path.find_last_of(".")) == ASSET_SHORT || path.substr(path.find_last_of(".")) == '.' + ASSET_SHORT)) {
		return ret;
	}

	if (!file) {
		file = fopen(path.c_str(), "rb");
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


	if (flags == 1) {
		return ret;
	}
}

nlohmann::json AModel::Save()
{
	nlohmann::json j = AssetBase::Save();
	
	j["LODCount"] = lods.size();
	j["IndexCount"] = indexCount;
	j["VertexCount"] = vertCount;
	j["DefaultMat"] = DefaultMaterialID;
	
	for (uint32_t i = 0; i < lods.size(); i++)
	{
		nlohmann::json lod;
		lod["MeshID"] = lods[i].mesh;
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

	if (fread(head, 2, 1, file) == 0) {
		return ret;
	}
	if (head[0] != A_DISC_VERI0) {
		return ret;
	}
	if (head[1] != A_DISC_VERI1) {
		return ret;
	}

	fseek(file, 3, SEEK_SET);
	fread(&ret, 4, 1, file);

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

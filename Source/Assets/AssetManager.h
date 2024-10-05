#pragma once


#include <string>
#include "../JSON/json.hpp"
#include <memory>

#include "assetTypes.h"

class AssetBase;

using AssetMap = std::unordered_map<AssetID, AssetBase*>;


class AssetManager {
public:
	static void Init();
	//static AssetBase* Import(std::string path, AssetID name);
	static void Save();

	static void Show();

	static AssetMap& GetMap();

	static bool IsValid(AssetID id);

	static AssetBase* GetAsset(AssetID name);

	static void AddFolder(std::string name);
	static void Delete(AssetID name);

	static AssetID RegisterAsset(AssetBase* asset);

	//static bool HasAsset(AssetID* id, std::string originalPath);


private:

	static void AssetExplorer();
	static void AssetImporter();
	
	static AssetID CreateDefaultScene();
};
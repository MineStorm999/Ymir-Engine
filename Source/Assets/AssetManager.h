#pragma once


#include <string>
#include "../JSON/json.hpp"
#include <memory>

#include "assetTypes.h"

using AssetMap = std::unordered_map<AssetID, AssetBase*>;

class AssetManager {
public:
	static void Init();
	//static AssetBase* Import(std::string path, AssetID name);
	static void Save();

	static void Show();

	static AssetMap& GetMap();

private:
	static void GoBack();
	static void Open(Node* n);

	static void AssetExplorer();
	static void AssetImporter();
	
	
	static AssetID RegisterAsset(AssetBase* asset);
	static void AddFolder(std::string name);
	static void Delete(AssetID name);

	static nlohmann::json SaveDir(Node* n);
	static Node* LoadDir(nlohmann::json& j, Node* parent);

	static AssetBase* GetAsset(AssetID name);
};
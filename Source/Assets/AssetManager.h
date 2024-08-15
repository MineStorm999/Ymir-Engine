#pragma once


#include <string>
#include "../JSON/json.hpp"
#include <memory>

#include "assetTypes.h"



class AssetManager {
public:
	static void Init();
	static AssetBase* Import(std::string path, std::string name);
	static void Save();

	static void Update();

	

private:
	static void GoBack();
	static void Open(Node* n);

	static void AssetExplorer();
	static void AssetImporter();
	static void AssetInspector();
	static void RegisterAsset(AssetBase* asset, std::string name);
	static void AddFolder(std::string name);


	static AssetBase* GetAsset(std::string name);
};
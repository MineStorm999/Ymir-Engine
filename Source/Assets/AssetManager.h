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

	static AssetBase* GetAsset(std::string name);
};
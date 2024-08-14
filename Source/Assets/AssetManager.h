#pragma once

#include <string>
#include "../JSON/json.hpp"
#include <memory>

enum AssetType
{
	None,
	VModel,
	Texture,
	Material,
	Audio
};

namespace std {

	inline string to_string(AssetType t) {
		switch (t)
		{
		case AssetType::VModel:
			return "VModel";
		case AssetType::Texture:
			return "Texture";
		case AssetType::Material:
			return "Material";
		case AssetType::Audio:
			return "Audio";
		default:
			return "None";
		}
	};

}

class AssetBase {
public:
	AssetType type;
};

class VModel : AssetBase {
public:
	VModel() { type = AssetType::VModel; };

	uint32_t ID{};
	uint32_t DefaultTexture{};
};

class Material : AssetBase
{
public:
	Material() { type = AssetType::Material; };
	uint32_t ID{};
};

class AssetManager {
public:
	static void Init();

	static AssetBase* GetAsset(std::string name);
};
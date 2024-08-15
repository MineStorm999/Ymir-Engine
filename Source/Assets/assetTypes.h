#pragma once

#include <memory>
#include "../Rendering/VirtualGeometry/MeshletStructs.h"

struct MeshletDesc;

enum AssetType
{
	None,
	VirtualModel,
	Texture,
	Material,
	Audio
};



namespace std {

	inline string to_string(AssetType t) {
		switch (t)
		{
		case AssetType::VirtualModel:
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

inline AssetType to_type(std::string str) {
	if (std::strcmp(str.c_str(), "VModel"))
		return AssetType::VirtualModel;
	if (std::strcmp(str.c_str(), "Texture"))
		return AssetType::Texture;
	if (std::strcmp(str.c_str(), "Material"))
		return AssetType::Material;
	if (std::strcmp(str.c_str(), "Audio"))
		return AssetType::Audio;

	return AssetType::None;
}

class AssetBase {
public:
	AssetType type;

	virtual void V() {};
};


class Material : public AssetBase
{
public:
	Material() { type = AssetType::Material; };
	uint32_t ID{};
};

class VModel : public AssetBase {
public:
	VModel();

	uint32_t ID{};
	uint32_t DefaultTexture{};
	// std::string matName;																		todo future

	std::vector<struct MeshletDesc> meshlets;
};
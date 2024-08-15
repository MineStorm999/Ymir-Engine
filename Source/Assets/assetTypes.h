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


struct Node
{
	Node(std::string n, Node* p) {
		parent = p;
		vName = n;
		realName = "--Directory--";
		if (parent) {
			parent->childs.push_back(this);
		}
		type = AssetType::None;
	};
	Node(std::string n, Node* p, std::string rN, AssetType t) {
		parent = p;
		vName = n;

		realName = rN;

		if (parent) {
			parent->childs.push_back(this);
		}

		type = t;
	};
	std::string vName;

	Node* parent;
	std::vector<Node*> childs;

	AssetType type;

	std::string realName;
};
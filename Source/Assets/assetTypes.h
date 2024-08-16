#pragma once

#include <memory>
#include "../Rendering/VirtualGeometry/MeshletStructs.h"
#include "../ECS/components.h"


struct MeshletDesc;
using AssetID = uint32_t;

enum AssetType
{
	None,
	VirtualModel,
	Texture,
	Material,
	Audio
};

struct Node
{
	Node(std::string n, Node* p) {
		parent = p;
		realName = 0xffffffff;
		if (parent) {
			parent->childs.push_back(this);
		}
		type = AssetType::None;
	};
	Node(std::string n, Node* p, AssetID rN, AssetType t) {
		parent = p;
		vName = n;

		realName = rN;

		if (parent) {
			parent->childs.push_back(this);
		}

		type = t;
	};

	Node* parent;
	std::vector<Node*> childs;

	std::string vName;
	AssetType type;

	AssetID realName;
};

namespace std {

	inline string to_string(AssetType t) {
		switch (t)
		{
		case AssetType::VirtualModel:
			return "Model";
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
	
	if (str.compare("Model")==0) {
		return AssetType::VirtualModel;
	}
	if (str.compare("Texture") == 0){
		return AssetType::Texture; 
	}
	if (str.compare("Material") == 0){
		return AssetType::Material;
	}
	if (str.compare("Audio") == 0){
		return AssetType::Audio;
	}
	return AssetType::None;
}

class AssetBase {
public:
	AssetType type;
	std::string path{};
	std::string name{};

	Node* vRepr = nullptr;

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
	uint32_t DefaultMaterialID{};
	// std::string matName;																		todo future

	std::vector<Transform> defaultTransforms;
	
	std::vector<struct MeshletDesc> meshlets;
};



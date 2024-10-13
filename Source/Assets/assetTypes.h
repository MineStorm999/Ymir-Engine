#pragma once

#include <memory>
#include <filesystem>
#include "NRIFramework.h"
#include "JSON/json.hpp"
#include "Rendering/RenderTypes.h"
//#include "../Rendering/VirtualGeometry/MeshletStructs.h"
//#include "../ECS/components.h"

#define A_DISC_VERI0 6
#define A_DISC_VERI1 66
#define ASSET_SHORT "yasset"
#define SAVE_SHORT "ysave"

#define INVALID_ASSET_ID UINT32_MAX

//struct MeshletDesc;
using AssetID = uint32_t;


/*
struct AssetHeader {
	char type[4];

};
*/

enum class AssetType : uint8_t
{
	None,
	Model,
	Texture,
	Material,
	Audio,
	Scene
};

enum class AssetLoadFlags : uint8_t
{
	None = 0,
	Compressed  = 1
};

class AssetUtils {
public:
	static AssetID GetAssetIDFromImported(std::filesystem::path path);
};

/*
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
};*/

namespace std {

	inline string to_string(AssetType t) {
		switch (t)
		{
		case AssetType::Model:
			return "Model";
		case AssetType::Texture:
			return "Texture";
		case AssetType::Material:
			return "Material";
		case AssetType::Audio:
			return "Audio";
		case AssetType::Scene:
			return "Scene";
		default:
			return "None";
		}
	};

}

inline AssetType to_type(std::string str) {
	
	if (str.compare("Model")==0) {
		return AssetType::Model;
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
	if (str.compare("Scene") == 0) {
		return AssetType::Scene;
	}
	return AssetType::None;
}

class AssetBase {
public:
	AssetType type;
	std::string path{};
	std::string originalPath{};

	std::string name{};

	//Node* vRepr = nullptr;

	FILE* file;
	uint32_t off;
	uint32_t lenght;

	virtual nlohmann::json Save();

	std::string GetActualPath();
};

class AAudio : public AssetBase {
public:
	AAudio() { type = AssetType::Audio; };
};

class ATexture : public AssetBase {
public:
	ATexture() { type = AssetType::Texture; };

	// !!!TODO!!!
	nlohmann::json Save() override;
	

	utils::Texture* GetTexture();
};

class AMaterial : public AssetBase
{
public:
	AMaterial() { type = AssetType::Material; };
	float4 baseColorAndMetallic;
	float4 emissiveColorAndRoughness;
	uint8_t alphaMode;

	AssetID baseColorTex;
	AssetID roughnessMetalnessTex;
	AssetID normalTex;
	AssetID emissiveTex;

	// !!!TODO!!!
	nlohmann::json Save() override;
};

struct LOD {
	float distance;
	uint32_t indexOffset;
	uint32_t lenght;
};

class AModel : public AssetBase {
public:
	AModel() { type = AssetType::Model; };
	AssetID DefaultMaterialID{};
	// std::string matName;														todo future

	//std::vector<Transform> defaultTransforms;

	uint32_t indexCount;
	uint32_t vertCount;
	uint32_t iBuffLenght;

	bool Load(std::vector<uint32_t>& indexes, std::vector<utils::Vertex>& vertices);
	
	nlohmann::json Save() override;

	std::vector<LOD> lods;

	// !!!TODO!!!
	RenderID GetRenderID(float disFromCam, AssetID id);
};

class AScene : public AssetBase{
public:
	AScene() { type = AssetType::Scene; };

	std::vector<AssetID> usedMeshes;
	std::vector<AssetID> usedTextures;
	std::vector<AssetID> usedMaterials;

	// TODO
	nlohmann::json Save() override;
private:
	uint32_t lastRenderID;
};




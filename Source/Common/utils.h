#pragma once
#include <string>
namespace utils {
	enum class CustomFolder : uint8_t {
		VIRTUALMESH,
		VIRTUALTEXTURE,
		VIRTUALMESHDESC,
		SAVEFILES,
		ASSETDIR
	};

	inline std::string GetCFullPath(std::string localPT, CustomFolder folder) {
		std::string path = "";
		if (folder == CustomFolder::VIRTUALMESH)
			path = "IMPORTANT_RESOURCES_DO_NOT_DELETE/VirtualMeshes/";
		else if (folder == CustomFolder::VIRTUALTEXTURE)
			path += "IMPORTANT_RESOURCES_DO_NOT_DELETE/VirtualTextures/";
		else if (folder == CustomFolder::VIRTUALMESHDESC)
			path += "IMPORTANT_RESOURCES_DO_NOT_DELETE/VirtualMeshDescs/";
		else if (folder == CustomFolder::SAVEFILES)
			path = "_SaveFiles/"; 
		else if (folder == CustomFolder::ASSETDIR)
			path = "AssetDirectory/";

		return path + localPT;
	};
}
#pragma once
#include <string>
namespace utils {
	enum class CustomFolder : uint8_t {
		VIRTUALMESH,
		VIRTUALTEXTURE,
		VIRTUALMESHDESC,
		SAVEFILES
	};

	inline std::string GetCFullPath(std::string localPT, CustomFolder folder) {
		std::string path = ""; // it's a symbolic link
		if (folder == CustomFolder::VIRTUALMESH)
			path = "IMPORTANT_RESOURCES_DO_NOT_DELETE/VirtualMeshes/"; // special folder with generated files
		else if (folder == CustomFolder::VIRTUALTEXTURE)
			path += "IMPORTANT_RESOURCES_DO_NOT_DELETE/VirtualTextures/";
		else if (folder == CustomFolder::VIRTUALMESHDESC)
			path += "IMPORTANT_RESOURCES_DO_NOT_DELETE/VirtualMeshDescs/";
		else if (folder == CustomFolder::SAVEFILES)
			path = "_SaveFiles/"; // special folder stored in Git

		return path + localPT;
	};
}
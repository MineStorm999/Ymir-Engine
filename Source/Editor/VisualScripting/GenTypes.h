#pragma once

#include <vector>
#include <string>

class GenTypes {
public:
	static void Init();
	static void RegisterType(std::string name);
	static std::vector<std::string>& GetTypes();
};
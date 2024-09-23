#pragma once

#include <vector>
#include <string>

#include <functional>

class GenTypes {
public:
	static void Init();
	static void RegisterType(std::string name);
	static void RegisterType(std::string name, std::function<std::string(std::string)> editFunc);

	static std::string ShowEditField(std::string type, std::string data);

	static std::vector<std::string>& GetTypes();

};
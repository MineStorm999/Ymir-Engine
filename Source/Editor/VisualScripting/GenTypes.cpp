#include "GenTypes.h"

std::vector<std::string> types;
std::unordered_map<std::string, std::function<std::string(std::string)>> typeEditFunc;

void GenTypes::Init()
{
	RegisterType("bool");
	RegisterType("int");
	RegisterType("float");
	RegisterType("string");
}

void GenTypes::RegisterType(std::string name)
{
	for (std::string& s : types)
	{
		if (s == name) {
			return;
		}
	}
	types.push_back(name);
}

void GenTypes::RegisterType(std::string name, std::function<std::string(std::string)> editFunc)
{
	for (std::string& s : types)
	{
		if (s == name) {
			return;
		}
	}
	types.push_back(name);
	typeEditFunc[name] = editFunc;
}

std::string GenTypes::ShowEditField(std::string type, std::string data)
{
	if (typeEditFunc.find(type) == typeEditFunc.end()) {
		return data;
	}

}

std::vector<std::string>& GenTypes::GetTypes()
{
    return types;
}

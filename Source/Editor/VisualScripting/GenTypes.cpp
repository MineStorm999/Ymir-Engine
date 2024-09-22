#include "GenTypes.h"

std::vector<std::string> types;

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

std::vector<std::string>& GenTypes::GetTypes()
{
    return types;
}

#pragma once

#include <unordered_map>
#include <string>

class json;



struct VariableDesc {
	VariableDesc(std::string name) {
		type = "bool";
		m_name = name;
	};

	VariableDesc(std::string name, std::string n_type) {
		type = n_type;
		m_name = name;
	};

	VariableDesc() {};


	std::string type;

	std::string m_name;

	bool isVector;

	std::string GetData();

	void Rename(std::string after);
	void ChangeType(std::string newType);
};

//									 name					index     type
using Variables = std::unordered_map<std::string, VariableDesc>;
//                                   name                   index     type

struct StructDesc {
	StructDesc(uint32_t n_id, std::string n_name) {
		name = n_name;
		id = n_id;
	}
	StructDesc() {};
	

	std::string name;

	Variables vars;

	uint32_t id{};

	uint32_t nameStart{};
	uint32_t lastBracket{};
	uint32_t firstBracket{};

	std::string data;

	std::string CreateData();

	void AddVar(std::string name);
	void Insert(uint32_t offset, std::string toInsert);
	void Erase(uint32_t start, uint32_t leng);

	void Rename(std::string before, std::string after);
	void ChangeType(std::string varName, std::string newType);
};

class StructEditor
{
public:
	StructEditor() {};
	~StructEditor() {};
	
	static void Init(std::string path);
	static void Save();

	static void CreateStruct(std::string name);

	static void Show();
	static void ShowEdited();

	static StructDesc ReadStruct(uint32_t offset);

};

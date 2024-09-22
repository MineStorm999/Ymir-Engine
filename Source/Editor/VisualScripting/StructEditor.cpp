#include "StructEditor.h"

#include <fstream>

#include <sstream>

#include "Imgui/imgui.h"

#include "../../Log/Log.h"
#include "GenTypes.h"
#include <iostream>

#include "../../JSON/json.hpp"

#include <filesystem>

#include "../../Generated_By_Code/components.h"

std::fstream file;
std::string m_path;
std::string m_genPath = "F:/GameEngines/NRISamples/Source/Generated_By_Code/components.h";

std::string curFile;

std::vector<StructDesc> curStructs;
uint32_t ptr;

VariableDesc VarFromJSON(nlohmann::json& j) {
	VariableDesc  desc(j["Name"], j["Type"]);
	desc.isVector = j["List"];
	return desc;
}

StructDesc StructFromJSON(uint32_t id, nlohmann::json& j) {
	StructDesc desc(id, j["Name"]);
	uint32_t varCount = j["VarCount"];

	for (uint32_t i = 0; i < varCount; i++)
	{
		nlohmann::json& js = j["Vars"][std::to_string(i)];
		desc.vars[js["Name"]] = VarFromJSON(js);
	}

	return desc;
}

void StructEditor::Init(std::string path)
{

	m_path = path;
	GenTypes::Init();
	
	//return;
	
	file.open(path.c_str());
	if (!file.is_open())
		return;

	if (std::filesystem::file_size(path) == 0) {
		return;
	}

	nlohmann::json j;
	j << file;

	uint32_t compCount = j["Count"];

	for (uint32_t i = 0; i < compCount; i++)
	{
		curStructs.push_back(StructFromJSON(i, j["Structs"][std::to_string(i)]));
	}

	Save();

	Gen_Component_Init::Init();
}

nlohmann::json VarToJSON(VariableDesc& varDesc) {
	nlohmann::json j;
	j["Name"] = varDesc.m_name;
	j["Type"] = varDesc.type;
	j["List"] = varDesc.isVector;
	return j;
}

nlohmann::json StructToJSON(StructDesc& desc) {
	nlohmann::json j;

	j["Name"] = desc.name;

	uint32_t i = 0;
	for (auto& it : desc.vars)
	{
		j["Vars"][std::to_string(i)] = VarToJSON(it.second);
		i++;
	}

	j["VarCount"] = i;

	return j;
}





void StructEditor::Save()
{
	// todo 

	nlohmann::json j;
	j["Count"] = curStructs.size();

	std::string file = "";

	// initiation stuff

	file.append("\n#include <Editor/VisualScripting/GenTypes.h>\n");
	file.append("\n#include <vector>\n");
	file.append("\n#include <string>\n\n");
	file.append("\nusing namespace std;\n\n");
	file.append("class Gen_Component_Init\n{\npublic:\n   static void Init() {\n");

	for (StructDesc& desc : curStructs)
	{
		//file.append("   GenTypes::RegisterType(\"" + desc.name + "\");\n");
		file.append("      GenTypes::RegisterType(\"" + desc.name + "*" + "\");\n");
	}
	file.append("   };\n};\n\n");

	for (StructDesc& desc : curStructs)
	{
		file.append("struct " + desc.name + ";\n");
	}
	file.append("\n");

	uint32_t i = 0;
	for (StructDesc& desc : curStructs)
	{
		file.append(desc.CreateData());
		j["Structs"][std::to_string(i)] = StructToJSON(desc);
		i++;
	}

	// debug
	Log::Message("Component Editor", "Genfile: " + file);
	//Log::Message("Component Editor", "Savefile: " + );
	std::cout << "[Component Editor] SaveFile: " << j << std::endl;


	std::fstream f;
	f.open(m_genPath, std::fstream::out);
	if (!f.is_open()) {
		Log::Error("Component Editor", m_genPath + " File not Found");
		return;
	}
	f << file;
	f.close();

	f.open(m_path, std::fstream::out);
	if (!f.is_open()) {
		Log::Error("Component Editor", m_path + " File not Found");
		return;
	}
	f << j.dump(4);
	f.close();
}


StructDesc* curEdited;
void StructEditor::CreateStruct(std::string name)
{
	if (name.find(" ") != std::string::npos) {
		return;
	}
	for (StructDesc& d : curStructs)
	{
		if (d.name == name) {
			name.append("1");
			CreateStruct(name);
			return;
		}
	}

	StructDesc desc(curStructs.size(), name);

	curStructs.push_back(desc);
	curEdited = &curStructs[curStructs.size() - 1];

	Log::Message("Component Editor", "Todo create file structs");
}



std::string curName = "NewComponent";

void StructEditor::Show()
{
	if (ImGui::Begin("Component Editor")) {

		ImGui::BeginGroup();

		char name[1024] = "";
		
		std::strcpy(name, curName.c_str());
		ImGui::InputText("Name", name, 1024);
		curName = name;
		
		if (ImGui::Button("Create New")) {
			CreateStruct(curName);
			curName = "NewComponent";
		}

		if (ImGui::TreeNode("Components")) {
			uint32_t i = 0;
			for (StructDesc& desc : curStructs)
			{
				ImGui::PushID(i++);
				if (ImGui::Button(desc.name.c_str())) {
					curEdited = &desc;
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::EndGroup();

		ShowEdited();

		ImGui::End();
	}
}

std::string curName2 = "newVariable";

void StructEditor::ShowEdited()
{
	ImGui::SameLine();

	ImGui::BeginGroup();
	char varname[1024] = "";

	std::strcpy(varname, curName2.c_str());
	ImGui::InputText("Variable Name", varname, 1024);
	curName2 = varname;

	if (ImGui::Button("Add Variable")) {
		
		curEdited->AddVar(curName2);
		curName2 = "newVariable";
		
	}

	if (ImGui::TreeNode("Variables"))
	{
		if (!curEdited) {
			ImGui::TreePop();
			ImGui::EndGroup();
			return;
		}
		uint32_t i = 0;
		std::vector<std::string> rmf;

		for (auto& var : curEdited->vars)
		{
			i++;
			ImGui::PushID(i);

			if (ImGui::TreeNode(var.first.c_str())) {
				//ImGui::Text("Type: %s", var.second.type.c_str());

				if (ImGui::BeginCombo("Type ", var.second.type.c_str())) {

					for (std::string& s : GenTypes::GetTypes())
					{
						bool selected = s == var.second.type;
						if (ImGui::Selectable(s.c_str(), selected)) {
							if(!selected){
								curEdited->ChangeType(var.first, s);
							}
						}
						if (selected) {
							ImGui::SetItemDefaultFocus();
						}

					}

					ImGui::EndCombo();
				}

				ImGui::Checkbox("List", &var.second.isVector);

				if (ImGui::Button("Remove")) {
					rmf.push_back(var.first);
				}
				ImGui::TreePop();
			}

			ImGui::PopID();
		}

		for (std::string& s : rmf)
		{
			curEdited->vars.erase(s);
		}
		ImGui::TreePop();
	}
	ImGui::EndGroup();
}

StructDesc StructEditor::ReadStruct(uint32_t offset)
{
	ptr = offset;

	StructDesc desc(0, "Test");

	return desc;
}




std::string StructDesc::CreateData()
{
	data = "//" + std::to_string(id) + '\n' + "struct " + name + "{\n";
	for (auto& var : vars)
	{
		data.append(var.second.GetData());
	}
	data.append("};\n");

	return data;
}

void StructDesc::AddVar(std::string name)
{
	if (name.find(" ") != std::string::npos) {
		return;
	}

	if (vars.find(name) != vars.end()) {
		return;
	}

	VariableDesc desc(name);

	vars[name] = desc;
}

void StructDesc::Insert(uint32_t offset, std::string toInsert)
{
	uint32_t sizeBefore = data.size();
	data = data.insert(offset, toInsert);

	if (sizeBefore = data.size()) {
		return;
	}

	if (nameStart >= offset) {
		nameStart += toInsert.size();
	}

	if (firstBracket >= offset) {
		firstBracket += toInsert.size();
	}

	if (lastBracket >= offset) {
		lastBracket += toInsert.size();
	}
}

void StructDesc::Erase(uint32_t start, uint32_t leng)
{
	uint32_t sizeBefore = data.size();
	
	data.erase(start, leng);

	if (sizeBefore = data.size()) {
		return;
	}

	uint32_t diff = leng - start;

	if (nameStart >= leng) {
		nameStart -= diff;
	}

	if (firstBracket >= leng) {
		firstBracket -= diff;
	}

	if (lastBracket >= leng) {
		lastBracket -= diff;
	}
}

void StructDesc::Rename(std::string before, std::string after)
{
	if (vars.find(before) == vars.end()) {
		return;
	}
	if (vars.find(after) != vars.end()) {
		return;
	}

	VariableDesc desc = vars[before];

	vars.erase(before);
	desc.Rename(after);
	vars[after] = desc;
	
}

void StructDesc::ChangeType(std::string varName, std::string newType)
{
	if (vars.find(varName) == vars.end()) {
		return;
	}

	VariableDesc& desc = vars[varName];
	desc.ChangeType(newType);
}

std::string VariableDesc::GetData()
{
	if (isVector) {
		return "   std::vector<" + type + "> " + m_name + ";\n";
	}
	return "   " + type + " " + m_name + ";\n";
}

void VariableDesc::Rename(std::string after)
{
	m_name = after;
}

void VariableDesc::ChangeType(std::string newType)
{
	type = newType;
}


#include "AssetManager.h"
#include "../Rendering/VirtualGeometry/VirtualGeometryBuilder.h"

#include <unordered_map>
#include <map>
#include <functional>

#include "../JSON/json.hpp"
#include "../Log/Log.h"
#include "fstream"

#include "NRIFramework.h"
#include "../Editor/Inspector.h"
#include "Imgui/imgui.h"
//#include "Imgui/misc/cpp/imgui_stdlib.h"

using AssetMap = std::unordered_map<AssetID, AssetBase*>;

using LoadMap = std::unordered_map<AssetType, std::function<AssetBase*(std::string, std::string)>>;


using ExtendToType = std::unordered_map<std::string, AssetType>;

using TypeToTextureMap = std::unordered_map<AssetType, AssetBase*>;

AssetMap map;

LoadMap loadMap;

AssetID lastID = 0;
AssetID GetNext() {
    
    while (map.find(lastID) != map.end() && lastID < 0xfffffffe)
    {
        lastID++;
    }
    return lastID;
}


ExtendToType extToTypeMap = { {"gltf", AssetType::VirtualModel}, {".gltf", AssetType::VirtualModel} };


Node* curDir = nullptr;
Node* rootNode = nullptr;
void AssetManager::Init()
{
    curDir = new Node("Root", nullptr);
    rootNode = curDir;

    // load map
    loadMap[AssetType::VirtualModel] = [](std::string name, std::string path) {
        
        return dynamic_cast<AssetBase*>(VirtualGeometryBuilder::LoadCached(name, path));
        
        };


    std::fstream fStream(utils::GetFullPath("AssetSave.tyr", utils::DataFolder::SAVEFILES));
    if (!fStream.is_open()) {
        return;
    }
    nlohmann::json j = nlohmann::json::parse(fStream);

    lastID = j["LastID"];

    uint32_t count = j["Head"]["Count"];
    for (size_t i = 0; i < count; i++)
    {
        
        nlohmann::json asset = j["Assets"][std::to_string(i)];

        Log::Message("AssetManager", "Load cached asset: " + std::string(asset["Name"]) + "(" + std::to_string(i) + "/" + std::to_string(count) + ")");

        AssetType tyoe = to_type(asset["Type"]);

        if (tyoe == AssetType::None) {
            Log::Error("Type is None");
            continue;
        }
        if (loadMap.find(tyoe) == loadMap.end()) {
            Log::Error("AssetManager", "Type " + std::to_string(tyoe) + " currently not supported");
            continue;
        }

        map[asset["ID"]] = loadMap[to_type(asset["Type"])](asset["Name"], asset["Path"]);
    }
    
    Node* tRoot = LoadDir(j["VirtualFSys"],  nullptr);
    if (tRoot) {
        curDir = tRoot;
        rootNode = tRoot;
    }
}

/*AssetBase* AssetManager::Import(std::string path, std::string name)
{
    Log::Message("Import: " + name);
    return nullptr;
}*/


void AssetManager::Save()
{
    std::ofstream ofStream(utils::GetFullPath("AssetSave.tyr", utils::DataFolder::SAVEFILES));

    if (!ofStream.is_open()) {
        Log::Error("AssetManager", "Wrong Path");
        return;
    }

    nlohmann::json j;

    uint32_t i = 0;

    for (auto& it : map)
    {
        nlohmann::json asset;

        asset["Type"] = std::to_string(it.second->type);
        asset["Name"] = it.second->name;
        asset["Path"] = it.second->path;
        asset["ID"] = it.first;

        j["Assets"][std::to_string(i)] = asset;
        i++;
    }

    j["Head"]["Count"] = i;

    j["VirtualFSys"] = SaveDir(rootNode);

    j["LastID"] = lastID;

    ofStream << std::setw(4) << j << "\n";
}

bool isImPorting;
AssetType importedType;

std::string extend;
std::string pt;

std::string virtualPath;
//std::string DirName = "--Directory--";

AssetBase* curInspected = nullptr;



void AssetManager::Show()
{
    AssetImporter();
    AssetExplorer();
}

void AssetManager::GoBack()
{
    if (curDir->parent) {
        curDir = curDir->parent;
    }
}

void AssetManager::Open(Node* n)
{
    if (!n) {
        return;
    }

    if (n->realName == 0xffffffff) {
        curDir = n;
    }
    else {
        Inspector::Select(GetAsset(n->realName));
        curInspected = map[n->realName];
    }
}
float size = 1;
std::string curPath = "";

void AssetManager::AssetExplorer()
{
    

    ImGui::Begin("AssetManager");

    if (ImGui::Button("<---")) {
        GoBack();
    }

    ImGui::SameLine();
    ImGui::SliderFloat("Size", &size, .4f, 2);
    ImGui::SameLine();
    if (ImGui::Button("New Folder")) {
        AddFolder("new folder");
    }
    ImGui::SameLine();
    
    if (ImGui::Button("Import Asset")) {
        importedType = AssetType::None;
        isImPorting = true;
    }
    

    float width = ImGui::GetContentRegionAvail().x;
    unsigned int wouldFit = width / (size * 144);

    uint32_t x = 0;
    uint32_t i = 0;

    static bool isRenaming = false;
    static Node* toEdit;
    static char nName[1024] = "";

    bool opened = false;

    for (Node* n : curDir->childs)
    {
        if (x >= wouldFit) {
            x = 0;
        }
        else {
            if (i > 0) {
                ImGui::SameLine();
            }
            
        }
        ImGui::BeginGroup();
        ImGui::PushID(i);

        if (ImGui::Button(std::to_string(n->type).c_str(), ImVec2(144 * size, 144 * size))) {
            Open(n);
        }
        ImGui::Text(n->vName.c_str());
        ImGui::PopID();

        ImGui::EndGroup();

        if (!opened) {
            if (ImGui::BeginPopupContextItem("Edit", 1)) {
                opened = true;
                if (ImGui::MenuItem("Rename")) {
                    isRenaming = true;
                    toEdit = n;
                    std::strcpy(nName, n->vName.c_str());
                }

                if (ImGui::MenuItem("Delete")) {
                    isRenaming = true;
                    toEdit = n;
                }

                if (ImGui::MenuItem("Dublicate")) {
                    Log::Message("Not Supported");
                }

                /*if (n->type != AssetType::None && n->realName != "--Directory--") {
                    if (ImGui::MenuItem("Reimport")) {
                        AssetBase* b = GetAsset(n->realName);
                        if (b) {
                            curPath = b->oldPath;
                            isImPorting = true;
                            importedType = n->type;
                        }
                    }
                }*/
                ImGui::EndPopup();
            }
        }
        x++;
        i++;
    }

    ImGui::End();


    if (!isRenaming) {
        return;
    }

    


    ImGui::Begin("Rename");
    
    ImGui::InputText("New Name", nName, 1024);
    if (ImGui::Button("Rename")) {
        toEdit->vName = nName;
        if (toEdit->realName != 0xffffffff) {
            GetAsset(toEdit->realName)->name = nName;
        }
        isRenaming = false;
        Save();
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        isRenaming = false;
    }

    ImGui::End();
}

void AssetManager::AssetImporter()
{
    if (!isImPorting) {
        return;
    }
    static char name[1024] = "New Asset";
    static char path[2048] = "C:/";
    ImGui::Begin("Import Asset");
    
    if (curPath != "") {
        std::strcpy(path, curPath.c_str());
        curPath.clear();
    }

    ImGui::InputText("Name", name, 1024);
    ImGui::InputText("Path", path, 2048);
    

    pt = path;
    if (pt.find(".") != std::string::npos) {
        extend = pt.substr(pt.find_last_of("."), pt.length() - pt.find_last_of("."));
    }


    if (extToTypeMap.find(extend) != extToTypeMap.end()) {
        importedType = extToTypeMap[extend];
    }

    static bool singleModel = false;

    switch (importedType)
    {
    case None:
        ImGui::Text("Type: None");
        ImGui::Button("Import");
        break;
    case VirtualModel:

        ImGui::Text("Type: Model");
        ImGui::Checkbox("Treat As Single Model", &singleModel);

        if (ImGui::Button("Import")) {

            if (!singleModel) {
                std::vector<VModel*> assets = VirtualGeometryBuilder::BuildVG(path, name, true, false);

                for (uint32_t i = 0; i < assets.size();i++)
                {
                    AssetBase* asset = dynamic_cast<AssetBase*>(assets[i]);
                    if (!asset) {
                        continue;
                    }
                    RegisterAsset(asset);
                    //map[(std::string(name) + "_subM" + std::to_string(i))] = dynamic_cast<AssetBase*>(assets[i]);
                }
            }
            else {
                AssetBase* asset = dynamic_cast<AssetBase*>(VirtualGeometryBuilder::BuildVG(path, name, true, true)[0]);
                if (!asset) {
                    Log::Error("Asset Manager", "Import Failed");
                    ImGui::End();
                    return;
                }
                RegisterAsset(asset);
            }

            isImPorting = false;

        }
        break;
    default:
        ImGui::Text("Type: None");
        ImGui::Button("Import");
        break;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
        isImPorting = false;
    }
    ImGui::End();
}

AssetID AssetManager::RegisterAsset(AssetBase* asset)
{
    AssetID id = GetNext();
    

    map[id] = asset;

    Node* n = new Node(asset->name, curDir, id, asset->type);
    map[id]->vRepr = n;
    Save();
    return id;
}

void AssetManager::AddFolder(std::string name)
{
    Node* n = new Node(name, curDir);
    Save();
}

void AssetManager::Delete(AssetID name)
{
    if (map.find(name) == map.end()) {
        return;
    }
    AssetBase* b = map[name];
    Save();
}

nlohmann::json AssetManager::SaveDir(Node* n)
{
    nlohmann::json j;
    j["VName"] = n->vName;
    j["Type"] = std::to_string(n->type);
    j["RealName"] = n->realName;
    j["ChildCount"] = n->childs.size();
    j["Test"] = true;
    
    if (n->realName == 0xffffffff) {
        for (uint32_t i = 0; i < n->childs.size(); i++)
        {
            j["Childs"][std::to_string(i)] = SaveDir(n->childs[i]);
        }
    }

    return j;
}

Node* AssetManager::LoadDir(nlohmann::json& j, Node* parent)
{
    if (j["Test"] != true) {
        return nullptr;
    }

    if (j["RealName"] == 0xffffffff) {
        Node* n = new Node(j["VName"], parent);

        uint32_t count = j["ChildCount"];
        for (uint32_t i = 0; i < count; i++)
        {
            LoadDir(j["Childs"][std::to_string(i)], n);
        }
        n->vName = j["VName"];
        return n;
    }

    AssetBase* a = GetAsset(j["RealName"]);
    if (!a) {
        return nullptr;
    }

    Node* n = new Node(a->name, parent, j["RealName"], to_type(j["Type"]));
    a->vRepr = n;
    n->vName = j["VName"];
    a->name = j["VName"];
    return n;
}

AssetBase* AssetManager::GetAsset(AssetID name)
{
    if (map.find(name) == map.end() || name == 0xffffffff)
        return nullptr;

    return map[name];
}






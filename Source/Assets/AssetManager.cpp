
#include "AssetManager.h"
#include "../Rendering/VirtualGeometry/VirtualGeometryBuilder.h"

#include <unordered_map>
#include <map>
#include <functional>

#include "../JSON/json.hpp"
#include "../Log/Log.h"
#include "fstream"

#include "NRIFramework.h"

#include "Imgui/imgui.h"
#include "Imgui/misc/cpp/imgui_stdlib.h"

using AssetMap = std::unordered_map<std::string, AssetBase*>;

using LoadMap = std::unordered_map<AssetType, std::function<AssetBase*(std::string)>>;


using ExtendToType = std::unordered_map<std::string, AssetType>;

using TypeToTextureMap = std::unordered_map<AssetType, AssetBase*>;

AssetMap map;

LoadMap loadMap;


ExtendToType extToTypeMap = { {"gltf", AssetType::VirtualModel}, {".gltf", AssetType::VirtualModel} };


Node* curDir = nullptr;
void AssetManager::Init()
{
    curDir = new Node("Root", nullptr);
    
    // load map
    loadMap[AssetType::VirtualModel] = [](std::string name) {
        
        return dynamic_cast<AssetBase*>(VirtualGeometryBuilder::LoadCached(name));
        
        };


    std::fstream fStream(utils::GetFullPath("AssetSave", utils::DataFolder::SAVEFILES));
    if (!fStream.is_open()) {
        return;
    }
    nlohmann::json j = nlohmann::json::parse(fStream);

    uint32_t count = j["Head"]["Count"];
    for (size_t i = 0; i < count; i++)
    {
        
        nlohmann::json asset = j["Assets"][std::to_string(i)];

        Log::Message("AssetManager", "Load cached asset: " + std::string(asset["Name"]) + "(" + std::to_string(i) + "/" + std::to_string(count) + ")");

        if (to_type(asset["Type"]) == AssetType::None) {
            Log::Error("Type is None");
            continue;
        }
        if (loadMap.find(to_type(asset["Type"])) == loadMap.end()) {
            Log::Error("Type currently not supported");
            continue;
        }
        map[asset["Name"]] = loadMap[to_type(asset["Type"])](asset["Name"]);
    }
}

AssetBase* AssetManager::Import(std::string path, std::string name)
{
    Log::Message("Import: " + name);
    return nullptr;
}


void AssetManager::Save()
{
    nlohmann::json j;

    uint32_t i = 0;

    for (auto& it : map)
    {
        nlohmann::json asset;

        asset["Type"] = std::to_string(it.second->type);
        asset["Name"] = it.first;

        j["Assets"][std::to_string(i)] = asset;
        i++;
    }

    j["Head"]["Count"] = std::to_string(i);
    std::ofstream ofStream(utils::GetFullPath("AssetSave", utils::DataFolder::SAVEFILES));

    ofStream << std::setw(4) << j << "\n";
}

bool isImPorting;
AssetType importedType;

std::string extend;
std::string pt;

std::string virtualPath;
//std::string DirName = "--Directory--";

AssetBase* curInspected = nullptr;



void AssetManager::Update()
{
    AssetImporter();
    AssetExplorer();
    AssetInspector();
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

    if (n->realName == "--Directory--") {
        curDir = n;
    }
    else {
        curInspected = map[n->realName];
    }
}
float size = 1;
void AssetManager::AssetExplorer()
{
    

    ImGui::Begin("AssetManager");

    if (ImGui::Button("<---")) {
        GoBack();
    }

    ImGui::SameLine();
    ImGui::SliderFloat("Size", &size, .1f, 3);
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
        if (ImGui::Button(n->vName.c_str(), ImVec2(144*size, 144*size))) {
            Open(n);
        }
        ImGui::Text(n->vName.c_str());
        ImGui::PopID();
        ImGui::EndGroup();

        x++;
        i++;
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
    {
        ImGui::InputText("Name", name, 1024);
        ImGui::InputText("Path", path, 2048);
    }

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

            if (map.find(name) != map.end()) {
                Log::Warning("Asset Manager", "Name Already Taken");
                ImGui::End();
                return;
            }

            if (!singleModel) {
                std::vector<VModel*> assets = VirtualGeometryBuilder::BuildVG(path, name, true, false);

                for (uint32_t i = 0; i < assets.size();i++)
                {
                    AssetBase* asset = dynamic_cast<AssetBase*>(assets[i]);
                    if (!asset) {
                        continue;
                    }
                    RegisterAsset(asset, std::string(name) + "_subM" + std::to_string(i));
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
                RegisterAsset(asset, name);
            }

            isImPorting = false;

        }
        break;
    default:
        ImGui::Text("Type: None");
        ImGui::Button("Import");
        break;
    }

    ImGui::End();
}

void AssetManager::AssetInspector()
{
    //                                                      todo
}

void AssetManager::RegisterAsset(AssetBase* asset, std::string name)
{
    if (map.find(name) != map.end()) {
        Log::Warning("Asset Manager", "Name Already Taken");
        ImGui::End();
        return;
    }

    map[name] = asset;

    Node* n = new Node(name, curDir, name);
}

void AssetManager::AddFolder(std::string name)
{
    Node* n = new Node(name, curDir);
}

AssetBase* AssetManager::GetAsset(std::string name)
{
    if (map.find(name) == map.end())
        return nullptr;

    return map[name];
}






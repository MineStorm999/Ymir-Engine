
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

AssetMap map;

LoadMap loadMap;


ExtendToType extToTypeMap = { {"gltf", AssetType::VirtualModel}, {".gltf", AssetType::VirtualModel} };



void AssetManager::Init()
{
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

void AssetManager::Update()
{
    
    ImGui::Begin("AssetManager");
                                            // todo
    if (ImGui::Button("Import Asset")) {
        importedType = AssetType::None;
        isImPorting = true;
    }

    ImGui::End();

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

                for (uint32_t i = 0; i<assets.size();i++)
                {
                    map[(std::string(name) + "_subM" + std::to_string(i))] = dynamic_cast<AssetBase*>(assets[i]);
                }
            }
            else {
                AssetBase* asset = dynamic_cast<AssetBase*>(VirtualGeometryBuilder::BuildVG(path, name, true, true)[0]);
                if (!asset) {
                    Log::Error("Asset Manager", "Import Failed");
                    ImGui::End();
                    return;
                }
                map[name] = asset;
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

AssetBase* AssetManager::GetAsset(std::string name)
{
    if (map.find(name) == map.end())
        return nullptr;

    return map[name];
}

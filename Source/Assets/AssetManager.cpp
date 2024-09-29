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
#include "../Common/utils.h"

#include "../Rendering/StaticMesh/StaticMeshLoader.h"
#include <chrono>

//#include "Imgui/misc/cpp/imgui_stdlib.h"


std::filesystem::path curPath = "/";
std::filesystem::path rootPath = "/";


using LoadMap = std::unordered_map<AssetType, std::function<AssetBase*(nlohmann::json)>>;


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


ExtendToType extToTypeMap = { {"gltf", AssetType::Model}, {".gltf", AssetType::Model} };

//Node* curDir = nullptr;
//Node* rootNode = nullptr;

void AssetManager::Init()
{

    loadMap[AssetType::Model] = [](nlohmann::json j) {
        AModel* m = new AModel();

        m->indexCount = j["IndexCount"];
        m->vertCount = j["VertexCount"];
        m->DefaultMaterialID = j["DefaultMat"];
        m->iBuffLenght = j["IBuffLenght"];

        uint32_t lodCount = j["LODCount"];
        for (uint32_t i = 0; i < lodCount; i++)
        {
            nlohmann::json lod = j["Lods"][std::to_string(i)];
            m->lods.push_back({ lod["Distance"], lod["IndexOffset"], lod["IndexCount"] });
        }

        return dynamic_cast<AssetBase*>(m);
        };


    //curDir = new Node("Root", nullptr);
    //rootNode = curDir;
    curPath = utils::GetCFullPath("", utils::CustomFolder::ASSETDIR);
    rootPath = utils::GetCFullPath("", utils::CustomFolder::ASSETDIR);

    // load map
    
    std::fstream fStream(utils::GetCFullPath("AssetSave.tyr", utils::CustomFolder::SAVEFILES));
    if (!fStream.is_open()) {
        return;
    }
    nlohmann::json j = nlohmann::json::parse(fStream);

    lastID = j["LastID"];

    uint32_t count = j["Head"]["Count"];
    for (size_t i = 0; i < count; i++)
    {
        
        nlohmann::json asset = j["Assets"][std::to_string(i)];

        Log::Message("AssetManager", "Load asset: " + std::string(asset["Name"]) + "(" + std::to_string(i+1) + "/" + std::to_string(count) + ")");

        AssetType tyoe = to_type(asset["Type"]);

        if (tyoe == AssetType::None) {
            Log::Error("Type is None");
            continue;
        }
        if (loadMap.find(tyoe) == loadMap.end()) {
            Log::Error("AssetManager", "Type " + std::to_string(tyoe) + " currently not supported");
            continue;
        }
        AssetBase* bAss = loadMap[tyoe](asset);
        bAss->name = asset["Name"];
        bAss->path = asset["Path"];
        bAss->originalPath = asset["OriginalPath"];
        bAss->lenght = asset["Lenght"];
        bAss->off = asset["Offset"];

        map[asset["ID"]] = bAss;
    }
    
    /*Node* tRoot = LoadDir(j["VirtualFSys"], nullptr);
    if (tRoot) {
        curDir = tRoot;
        rootNode = tRoot;
    }*/
}

/*AssetBase* AssetManager::Import(std::string path, std::string name)
{
    Log::Message("Import: " + name);
    return nullptr;
}*/


void AssetManager::Save()
{
    std::ofstream ofStream(utils::GetCFullPath("AssetSave.tyr", utils::CustomFolder::SAVEFILES));

    if (!ofStream.is_open()) {
        Log::Error("AssetManager", "Wrong Path");
        return;
    }

    nlohmann::json j;

    uint32_t i = 0;

    for (auto& it : map)
    {
        if (!it.second) {
            continue;
        }
        nlohmann::json asset;
        if (it.second->type == AssetType::None) {
            continue;
        }
        else if (it.second->type == AssetType::Model) {
            AModel* m = dynamic_cast<AModel*>(it.second);
            if (m) {
                asset = m->Save();
            }
        }
        else if (it.second->type == AssetType::Texture) {
            ATexture* tex = dynamic_cast<ATexture*>(it.second);
            if (tex) {
                asset = tex->Save();
            }
        }
        else if (it.second->type == AssetType::Material) {
            AMaterial* mat = dynamic_cast<AMaterial*>(it.second);
            if (mat) {
                asset = mat->Save();
            }
        }
        else if (it.second->type == AssetType::Audio) {
            AAudio* aud = dynamic_cast<AAudio*>(it.second);
            if (aud) {
                asset = aud->Save();
            }
        }
        else if (it.second->type == AssetType::Scene) {
            AScene* sce = dynamic_cast<AScene*>(it.second);
            if (sce) {
                asset = sce->Save();
            }
        }

        asset["ID"] = it.first;
        j["Assets"][std::to_string(i)] = asset;
        i++;
    }

    j["Head"]["Count"] = i;

    //j["VirtualFSys"] = SaveDir(rootNode);

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

AssetMap& AssetManager::GetMap()
{
    return map;
}

bool AssetManager::IsValid(AssetID id)
{
    return !(map.find(id) == map.end() || id == 0xffffffff);
}


float size = 1;

struct CachedDirEntry
{
    AssetID id;
    std::filesystem::file_time_type timeStamp;
};

std::unordered_map<std::string, CachedDirEntry> cache;

void AssetManager::AssetExplorer()
{
    ImGui::Begin("AssetManager");

    if (curPath.string() != rootPath.string()) {
        if (ImGui::Button("<---")) {
            curPath = curPath.parent_path();
        }
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
    static AssetID toEdit;
    static char nName[1024] = "";

    bool opened = false;
    
    

    for (auto& it : std::filesystem::directory_iterator(curPath))
    {
        if (x >= wouldFit) {
            x = 0;
        }
        else {
            if (i > 0) {
                ImGui::SameLine();
            }
        }
        

        AssetID curAsset = INVALID_ASSET_ID;
        if (!it.is_directory()) {
            if (cache.find(it.path().string()) != cache.end()) {
                //if(it.last_write_time())
                curAsset = cache[it.path().string()].id;
                //Log::Message("AssetManager", it.path().string() + ", cached");
            }
            else {
                Log::Message("AssetManager", it.path().string() + ", NOT cached");
                curAsset = AssetUtils::GetAssetIDFromImported(it.path().string());

                if (!AssetManager::IsValid(curAsset)) {
                    //Log::Message("AssetManager", "Cur Asset is invalid, continuing..." + std::to_string(curAsset));
                    continue;
                }
                cache[it.path().string()] = { curAsset, it.last_write_time() };
            }
        }

        ImGui::BeginGroup();
        ImGui::PushID(i);
        if (it.is_directory()) {
            if (ImGui::Button("Folder", ImVec2(144 * size, 144 * size))) {
                curPath /= it.path().filename();
            }
            ImGui::Text(it.path().filename().string().c_str());
        }
        else {
            if (ImGui::Button(std::to_string(GetAsset(curAsset)->type).c_str(), ImVec2(144 * size, 144 * size))) {
                //Open(n);
            }
            ImGui::Text(GetAsset(curAsset)->name.c_str());
        }

        ImGui::EndGroup();
        


        /*if (!opened && IsValid(curAsset)) {
            if (ImGui::BeginPopupContextItem("Edit", 1)) {
                opened = true;
                if (ImGui::MenuItem("Rename")) {
                    isRenaming = true;
                    toEdit = curAsset;
                    std::strcpy(nName, it.path().filename().string().c_str());
                }

                if (ImGui::MenuItem("Delete")) {
                    Delete(curAsset);
                    cache.erase(it.path().string());
                }

                if (ImGui::MenuItem("Dublicate")) {
                    Log::Message("Not Supported");
                }*/

                /*if (n->type != AssetType::None && n->realName != "--Directory--") {
                    if (ImGui::MenuItem("Reimport")) {
                        AssetBase* b = GetAsset(n->realName);
                        if (b) {
                            curPath = b->oldPath;
                            isImPorting = true;
                            importedType = n->type;
                        }
                    }
                }
                ImGui::EndPopup();
            }
        }*/
        ImGui::PopID();

        
        x++;
        i++;
    }

    ImGui::End();


    if (!isRenaming) {
        return;
    }

    
    //return;
    if (!IsValid(toEdit)) {
        return;
    }
    ImGui::Begin("Rename");
    
    ImGui::InputText("New Name", nName, 1024);
    if (ImGui::Button("Rename")) {
        if (std::rename(GetAsset(toEdit)->GetActualPath().c_str(), nName)) {
            Log::Error("AssetManager", "Renaming from  " + GetAsset(toEdit)->GetActualPath() + " , failed");
            std::perror("Error renaming file");
        }
        else {
            GetAsset(toEdit)->name = nName;
            isRenaming = false;
            Save();
        }
        

        
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
    //static std::string curP = "C:/";

    ImGui::Begin("Import Asset");
    
    if (pt != "") {
        std::strcpy(path, pt.c_str());
        pt.clear();
    }

    ImGui::InputText("Name", name, 1024);
    ImGui::InputText("Path", path, 2048);
    
    //curP = path;
    pt = path;
    if (pt.find(".") != std::string::npos) {
        extend = pt.substr(pt.find_last_of("."), pt.length() - pt.find_last_of("."));
    }


    if (extToTypeMap.find(extend) != extToTypeMap.end()) {
        importedType = extToTypeMap[extend];
    }

    static bool singleModel = false;

    static MeshImportSettings settings;

    switch (importedType)
    {
    case AssetType::None:
        ImGui::Text("Type: None");
        ImGui::Button("Import");
        break;
    case AssetType::Model:
        //MeshImportSettings settings;
        ImGui::Text("Type: Model");
        ImGui::Checkbox("Treat As Single Model", &settings.singleModel);
        ImGui::Checkbox("Auto LOD", &settings.autoLOD);
        if (settings.autoLOD) {
            ImGui::SliderInt("Lod Count", &settings.lodCount, 1, 10);
        }
        else {
            settings.lodCount = 0;
        }

        ImGui::Checkbox("Optimize", &settings.optimize);
        ImGui::Checkbox("Compress", &settings.compress);

        if (ImGui::Button("Import")) {
            
            
            MeshLoader::ImportModel(pt, curPath.string(), name, settings);

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
    if (!asset) {
        return INVALID_ASSET_ID;
    }
    AssetID id = GetNext();
    

    map[id] = asset;

    Save();
    return id;
}

void AssetManager::AddFolder(std::string name)
{
    std::filesystem::create_directories(curPath / name);
    Save();
}

void AssetManager::Delete(AssetID name)
{
    if (map.find(name) == map.end()) {
        return;
    }

    AssetBase* b = map[name];
    std::remove(b->GetActualPath().c_str());
    
    delete b;

    map.erase(name);
    Save();
}


AssetBase* AssetManager::GetAsset(AssetID name)
{
    if (map.find(name) == map.end() || name == INVALID_ASSET_ID)
        return nullptr;

    return map[name];
}






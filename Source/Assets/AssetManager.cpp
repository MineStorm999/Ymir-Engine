#include "AssetManager.h"
#include <unordered_map>
#include <map>
#include <functional>

using AssetMap = std::unordered_map<std::string, AssetBase*>;
using LoadMap = std::unordered_map<AssetType, std::function<AssetBase*(std::string)>>;

AssetMap map;
LoadMap loadMap;


void AssetManager::Init()
{
    
}

AssetBase* AssetManager::GetAsset(std::string name)
{
    if (map.find(name) == map.end())
        return nullptr;

    return map[name];
}

#include "VirtualGeometryBuilder.h"

#include "NRIFramework.h"

#include "meshoptimizer.h"

void VirtualGeometryBuilder::BuildVG(std::string inPath, std::string name, bool forceUpdate)
{
    utils::Scene scene;
    utils::LoadScene(inPath, scene, false);

    std::vector<float3> pos;
    pos.reserve(scene.vertices.size());

    for (utils::Vertex& v : scene.vertices)
    {
        pos.push_back(v.pos);
    }

    FILE* virtualModel = fopen(inPath.c_str(), "r");
    if (virtualModel) {
        fclose(virtualModel);
        if (!forceUpdate) {
            return;
        }
    }

    virtualModel = fopen(inPath.c_str(), "w");
    
}

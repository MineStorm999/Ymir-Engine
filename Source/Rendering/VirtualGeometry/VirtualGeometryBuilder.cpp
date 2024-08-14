#include "VirtualGeometryBuilder.h"

#include "NRIFramework.h"

#include "meshoptimizer.h"

#include "../../Log/Log.h"

#include "../../../Shaders/MeshletStructs.h"




VModel* VirtualGeometryBuilder::BuildVG(std::string inPath, std::string name, bool forceUpdate)
{
    Log::Message("Load Scene: " + inPath);

    utils::Scene scene;
    utils::LoadScene(inPath, scene, false);

    VModel* vModel = new VModel();

    std::vector<float> pos;
    pos.reserve(scene.vertices.size() * 3);

    for (utils::Vertex& v : scene.vertices)
    {
        pos.push_back(v.pos[0]);
        pos.push_back(v.pos[1]);
        pos.push_back(v.pos[2]);
    }

    uint32_t curWritePos = 0;


    uint32_t maxVerts = 64;
    uint32_t maxTries = 128;
    float coneWeight = 0.0f;

    

    size_t maxMeshlets = meshopt_buildMeshletsBound(scene.indices.size(), maxVerts, maxTries);

    std::vector<meshopt_Meshlet> meshlets(maxMeshlets);
    std::vector<unsigned int> meshlet_vertices(maxMeshlets * maxVerts);
    std::vector<unsigned char> meshlet_triangles(maxMeshlets * maxTries * 3);


    size_t meshlet_count = meshopt_buildMeshlets(meshlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), scene.indices.data(),
        scene.indices.size(), pos.data(), pos.size(), sizeof(float)*3, maxVerts, maxTries, coneWeight);

    const meshopt_Meshlet& last = meshlets[meshlet_count - 1];

    meshlet_vertices.resize(last.vertex_offset + last.vertex_count);
    meshlet_triangles.resize(last.triangle_offset + ((last.triangle_count * 3 + 3) & ~3));
    meshlets.resize(meshlet_count);

    for (meshopt_Meshlet& m : meshlets)
    {
        meshopt_optimizeMeshlet(&meshlet_vertices[m.vertex_offset], &meshlet_triangles[m.triangle_offset], m.triangle_count, m.vertex_count);
    }
    


    FILE* virtualModel = fopen(inPath.c_str(), "r");
    if (virtualModel) {
        fclose(virtualModel);
        if (!forceUpdate) {
            Log::Warning("File alsready Exists, stopping");
            return;
        }
        else {
            Log::Warning("File alsready Exists, continuing");
        }
    }

    virtualModel = fopen(inPath.c_str(), "w");
    
    return vModel;
}

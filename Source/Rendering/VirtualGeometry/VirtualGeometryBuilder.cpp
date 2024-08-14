#include "VirtualGeometryBuilder.h"

#include "NRIFramework.h"

#include "meshoptimizer.h"

#include "../../Log/Log.h"

#include "MeshletStructs.h"

#include "Utils.h"



VModel* VirtualGeometryBuilder::BuildVG(std::string inPath, std::string name, bool forceUpdate)
{
    Log::Message("Build Meshlet for: " + inPath);

    utils::Scene scene;
    utils::LoadScene(inPath, scene, false);

    VModel* vModelDesc = new VModel();
    
    

    std::vector<float> pos;
    pos.reserve(scene.vertices.size() * 3);

    for (uint32_t i = 0; i< scene.vertices.size(); i++)
    {
        utils::Vertex& v = scene.vertices[i];
        pos.push_back(v.pos[0]);
        pos.push_back(v.pos[1]);
        pos.push_back(v.pos[2]);
        pos.push_back(i);
    }

    uint32_t curWritePos = 0;

    //                                                                                          temp
    curWritePos += 0;

    uint32_t maxVerts = 64;
    uint32_t maxTries = 128;
    float coneWeight = 0.0f;

    
    Log::Message("Build Meshlets Bounds");
    size_t maxMeshlets = meshopt_buildMeshletsBound(scene.indices.size(), maxVerts, maxTries);

    std::vector<meshopt_Meshlet> meshlets(maxMeshlets);
    std::vector<unsigned int> meshlet_vertices(maxMeshlets * maxVerts);
    std::vector<unsigned char> meshlet_triangles(maxMeshlets * maxTries * 3);

    Log::Message("Build Meshlets");
    size_t meshlet_count = meshopt_buildMeshlets(meshlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), scene.indices.data(),
        scene.indices.size(), pos.data(), pos.size(), sizeof(float)*4, maxVerts, maxTries, coneWeight);

    const meshopt_Meshlet& last = meshlets[meshlet_count - 1];

    meshlet_vertices.resize(last.vertex_offset + last.vertex_count);
    meshlet_triangles.resize(last.triangle_offset + ((last.triangle_count * 3 + 3) & ~3));
    meshlets.resize(meshlet_count);

    vModelDesc->meshlets.reserve(meshlets.size());


    void* streamData = malloc((sizeof(Vertex) * meshlet_vertices.size()) + meshlet_triangles.size());
    std::vector<Vertex> meshletVerts(meshlet_vertices.size());

    for (uint32_t i = 0; i < meshlet_vertices.size(); i++)
    {
        Vertex& v = meshletVerts[i];
        float4 posI = { meshlet_vertices[(i * 4)], meshlet_vertices[(i * 4) + 1],
            meshlet_vertices[(i * 4) + 2],meshlet_vertices[(i * 4) + 3] };

        utils::Vertex& oldV = scene.vertices[meshlet_vertices[(i * 4) + 3]];

       
        if (!(v.pos.x == posI.x && v.pos.y == posI.y && v.pos.z == posI.z)) {
            Log::Error("New And Old Pos Not Same");
            return nullptr;
        }

        v.pos = oldV.pos;
        v.norm = { oldV.N, oldV.uv.x };
        v.tangent = { oldV.T, oldV.uv.y };
    }

    int i = 0;
    for (meshopt_Meshlet& m : meshlets)
    {
        i++;
        Log::Message("Meshlet " + std::to_string(i) + "   Vert Count: " + std::to_string(m.vertex_count) + "   tri count: " + std::to_string(m.triangle_count));

        // optimize
        meshopt_optimizeMeshlet(&meshlet_vertices[m.vertex_offset], &meshlet_triangles[m.triangle_offset], m.triangle_count, m.vertex_count);


        meshopt_Bounds bounds = meshopt_computeMeshletBounds(&meshlet_vertices[m.vertex_offset], &meshlet_triangles[m.triangle_offset],
            m.triangle_count, pos.data(), pos.size(), sizeof(float)*3);


        // meshlet descriptor
        MeshletDesc mDesc;
        
        // culling 
        mDesc.boundingSphere = { bounds.center, bounds.radius };
        mDesc.cone_axis = { bounds.cone_axis };
        
        uint32_t vertTriCount = m.vertex_count;
        //vertTriCount << (uint16_t)m.triangle_count;                               todo make work
        mDesc.vertTriCount = vertTriCount;


        
        //                                              todo child count

        vModelDesc->meshlets.push_back(mDesc);
    }
    
    // create 


    std::string outPath = utils::GetFullPath(name, utils::DataFolder::VIRTUALMESH);

    FILE* virtualModel = fopen(inPath.c_str(), "r");
    if (virtualModel) {
        fclose(virtualModel);
        if (!forceUpdate) {
            Log::Warning("File alsready Exists, stopping");
            return nullptr;
        }
        else {
            Log::Message("File alsready Exists, continuing");
        }
    }

    virtualModel = fopen(inPath.c_str(), "w");
    scene.UnloadGeometryData();
    scene.UnloadTextureData();
    return vModelDesc;
}

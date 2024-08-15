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
    
    std::string outPath = utils::GetFullPath(name, utils::DataFolder::VIRTUALMESH);

    FILE* outData = fopen(inPath.c_str(), "r");
    if (outData) {
        fclose(outData);
        if (!forceUpdate) {
            Log::Warning("File alsready Exists, stopping");
            return nullptr;
        }
        else {
            Log::Message("File alsready Exists, continuing");
        }
    }

    outData = fopen(inPath.c_str(), "w");

    std::vector<float> pos;
    pos.reserve(scene.vertices.size() * 3);

    float id = 0;
    for (uint32_t i = 0; i< scene.vertices.size(); i++)
    {
        utils::Vertex& v = scene.vertices[i];
        pos.push_back(v.pos[0]);
        pos.push_back(v.pos[1]);
        pos.push_back(v.pos[2]);
        pos.push_back(id);

        id++;
    }

   

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

    //Vertex* dummyVert = new Vertex();

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

        
        for (uint32_t ver = 0; ver < m.vertex_count; ver++)
        {
            uint32_t vert = meshlet_vertices[ver + m.vertex_offset];
            float3 pso = { pos[vert * 4 + 0], pos[vert * 4 + 1], pos[vert * 4 + 2] };
            uint32_t id = pos[vert * 4 + 3];

            //uint32_t id = meshlet_vertices[m.vertex_offset + (vert * 4) + 3];
            if (!(scene.vertices[id].pos[0] == pso.x && scene.vertices[id].pos[1] == pso.y && scene.vertices[id].pos[2] == pso.z)) {

            }
        }
        //fwrite( dummyVert, sizeof(Vertex), 1, outData);
    }
    
    // create 


    

    
    scene.UnloadGeometryData();
    scene.UnloadTextureData();
    return vModelDesc;
}

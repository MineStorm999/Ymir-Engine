#include "VirtualGeometryBuilder.h"

#include "NRIFramework.h"

#include "meshoptimizer.h"

#include "../../Log/Log.h"

#include "MeshletStructs.h"

#include "Utils.h"

#include "../../Assets/exts.h"

VModel* VirtualGeometryBuilder::BuildVGImpl(std::vector<unsigned int> indices, std::vector<utils::Vertex> vertices, std::string name, bool forceUpdate)
{
    

    

    VModel* vModelDesc = new VModel();
    
    std::string outPath = utils::GetFullPath(name, utils::DataFolder::VIRTUALMESH) + VModelExt;

    FILE* outData = fopen(outPath.c_str(), "r");
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

    outData = fopen(outPath.c_str(), "w");

    std::vector<float> pos;
    pos.reserve(vertices.size() * 4);

    for (uint32_t i = 0; i< vertices.size(); i++)
    {
        utils::Vertex& v = vertices[i];
        pos.push_back(v.pos[0]);
        pos.push_back(v.pos[1]);
        pos.push_back(v.pos[2]);
        pos.push_back((float)i);
    }

   

    uint32_t maxVerts = 64;
    uint32_t maxTries = 128;
    float coneWeight = 0.0f;

    
    Log::Message("Build Meshlets Bounds");
    size_t maxMeshlets = meshopt_buildMeshletsBound(indices.size(), maxVerts, maxTries);

    std::vector<meshopt_Meshlet> meshlets(maxMeshlets);
    std::vector<unsigned int> meshlet_vertices(maxMeshlets * maxVerts);
    std::vector<unsigned char> meshlet_triangles(maxMeshlets * maxTries * 3);

    Log::Message("Build Meshlets");
    size_t meshlet_count = meshopt_buildMeshlets(meshlets.data(), meshlet_vertices.data(), meshlet_triangles.data(), indices.data(),
        indices.size(), pos.data(), pos.size(), sizeof(float)*4, maxVerts, maxTries, coneWeight);

    const meshopt_Meshlet& last = meshlets[meshlet_count - 1];

    meshlet_vertices.resize(last.vertex_offset + last.vertex_count);
    meshlet_triangles.resize(last.triangle_offset + ((last.triangle_count * 3 + 3) & ~3));
    meshlets.resize(meshlet_count);

    vModelDesc->meshlets.reserve(meshlets.size());

    Vertex* dummyVert = new Vertex();
    uint8_t* zero = new uint8_t(0);

    int i = 0;
    for (meshopt_Meshlet& m : meshlets)
    {
        i++;
        //Log::Message("Meshlet " + std::to_string(i) + "   Vert Count: " + std::to_string(m.vertex_count) + "   tri count: " + std::to_string(m.triangle_count));

        // optimize
        meshopt_optimizeMeshlet(&meshlet_vertices[m.vertex_offset], &meshlet_triangles[m.triangle_offset], m.triangle_count, m.vertex_count);


        meshopt_Bounds bounds = meshopt_computeMeshletBounds(&meshlet_vertices[m.vertex_offset], &meshlet_triangles[m.triangle_offset],
            m.triangle_count, pos.data(), pos.size(), sizeof(float)*3);


        // meshlet descriptor
        MeshletDesc mDesc;
        
        // culling 
        mDesc.boundingSphere = { bounds.center, bounds.radius };
        mDesc.cone_axis = { bounds.cone_axis };
        
                            


        
        //                                              todo child count

        

        
        for (uint32_t ver = 0; ver < m.vertex_count; ver++)
        {
            uint32_t vert = meshlet_vertices[ver + m.vertex_offset];
            float3 pso = { pos[vert * 4 + 0], pos[vert * 4 + 1], pos[vert * 4 + 2] };
            uint32_t id = pos[vert * 4 + 3];

            //uint32_t id = meshlet_vertices[m.vertex_offset + (vert * 4) + 3];
            utils::Vertex& oldV = vertices[id];
            if (!(oldV.pos[0] == pso.x && oldV.pos[1] == pso.y && oldV.pos[2] == pso.z)) {
                Log::Error("Scene Vertex != expected");
                fclose(outData);
                return nullptr;
            }

            dummyVert->pos = pso;
            dummyVert->norm = { oldV.N, oldV.uv.x };
            dummyVert->tangent = { oldV.T, oldV.uv.x };

            fwrite(dummyVert, sizeof(Vertex), 1, outData);
        }
        


        fwrite(&meshlet_triangles[m.triangle_offset], m.triangle_count * 3 * sizeof(uint8_t), 1, outData);
        uint8_t rem = m.triangle_count % 4;
        uint8_t rCount = (uint8_t)ceil((float)m.triangle_count / 4.f);
        if (rem > 0) {
            fwrite(zero, sizeof(uint8_t), rem, outData);
        }

        uint32_t vertTriCount = m.vertex_count;
        vertTriCount << (uint8_t)m.triangle_count;
        vertTriCount << rCount;
        mDesc.vertTriCount = vertTriCount;
        vModelDesc->meshlets.push_back(mDesc);
    }
    
    

    
    

    delete dummyVert;
    delete zero;

    Log::Message("Finished Mesh");

    return vModelDesc;
}

std::vector<VModel*> VirtualGeometryBuilder::BuildVG(std::string inPath, std::string name, bool forceUpdate, bool buildAsSingleMesh)
{
    Log::Message("Build Meshlet for: " + inPath);
    utils::Scene scene;
    utils::LoadScene(inPath, scene, false);
    
    std::vector<VModel*> ret;
    ret.reserve(scene.meshes.size());

    if (buildAsSingleMesh) {
        

        ret.push_back(BuildVGImpl(scene.indices, scene.vertices, name, forceUpdate));

        scene.UnloadGeometryData();
        scene.UnloadTextureData();
        return ret;
    }

    ret.reserve(scene.meshes.size());

    uint32_t mID = 0;
    for (utils::Mesh& m : scene.meshes)
    {
        std::vector<unsigned int> indices;
        std::vector<utils::Vertex> vertices;

        indices.reserve(m.indexNum);
        vertices.reserve(m.vertexNum);

        for (size_t i = 0; i < m.indexNum; i++)
        {
            indices.push_back(scene.indices[i + m.indexOffset]);
        }
        for (size_t i = 0; i < m.vertexNum; i++)
        {
            vertices.push_back(scene.vertices[i + m.vertexOffset]);
        }
        
        ret.push_back(BuildVGImpl(indices, vertices, name + "_subM" + std::to_string(mID), forceUpdate));

        mID++;
    }

    scene.UnloadGeometryData();
    scene.UnloadTextureData();

    return ret;
}

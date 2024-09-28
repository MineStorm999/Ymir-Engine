/*
#include "VirtualGeometryBuilder.h"



#include "meshoptimizer.h"

#include "../../Log/Log.h"

#include "MeshletStructs.h"

#include "Utils.h"

#include "../../Assets/exts.h"

#include "../../Common/utils.h"

struct VModelDescHead
{
    //

    uint32_t meshletCount;
};



VModel* VirtualGeometryBuilder::BuildVGImpl(std::vector<unsigned int> indices, std::vector<utils::Vertex> vertices, std::string name, bool forceUpdate, std::string path)
{
    VModel* vModelDesc = new VModel();


    std::string outPath = utils::GetCFullPath(name, utils::CustomFolder::VIRTUALMESH) + VModelExt;

    FILE* outData = fopen(outPath.c_str(), "r");
    if (outData) {
        fclose(outData);
        if (!forceUpdate) {
            Log::Warning("File alsready Exists, stopping");
            //return nullptr;
        }
        else {
            Log::Message("File alsready Exists, continuing");
        }
    }

    std::string vMDescP = utils::GetCFullPath(name, utils::CustomFolder::VIRTUALMESHDESC) + VModelDescExt;
    FILE* vMDescCache = fopen(vMDescP.c_str(), "r");
    if (vMDescCache) {
        if (!forceUpdate) {
            Log::Message("File alsready Exists, using cache");

            VModelDescHead head;
            fread(&head, sizeof(head), 1, vMDescCache);

            vModelDesc->meshlets.resize(head.meshletCount);
            fread(vModelDesc->meshlets.data(), sizeof(MeshletDesc), head.meshletCount, vMDescCache);
            Log::Message("Cached VModel had " + std::to_string(vModelDesc->meshlets.size()) + "meshlets");
            return vModelDesc;
        }
        else {
            Log::Message("File alsready Exists, continuing");
        }
    }
    vMDescCache = fopen(vMDescP.c_str(), "w");
    outData = fopen(outPath.c_str(), "w");

    std::vector<float> pos;
    pos.reserve(vertices.size() * 4);

    for (uint32_t i = 0; i < vertices.size(); i++)
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
        indices.size(), pos.data(), pos.size(), sizeof(float) * 4, maxVerts, maxTries, coneWeight);

    const meshopt_Meshlet& last = meshlets[meshlet_count - 1];

    meshlet_vertices.resize(last.vertex_offset + last.vertex_count);
    meshlet_triangles.resize(last.triangle_offset + ((last.triangle_count * 3 + 3) & ~3));
    meshlets.resize(meshlet_count);

    vModelDesc->meshlets.reserve(meshlets.size());

    Vertex* dummyVert = new Vertex();
    uint8_t* zero = new uint8_t(0);

    uint32_t wPtr = 0;

    int i = 0;
    for (meshopt_Meshlet& m : meshlets)
    {
        i++;
        //Log::Message("Meshlet " + std::to_string(i) + "   Vert Count: " + std::to_string(m.vertex_count) + "   tri count: " + std::to_string(m.triangle_count));

        // optimize
        meshopt_optimizeMeshlet(&meshlet_vertices[m.vertex_offset], &meshlet_triangles[m.triangle_offset], m.triangle_count, m.vertex_count);


        meshopt_Bounds bounds = meshopt_computeMeshletBounds(&meshlet_vertices[m.vertex_offset], &meshlet_triangles[m.triangle_offset],
            m.triangle_count, pos.data(), pos.size(), sizeof(float) * 3);


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
            dummyVert->norm = oldV.N;
            dummyVert->tangent = oldV.T;
            dummyVert->uv = ((uint16_t)(oldV.uv.x * 65536) << 16) | (uint16_t)(oldV.uv.y * 65536);
        }

        fwrite(dummyVert, sizeof(Vertex), 1, outData);
    



        fwrite(&meshlet_triangles[m.triangle_offset], m.triangle_count * 3 * sizeof(uint8_t), 1, outData);
        uint32_t rest = (m.triangle_count * 3) % 4;

        if (rest > 0) {
            fwrite(&meshlet_triangles[m.triangle_offset], sizeof(uint8_t), rest, outData);
        }

        mDesc.clusterOffset = wPtr;
        uint32_t lenght = (sizeof(Vertex) * m.vertex_count) + ((sizeof(uint8_t) * m.triangle_count * 3));
        lenght += sizeof(uint8_t) * rest;
        mDesc.clusterLenght = lenght;

        wPtr += lenght;

        
        uint32_t vertTriCount = (m.vertex_count << 16) | (uint16_t)m.triangle_count;
        
        mDesc.vertTriCount = vertTriCount;
        vModelDesc->meshlets.push_back(mDesc);
    }
    
    

    
    

    delete dummyVert;
    delete zero;

    Log::Message("Finished Mesh");

    VModelDescHead h = { vModelDesc->meshlets.size() };

    fwrite(&h, sizeof(VModelDescHead), 1, vMDescCache);
    fwrite(vModelDesc->meshlets.data(), sizeof(MeshletDesc), vModelDesc->meshlets.size(), vMDescCache);


    vModelDesc->name = name;
    vModelDesc->path = path;
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
        

        ret.push_back(BuildVGImpl(scene.indices, scene.vertices, name, forceUpdate, inPath));

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
        
        VModel* re = BuildVGImpl(indices, vertices, name + "_subM" + std::to_string(mID), forceUpdate, inPath);
        if (!re) {
            continue;
        }

        for (utils::Instance& inst : scene.instances)
        {
            if (inst.meshInstanceIndex == mID) {
                Transform t{};
                t.localPos = inst.position;
                t.localRot = inst.rotation.GetRotationYPR();
                t.localScale = inst.scale;
                re->defaultTransforms.push_back(t);
            }
        }

        ret.push_back(re);

        mID++;
    }

    scene.UnloadGeometryData();
    scene.UnloadTextureData();

    return ret;
}

VModel* VirtualGeometryBuilder::LoadCached(std::string name, std::string path)
{
    Log::Message("Load Cached Virtual Model: " + name);
    VModel* vModelDesc = new VModel();


    std::string outPath = utils::GetCFullPath(name, utils::CustomFolder::VIRTUALMESH) + VModelExt;

    FILE* outData = fopen(outPath.c_str(), "r");
    if (outData) {
        fclose(outData);
    }
    else {
        Log::Error("Model Data Not Cached!!!");
        return nullptr;
    }

    std::string vMDescP = utils::GetCFullPath(name, utils::CustomFolder::VIRTUALMESHDESC) + VModelDescExt;
    FILE* vMDescCache = fopen(vMDescP.c_str(), "r");
    if (vMDescCache) {
        VModelDescHead head;
        fread(&head, sizeof(head), 1, vMDescCache);

        vModelDesc->meshlets.resize(head.meshletCount);
        fread(vModelDesc->meshlets.data(), sizeof(MeshletDesc), head.meshletCount, vMDescCache);
        Log::Message("Cached VModel had " + std::to_string(vModelDesc->meshlets.size()) + "meshlets");
        return vModelDesc;
    }
    Log::Error("Model Descriptors Not Cached!!!");
    return nullptr;
}

VModel::VModel()
{
    type = AssetType::VirtualModel;
}
*/
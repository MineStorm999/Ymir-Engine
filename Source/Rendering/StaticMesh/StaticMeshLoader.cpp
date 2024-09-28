#include "StaticMeshLoader.h"

#include "Assets/AssetManager.h"
#include "NRIFramework.h"
#include "Log/Log.h"
#include "meshoptimizer.h"
#include <fstream>

std::vector<AModel*> MeshLoader::ImportModel(std::string path, std::string savePath, std::string name, MeshImportSettings settings)
{
    std::vector<AModel*> ret;
    ret.reserve(1);
    FILE* loadF = fopen(path.c_str(), "r");
    if (!loadF) {
        Log::Error("Mesh Importer", "couldn't open " + path);
        ret.push_back(nullptr);
        return ret;
    }
    fclose(loadF);

    utils::Scene packedScene;
    utils::LoadScene(path, packedScene, false);

    

    if (settings.singleModel) {
        ret.push_back(nullptr);
        return ret;
    }

    // for mesh
    uint32_t idx = 0;
    for (utils::Mesh& m : packedScene.meshes)
    {
        size_t index_count = m.indexNum;
        size_t vertex_countt = m.vertexNum;
        size_t vertex_count = 0;
        std::vector<utils::Index> indices(m.indexNum);
        std::vector<utils::Vertex> vertices(m.vertexNum);


        if (settings.optimize) {
            std::vector<unsigned int> remap(index_count); // allocate temporary memory for the remap table

            vertex_count = meshopt_generateVertexRemap(&remap[0], &packedScene.indices[m.indexOffset], index_count, &packedScene.vertices[m.vertexOffset], vertex_countt, sizeof(utils::Vertex));


            meshopt_remapIndexBuffer(&indices[0], &packedScene.indices[m.indexOffset], index_count, &remap[0]);
            meshopt_remapVertexBuffer(&vertices[0], &packedScene.vertices[m.vertexOffset], vertex_countt, sizeof(utils::Vertex), &remap[0]);


            // cache
            meshopt_optimizeVertexCache(&indices[0], &indices[0], index_count, vertex_count);

            // owerdraw
            meshopt_optimizeOverdraw(&indices[0], &indices[0], index_count, &vertices[0].pos[0], vertex_count, sizeof(utils::Vertex), 1.05f);

            // fetch opt.
            meshopt_optimizeVertexFetch(&vertices[0], &indices[0], index_count, &vertices[0], vertex_count, sizeof(utils::Vertex));

            /* Shadow optimisation
            *
            std::vector<unsigned int> shadow_indices(index_count);
            // note: this assumes Vertex starts with float3 positions and should be adjusted accordingly for quantized positions
            meshopt_generateShadowIndexBuffer(&shadow_indices[0], indices, index_count, &vertices[0].x, vertex_count, sizeof(float) * 3, sizeof(Vertex));*/

            vertices.resize(vertex_count);
        }
        else {
            for (uint32_t i = 0; i < index_count; i++)
            {
                indices[i] = packedScene.indices[m.indexOffset + i];
            }
            for (uint32_t i = 0; i < vertex_countt; i++)
            {
                vertices[i] = packedScene.vertices[m.vertexOffset + i];
            }
        }

        std::string actualPath = savePath + "/" + name + std::to_string(idx++) + "." + ASSET_SHORT;
        std::ofstream ofSr;
        ofSr.open(actualPath);
        ofSr.close();
        FILE* outF = fopen(actualPath.c_str(), "wb");
        if (!outF) {
            continue;
        }

        AModel* model = new AModel();
        model->name = name + std::to_string(idx);

        model->path = actualPath;
        model->originalPath = path;
        model->indexCount = index_count;
        model->vertCount = vertex_count;
        
        if (settings.compress) {
            // compression
            std::vector<unsigned char> vbuf(meshopt_encodeVertexBufferBound(vertex_count, sizeof(utils::Vertex)));
            vbuf.resize(meshopt_encodeVertexBuffer(&vbuf[0], vbuf.size(), &vertices[0], vertex_count, sizeof(utils::Vertex)));

            std::vector<unsigned char> ibuf(meshopt_encodeIndexBufferBound(index_count, vertex_count));
            ibuf.resize(meshopt_encodeIndexBuffer(&ibuf[0], ibuf.size(), &indices[0], index_count));


            // head
            uint8_t head[2] = { A_DISC_VERI0, A_DISC_VERI1 };
            uint8_t flag = (uint8_t)AssetLoadFlags::Compressed;
            AssetID id = AssetManager::RegisterAsset(dynamic_cast<AssetBase*>(model));

            
            fwrite(head, 2, 1, outF);
            fwrite(&flag, 1, 1, outF);
            fwrite(&id, 4, 1, outF);

            // data
            model->off = 7;
            model->lenght = ibuf.size() + vbuf.size();

            fwrite(ibuf.data(), sizeof(unsigned char), ibuf.size(), outF);
            fwrite(vbuf.data(), sizeof(unsigned char), vbuf.size(), outF);
        }
        else {
            // head
            uint8_t head[2] = { A_DISC_VERI0, A_DISC_VERI1 };
            uint8_t flag = (uint8_t)AssetLoadFlags::None;
            AssetID id = AssetManager::RegisterAsset(dynamic_cast<AssetBase*>(model));

            fwrite(head, 2, 1, outF);
            fwrite(&flag, 1, 1, outF);
            fwrite(&id, 4, 1, outF);

            // data
            model->off = 7;
            model->lenght = indices.size() + vertices.size();

            fwrite(indices.data(), sizeof(unsigned int), indices.size(), outF);
            fwrite(vertices.data(), sizeof(utils::Vertex), vertices.size(), outF);
        }
        ret.push_back(model);
    }
    
    return ret;
}


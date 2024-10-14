#include "StaticMeshLoader.h"

#include "Assets/AssetManager.h"
#include "NRIFramework.h"
#include "Log/Log.h"
#include "meshoptimizer.h"
#include <fstream>

#include "Stb_Image/stb_image.h"

void Importer::ImportModel(std::string path, std::string savePath, std::string name, MeshImportSettings settings, AssetID* out)
{
    FILE* loadF = fopen(path.c_str(), "r");
    if (!loadF) {
        Log::Error("Mesh Importer", "couldn't open " + path);
        return;
    }
    fclose(loadF);

    utils::Scene packedScene;
    utils::LoadScene(path, packedScene, false);



    if (settings.createFolder) {
        savePath.append("/" + name);
        std::filesystem::create_directories(savePath);
    }

    std::vector<AssetID> textures;
    textures.reserve(packedScene.textures.size());

    std::vector<AssetID> materials;
    materials.reserve(packedScene.materials.size());

    std::string textureSavePath = savePath + "/" + "Textures/";
    std::filesystem::create_directories(textureSavePath);

    uint32_t i = 0;
    for (auto& tex : packedScene.textures)
    {
        textures.push_back(ImportTexture(tex->name, textureSavePath, name + "_texture_" + std::to_string(i++)));
    }

    i = 0;
    std::string materialSavePath = savePath + "/" + "Materials/";
    for (auto& mat : packedScene.materials)
    {
        ImportMaterial(savePath, name + "_material_" + std::to_string(i++), &mat, textures);
    }

    if (settings.singleModel) {
        size_t index_count = packedScene.indices.size();
        size_t vertex_countt = packedScene.vertices.size();
        size_t vertex_count = 0;
        std::vector<utils::Index> indices(index_count);
        std::vector<utils::Vertex> vertices(vertex_countt);


        if (settings.optimize) {
            std::vector<unsigned int> remap(index_count); // allocate temporary memory for the remap table

            vertex_count = meshopt_generateVertexRemap(&remap[0], &packedScene.indices[0], index_count, &packedScene.vertices[0], vertex_countt, sizeof(utils::Vertex));


            meshopt_remapIndexBuffer(&indices[0], &packedScene.indices[0], index_count, &remap[0]);
            meshopt_remapVertexBuffer(&vertices[0], &packedScene.vertices[0], vertex_countt, sizeof(utils::Vertex), &remap[0]);


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
                indices[i] = packedScene.indices[i];
            }
            for (uint32_t i = 0; i < vertex_countt; i++)
            {
                vertices[i] = packedScene.vertices[i];
            }
        }

        std::string actualPath = savePath + "/" + name + "." + ASSET_SHORT;
        std::ofstream ofSr;
        ofSr.open(actualPath);
        ofSr.close();
        FILE* outF = fopen(actualPath.c_str(), "wb");
        if (!outF) {
            return;
        }

        AModel* model = new AModel();
        model->name = name;

        model->path = savePath;
        model->originalPath = path;
        model->vertCount = vertex_count;


        LOD lod0;
        lod0.distance = 10;
        lod0.indexOffset = 0;
        lod0.lenght = index_count;
        model->lods.push_back(lod0);

        if (settings.autoLOD) {
            float simp = 1 / settings.lodCount;

            for (uint32_t i = 0; i < settings.lodCount; i++)
            {
                //Log::Message("Importer", "Create LOD" + std::to_string(i));
                std::vector<unsigned int> lodIdx = CreateLOD(1 - (i * simp), &indices[0], index_count, &vertices[0], vertex_count);
                LOD lodn;
                lodn.distance = (i + 2) * 10;
                lodn.indexOffset = indices.size();
                lodn.lenght = lodIdx.size();
                model->lods.push_back(lodn);

                indices.reserve(indices.size() + lodn.lenght);
                for (uint32_t& idx : lodIdx)
                {
                    indices.push_back(idx);
                }
            }
        }

        model->indexCount = index_count;

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
            if (out) {
                *out = id;
            }

            fwrite(head, 2, 1, outF);
            fwrite(&flag, 1, 1, outF);
            fwrite(&id, 4, 1, outF);

            // data
            model->off = 7;
            model->lenght = ibuf.size() + vbuf.size();
            model->iBuffLenght = ibuf.size();
            fwrite(ibuf.data(), sizeof(unsigned char), ibuf.size(), outF);
            fwrite(vbuf.data(), sizeof(unsigned char), vbuf.size(), outF);

            fclose(outF);
        }
        else {
            // head
            uint8_t head[2] = { A_DISC_VERI0, A_DISC_VERI1 };
            uint8_t flag = (uint8_t)AssetLoadFlags::None;
            AssetID id = AssetManager::RegisterAsset(dynamic_cast<AssetBase*>(model));
            if (out) {
                *out = id;
            }

            fwrite(head, 2, 1, outF);
            fwrite(&flag, 1, 1, outF);
            fwrite(&id, 4, 1, outF);

            // data
            model->off = 7;
            model->lenght = (indices.size() * sizeof(uint32_t)) + ((vertices.size() * sizeof(utils::Vertex)));
            model->iBuffLenght = indices.size();
            fwrite(indices.data(), sizeof(unsigned int), indices.size(), outF);
            fwrite(vertices.data(), sizeof(utils::Vertex), vertices.size(), outF);

            fclose(outF);
        }
        return;
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
            Log::Error("Mesh Importer", "Couldnt create: " + actualPath);
            continue;
        }

        AModel* model = new AModel();
        model->name = name + std::to_string(idx);

        model->path = savePath;
        model->originalPath = path;
        
        model->vertCount = vertex_count;
        

        LOD lod0;
        lod0.distance = 10;
        lod0.indexOffset = 0;
        lod0.lenght = index_count;
        model->lods.push_back(lod0);

        if (settings.autoLOD) {
            float simp = 1 / settings.lodCount;

            for (uint32_t i = 0; i < settings.lodCount; i++)
            {
                //Log::Message("Importer", "Create LOD" + std::to_string(i));
                std::vector<unsigned int> lodIdx = CreateLOD(i* simp, &indices[0], index_count, &vertices[0], vertex_count);
                LOD lodn;
                lodn.distance = (i + 2) * 10;
                lodn.indexOffset = indices.size();
                lodn.lenght = lodIdx.size();
                model->lods.push_back(lodn);

                indices.reserve(indices.size() + lodn.lenght);
                for (uint32_t& idx : lodIdx)
                {
                    indices.push_back(idx);
                }
            }
        }

        model->indexCount = index_count;

        if (settings.compress) {
            // compression
            std::vector<unsigned char> vbuf(meshopt_encodeVertexBufferBound(vertex_count, sizeof(utils::Vertex)));
            vbuf.resize(meshopt_encodeVertexBuffer(&vbuf[0], vbuf.size(), &vertices[0], vertex_count, sizeof(utils::Vertex)));

            std::vector<unsigned char> ibuf(meshopt_encodeIndexBufferBound(indices.size(), vertex_count));
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
            model->iBuffLenght = ibuf.size();
            fwrite(ibuf.data(), sizeof(unsigned char), ibuf.size(), outF);
            fwrite(vbuf.data(), sizeof(unsigned char), vbuf.size(), outF);
            fclose(outF);
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
            model->lenght = (indices.size() * sizeof(uint32_t)) + ((vertices.size() * sizeof(utils::Vertex)));
            model->iBuffLenght = indices.size();
            fwrite(indices.data(), sizeof(unsigned int), indices.size(), outF);
            fwrite(vertices.data(), sizeof(utils::Vertex), vertices.size(), outF);
            fclose(outF);
        }
    }
    return;
}

std::vector<unsigned int> Importer::CreateLOD(float siplificationAmmount, const unsigned int* originalIndices, uint32_t indexCount, const void* originalVerices, uint32_t vertexCount)
{

    size_t target_index_count = size_t(indexCount * siplificationAmmount);
    float target_error = 1e-2f;

    std::vector<unsigned int> lod(indexCount);
    float lod_error = 0.f;
    lod.resize(meshopt_simplify(&lod[0], originalIndices, indexCount, (float*)originalVerices, vertexCount, sizeof(utils::Vertex), target_index_count, target_error, /* options= */ 0, &lod_error));

    return lod;
}

// only temporary
AssetID Importer::ImportTexture(std::string path, std::string savePath, std::string name)
{
    uint32_t off = path.find_last_of(".");
    if (off == std::string::npos) {
        return INVALID_ASSET_ID;
    }
    else {
        std::string fileE = path.substr(off, path.size() - off);
        std::string save = savePath + name + fileE;
        if (!std::filesystem::copy_file(path, save)) {
            return INVALID_ASSET_ID;
        }
    }

    ATexture* tex = new ATexture();
    tex->lenght = 0xffffffff;
    tex->name = name;
    tex->originalPath = path;
    tex->path = savePath;
    
    uint8_t head[2] = { A_DISC_VERI0, A_DISC_VERI1 };
    uint8_t flag = (uint8_t)AssetLoadFlags::None;
    AssetID id = AssetManager::RegisterAsset(dynamic_cast<AssetBase*>(tex));

    FILE* f = fopen(tex->GetActualPath().c_str(), "wb");
    if (!f){
        Log::Error("Importer(Texture)", "Couldn't create: " + tex->GetActualPath());
        return INVALID_ASSET_ID;
    }

    if (fwrite(head, 1, 2, f) != 2) {
        Log::Error("Importer(Texture)", "Couldn't write: " + tex->GetActualPath());
        fclose(f);
        return INVALID_ASSET_ID;
    }
    if (fwrite(&flag, 1, 1, f) != 1) {
        Log::Error("Importer(Texture)", "Couldn't write: " + tex->GetActualPath());
        fclose(f);
        return INVALID_ASSET_ID;
    }
    if (fwrite(&id, 4, 1, f) != 1) {
        Log::Error("Importer(Texture)", "Couldn't write: " + tex->GetActualPath());
        fclose(f);
        return INVALID_ASSET_ID;
    }
    //fwrite(&flag, 1, 1, f);
    //fwrite(&id, 4, 1, f);
    fclose(f);
    return id;
}

AssetID Importer::ImportTexture(utils::Texture texture, std::string savePath)
{
    return INVALID_ASSET_ID;
}

AssetID Importer::ImportMaterial(std::string savePath, std::string name, utils::Material* material, std::vector<AssetID>& originalTextureIds)
{
    if (!material) {
        return INVALID_ASSET_ID;
    }
    AMaterial* mat = new AMaterial();
    mat->normalTex = originalTextureIds[material->normalTexIndex];
    mat->baseColorTex = originalTextureIds[material->baseColorTexIndex];
    mat->emissiveTex = originalTextureIds[material->emissiveTexIndex];
    mat->roughnessMetalnessTex = originalTextureIds[material->roughnessMetalnessTexIndex];

    mat->baseColorAndMetallic = material->baseColorAndMetalnessScale;
    mat->emissiveColorAndRoughness = material->emissiveAndRoughnessScale;
    mat->alphaMode = (uint8_t)material->alphaMode;

    uint8_t head[2] = { A_DISC_VERI0, A_DISC_VERI1 };
    uint8_t flag = (uint8_t)AssetLoadFlags::None;
    AssetID id = AssetManager::RegisterAsset(dynamic_cast<AssetBase*>(mat));

    FILE* f = fopen(mat->GetActualPath().c_str(), "wb");
    if (!f) {
        Log::Error("Importer(Texture)", "Couldn't create: " + mat->GetActualPath());
        return INVALID_ASSET_ID;
    }

    if (fwrite(head, 1, 2, f) != 2) {
        Log::Error("Importer(Texture)", "Couldn't write: " + mat->GetActualPath());
        fclose(f);
        return INVALID_ASSET_ID;
    }
    if (fwrite(&flag, 1, 1, f) != 1) {
        Log::Error("Importer(Texture)", "Couldn't write: " + mat->GetActualPath());
        fclose(f);
        return INVALID_ASSET_ID;
    }
    if (fwrite(&id, 4, 1, f) != 1) {
        Log::Error("Importer(Texture)", "Couldn't write: " + mat->GetActualPath());
        fclose(f);
        return INVALID_ASSET_ID;
    }
    //fwrite(&flag, 1, 1, f);
    //fwrite(&id, 4, 1, f);
    fclose(f);
}

AssetID Importer::ImportMaterial(std::string savePath, std::string name, const std::vector<std::string>& texturePaths)
{
    return INVALID_ASSET_ID;
}


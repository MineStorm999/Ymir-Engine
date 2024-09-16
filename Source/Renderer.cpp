#include "Renderer.hpp"
#include "Editor/ToolBarWindowManager.h"
#include "Log/Log.h"

/// QuadLayout
/// 
/// 0---1
/// |  /|
/// | / |
/// |/  |
/// 2---3
/// 
/// QuadLayout

#define MESHLETDATASIZE 2000000

uint32_t indices[6]{
    0,1,2, // 1. triangle
    3,2,1
};
struct QuatVertex
{
    float2 pos;
    float2 uv;
};



QuatVertex windowVerts[4] = {};

Renderer::~Renderer()
{
    NRI.WaitForIdle(*m_CommandQueue);
    Log::Message("Renderer", "Shutting down BiggestRequest Was: " + std::to_string(biggestSize) + "bytes");
    // free all memory
    free(allRenderedEntites0);
    free(allRenderedEntites1);
    free(lastFrameEntitiesID);

    free(VGBuffer0);
    free(VGBuffer1);

    // destroy nri stuff
    for (Frame& frame : m_Frames)
    {
        NRI.DestroyCommandBuffer(*frame.cmdBuffEvalStage);
        NRI.DestroyCommandAllocator(*frame.cmdAllocEvalStage);

        NRI.DestroyCommandBuffer(*frame.cmdBuffRenderStage);
        NRI.DestroyCommandAllocator(*frame.cmdAllocRenderStage);
    }

    for (uint32_t i = 0; i < m_SwapChainBuffers.size(); i++)
        NRI.DestroyDescriptor(*m_SwapChainBuffers[i].colorAttachment);

    for (size_t i = 0; i < m_descriptors.size(); i++)
        NRI.DestroyDescriptor(*m_descriptors[i]);

    for (size_t i = 0; i < m_Textures.size(); i++)
        NRI.DestroyTexture(*m_Textures[i]);

    for (size_t i = 0; i < m_buffers.size(); i++)
        NRI.DestroyBuffer(*m_buffers[i]);

    for (size_t i = 0; i < m_memoryAllocations.size(); i++)
        NRI.FreeMemory(*m_memoryAllocations[i]);

    NRI.DestroyDescriptor(*m_visBuff.dataDesc);
    NRI.DestroyDescriptor(*m_visBuff.depthDesc);

    NRI.DestroyTexture(*m_visBuff.data);
    NRI.DestroyTexture(*m_visBuff.depth);

    

    NRI.FreeMemory(*m_visBuff.dataMem);
    NRI.FreeMemory(*m_visBuff.depthMem);

    NRI.DestroyPipeline(*m_cachedRenderPipeline);
    NRI.DestroyPipeline(*m_cullingPipeline);
    NRI.DestroyPipeline(*m_visibilityBuffPipeline);
    NRI.DestroyPipeline(*m_materialPipeline);
    NRI.DestroyPipeline(*m_mergePipeline);

    NRI.DestroyQueryPool(*m_QueryPool);


    NRI.DestroyPipelineLayout(*m_cachedRenderLayout);
    NRI.DestroyPipelineLayout(*m_cullingLayout);
    NRI.DestroyPipelineLayout(*m_visibilityBuffLayout);
    NRI.DestroyPipelineLayout(*m_materialLayout);
    NRI.DestroyPipelineLayout(*m_mergeLayout);


    NRI.DestroyDescriptorPool(*m_DescriptorPool);
    NRI.DestroyFence(*m_FrameFence);
    NRI.DestroySwapChain(*m_SwapChain);
    NRI.DestroyStreamer(*m_Streamer);

    DestroyUI(NRI);

    nri::nriDestroyDevice(*m_Device);
}

bool Renderer::Initialize(nri::GraphicsAPI graphicsAPI)
{
    CachedCluster c;
    c.clusterID = 0xffffffff;
    c.modelID = 0xffffffff;
    c.clusterOffSet = 0xffffffff;
    nullCluster = c;
    // init asset manager, log
    WindowManager::Init();

    EntityManager::SetWorld(new ECSWorld());

    /*Log::Error("Renderer", "You didnt implemented free frunction!!!!!");
    exit(EXIT_FAILURE);*/
    // init geometry streamer, sets model render ids, creates model desc & meshlet desc buffer      // todo meshlet childs (lod hyrachie)
    m_vGeomStreamer = new VirtualGeometryStreamer();
    m_vGeomStreamer->Init();

    const uint32_t windowWidth = GetWindowResolution().x;
    const uint32_t windowHeight = GetWindowResolution().y;

    QuatVertex curVert = {};
    curVert.pos = { 1, 1 };
    curVert.uv = { 1, 1 };
    windowVerts[0] = curVert;

    curVert.pos = { 0, 1 };
    curVert.uv = { 0, 1 };
    windowVerts[1] = curVert;

    curVert.pos = { 1, 1 };
    curVert.uv = { 1, 0 };
    windowVerts[2] = curVert;

    curVert.pos = { 0, 0 };
    curVert.uv = { 0, 0 };
    windowVerts[3] = curVert;
   
    if (graphicsAPI == nri::GraphicsAPI::D3D11) {
        printf("This Project supports only D3D12 and Vulkan.");
        return false;
    }

    nri::AdapterDesc bestAdapterDesc = {};
    uint32_t adapterDescsNum = 1;
    NRI_ABORT_ON_FAILURE(nri::nriEnumerateAdapters(&bestAdapterDesc, adapterDescsNum));

    // Device
    nri::DeviceCreationDesc deviceCreationDesc = {};
    deviceCreationDesc.graphicsAPI = graphicsAPI;
    deviceCreationDesc.enableGraphicsAPIValidation = true;
    deviceCreationDesc.enableNRIValidation = true;

    deviceCreationDesc.enableD3D11CommandBufferEmulation = D3D11_COMMANDBUFFER_EMULATION;
    deviceCreationDesc.enableD3D12DrawParametersEmulation = true;
    deviceCreationDesc.spirvBindingOffsets = SPIRV_BINDING_OFFSETS;
    deviceCreationDesc.adapterDesc = &bestAdapterDesc;
    deviceCreationDesc.memoryAllocatorInterface = m_MemoryAllocatorInterface;
    NRI_ABORT_ON_FAILURE(nri::nriCreateDevice(deviceCreationDesc, m_Device));

    // NRI
    NRI_ABORT_ON_FAILURE(nri::nriGetInterface(*m_Device, NRI_INTERFACE(nri::CoreInterface), (nri::CoreInterface*)&NRI));
    NRI_ABORT_ON_FAILURE(nri::nriGetInterface(*m_Device, NRI_INTERFACE(nri::HelperInterface), (nri::HelperInterface*)&NRI));
    NRI_ABORT_ON_FAILURE(nri::nriGetInterface(*m_Device, NRI_INTERFACE(nri::StreamerInterface), (nri::StreamerInterface*)&NRI));
    NRI_ABORT_ON_FAILURE(nri::nriGetInterface(*m_Device, NRI_INTERFACE(nri::SwapChainInterface), (nri::SwapChainInterface*)&NRI));

    // Create streamer
    nri::StreamerDesc streamerDesc = {};
    streamerDesc.dynamicBufferMemoryLocation = nri::MemoryLocation::HOST_UPLOAD;
    streamerDesc.dynamicBufferUsageBits = nri::BufferUsageBits::VERTEX_BUFFER | nri::BufferUsageBits::INDEX_BUFFER/* | nri::BufferUsageBits::SHADER_RESOURCE | nri::BufferUsageBits::SHADER_RESOURCE_STORAGE*/;
    streamerDesc.constantBufferMemoryLocation = nri::MemoryLocation::HOST_UPLOAD;
    streamerDesc.frameInFlightNum = BUFFERED_FRAME_MAX_NUM;
    
    NRI_ABORT_ON_FAILURE(NRI.CreateStreamer(*m_Device, streamerDesc, m_Streamer));

    // Command queue
    NRI_ABORT_ON_FAILURE(NRI.GetCommandQueue(*m_Device, nri::CommandQueueType::GRAPHICS, m_CommandQueue));

    // Fences
    NRI_ABORT_ON_FAILURE(NRI.CreateFence(*m_Device, 0, m_FrameFence));

    m_DepthFormat = nri::GetSupportedDepthFormat(NRI, *m_Device, 24, false);

    { // Swap chain
        nri::SwapChainDesc swapChainDesc = {};
        swapChainDesc.window = GetWindow();
        swapChainDesc.commandQueue = m_CommandQueue;
        swapChainDesc.format = nri::SwapChainFormat::BT709_G22_10BIT;
        swapChainDesc.verticalSyncInterval = m_VsyncInterval;
        swapChainDesc.width = (uint16_t)GetWindowResolution().x;
        swapChainDesc.height = (uint16_t)GetWindowResolution().y;
        swapChainDesc.textureNum = SWAP_CHAIN_TEXTURE_NUM;
        NRI_ABORT_ON_FAILURE(NRI.CreateSwapChain(*m_Device, swapChainDesc, m_SwapChain));
    }

    uint32_t swapChainTextureNum;
    nri::Texture* const* swapChainTextures = NRI.GetSwapChainTextures(*m_SwapChain, swapChainTextureNum);
    nri::Format swapChainFormat = NRI.GetTextureDesc(*swapChainTextures[0]).format;

    // Buffered resources
    for (Frame& frame : m_Frames)
    {
        NRI_ABORT_ON_FAILURE(NRI.CreateCommandAllocator(*m_CommandQueue, frame.cmdAllocEvalStage));
        NRI_ABORT_ON_FAILURE(NRI.CreateCommandBuffer(*frame.cmdAllocEvalStage, frame.cmdBuffEvalStage));

        NRI_ABORT_ON_FAILURE(NRI.CreateCommandAllocator(*m_CommandQueue, frame.cmdAllocRenderStage));
        NRI_ABORT_ON_FAILURE(NRI.CreateCommandBuffer(*frame.cmdAllocRenderStage, frame.cmdBuffRenderStage));
    }

    // pipelines
    const nri::DeviceDesc& deviceDesc = NRI.GetDeviceDesc(*m_Device);
    utils::ShaderCodeStorage shaderCodeStorage;
    {
        {
            Log::Message("Renderer", "Create pipline layouts");
            // global descriptor ranges
            nri::DescriptorRangeDesc globalDescriptorRange[4] = {};
            globalDescriptorRange[0] = { (uint32_t)((deviceDesc.graphicsAPI == nri::GraphicsAPI::D3D12) ? 0 : 0), 1, nri::DescriptorType::CONSTANT_BUFFER, nri::StageBits::ALL };
            globalDescriptorRange[1] = { 0, 1, nri::DescriptorType::SAMPLER, nri::StageBits::ALL };
            globalDescriptorRange[2] = { 0, GetStructuredBuffCount(), nri::DescriptorType::STRUCTURED_BUFFER, nri::StageBits::ALL };
            globalDescriptorRange[3] = { 0, GetRWStructuredBuffCount(), nri::DescriptorType::STORAGE_STRUCTURED_BUFFER, nri::StageBits::ALL };
            //globalDescriptorRange[4] = { 0, 2, nri::DescriptorType::TEXTURE, nri::StageBits::ALL, true, true };


            nri::DescriptorRangeDesc visBuffDescRange[2] = {};
            visBuffDescRange[0] = { 0, 2, nri::DescriptorType::STORAGE_TEXTURE, nri::StageBits::ALL, true, true };
            visBuffDescRange[1] = { 0, 1, nri::DescriptorType::SAMPLER, nri::StageBits::ALL };

            nri::DescriptorRangeDesc windowRenderData[1] = {};
            visBuffDescRange[0] = { 0, 2, nri::DescriptorType::STRUCTURED_BUFFER, nri::StageBits::ALL };
            
            // need to add texture and material support
            //nri::DescriptorRangeDesc textureDescriptorRange[1] = {};
            //textureDescriptorRange[0] = { 0, 512, nri::DescriptorType::TEXTURE, nri::StageBits::ALL, true, true };

            //nri::DescriptorRangeDesc readbackDescriptorRange[1] = {};
            //readbackDescriptorRange[0] = { 0, 512, nri::DescriptorType::, nri::StageBits::ALL, true, true };

            /*nri::DescriptorRangeDesc cachedInstanceDescriptorRange[1] = {};
            cachedInstanceDescriptorRange[0] = { 0, 1, nri::DescriptorType::STRUCTURED_BUFFER, nri::StageBits::ALL };*/

            nri::DescriptorSetDesc defaultDescriptorSetDescs[] =
            {
                {0, globalDescriptorRange, helper::GetCountOf(globalDescriptorRange)},
                {1, visBuffDescRange, helper::GetCountOf(visBuffDescRange)},
                //{2, cachedInstanceDescriptorRange, helper::GetCountOf(cachedInstanceDescriptorRange) }
            };

            nri::DescriptorSetDesc mergeDescriptorSetDescs[] =
            {
                {0, visBuffDescRange, helper::GetCountOf(visBuffDescRange)},
                //{1, windowRenderData, helper::GetCountOf(windowRenderData)}

            };

            nri::DescriptorSetDesc materialDescriptorSetDescs[] =
            {
                {0, globalDescriptorRange, helper::GetCountOf(globalDescriptorRange)},
                {1, visBuffDescRange, helper::GetCountOf(visBuffDescRange)},
                //{2, cachedInstanceDescriptorRange, helper::GetCountOf(cachedInstanceDescriptorRange) }
                //{3, textureDescriptorRange, helper::GetCountOf(textureDescriptorRange) }

            };

            /*nri::DescriptorSetDesc cachedDescSetDescs[] =
            {
                {0, globalDescriptorRange, helper::GetCountOf(globalDescriptorRange)},
                {1, textureDescriptorRange, helper::GetCountOf(textureDescriptorRange)},
                {2, cachedInstanceDescriptorRange, helper::GetCountOf(cachedInstanceDescriptorRange)}
            };*/


            // cached render pipeline
            nri::PipelineLayoutDesc cachedRenderLayoutDesc = {};
            cachedRenderLayoutDesc.descriptorSetNum = helper::GetCountOf(defaultDescriptorSetDescs);
            cachedRenderLayoutDesc.descriptorSets = defaultDescriptorSetDescs;
            cachedRenderLayoutDesc.shaderStages = nri::StageBits::COMPUTE_SHADER;
            //cachedRenderLayoutDesc.enableD3D12DrawParametersEmulation = true;

                NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, cachedRenderLayoutDesc, m_cachedRenderLayout));


            // culling pipeline desc
            nri::PipelineLayoutDesc cullingLayoutDesc = {};
            cullingLayoutDesc.descriptorSetNum = helper::GetCountOf(defaultDescriptorSetDescs);
            cullingLayoutDesc.descriptorSets = defaultDescriptorSetDescs;
            cullingLayoutDesc.shaderStages = nri::StageBits::COMPUTE_SHADER;
            //cullingLayoutDesc.enableD3D12DrawParametersEmulation = true;

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, cullingLayoutDesc, m_cullingLayout));

            // vis buff pipeline desc
            nri::PipelineLayoutDesc visBuffLayoutDesc = {};
            visBuffLayoutDesc.descriptorSetNum = helper::GetCountOf(defaultDescriptorSetDescs);
            visBuffLayoutDesc.descriptorSets = defaultDescriptorSetDescs;
            visBuffLayoutDesc.shaderStages = nri::StageBits::COMPUTE_SHADER;
            //cullingLayoutDesc.enableD3D12DrawParametersEmulation = true;

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, visBuffLayoutDesc, m_visibilityBuffLayout));

            // material pipeline desc
            nri::PipelineLayoutDesc materialLayoutDesc = {};
            materialLayoutDesc.descriptorSetNum = helper::GetCountOf(materialDescriptorSetDescs);
            materialLayoutDesc.descriptorSets = materialDescriptorSetDescs;
            materialLayoutDesc.shaderStages = nri::StageBits::COMPUTE_SHADER;
            //cullingLayoutDesc.enableD3D12DrawParametersEmulation = true;

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, materialLayoutDesc, m_materialLayout));

            // merge pipeline desc
            nri::PipelineLayoutDesc mergeLayoutDesc = {};
            mergeLayoutDesc.descriptorSetNum = helper::GetCountOf(mergeDescriptorSetDescs);
            mergeLayoutDesc.descriptorSets = mergeDescriptorSetDescs;
            mergeLayoutDesc.shaderStages = nri::StageBits::VERTEX_SHADER | nri::StageBits::FRAGMENT_SHADER;
            mergeLayoutDesc.enableD3D12DrawParametersEmulation = true;

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, mergeLayoutDesc, m_mergeLayout));
        }
    
        // merge stage
        Log::Message("Renderer", "Create merge pipeline");
        {
            // vertex layout
            nri::VertexStreamDesc vertexStreamDesc = {};
            vertexStreamDesc.bindingSlot = 0;
            vertexStreamDesc.stride = sizeof(utils::Vertex);

            nri::VertexAttributeDesc vertexAttributeDesc[2] = {};
            {
                vertexAttributeDesc[0].format = nri::Format::RG32_SFLOAT;
                vertexAttributeDesc[0].offset = helper::GetOffsetOf(&QuatVertex::pos);
                vertexAttributeDesc[0].d3d = { "POSITION", 0 };
                vertexAttributeDesc[0].vk = { 0 };

                vertexAttributeDesc[1].format = nri::Format::RG32_SFLOAT;
                vertexAttributeDesc[1].offset = helper::GetOffsetOf(&QuatVertex::uv);
                vertexAttributeDesc[1].d3d = { "UV", 0 };
                vertexAttributeDesc[1].vk = { 1 };
            }

            nri::VertexInputDesc vertexInputDesc = {};
            vertexInputDesc.attributes = vertexAttributeDesc;
            vertexInputDesc.attributeNum = (uint8_t)helper::GetCountOf(vertexAttributeDesc);
            vertexInputDesc.streams = &vertexStreamDesc;
            vertexInputDesc.streamNum = 1;

            nri::InputAssemblyDesc inputAssemblyDesc = {};
            inputAssemblyDesc.topology = nri::Topology::TRIANGLE_LIST;

            nri::RasterizationDesc rasterizationDesc = {};
            rasterizationDesc.viewportNum = 1;
            rasterizationDesc.fillMode = nri::FillMode::SOLID;
            rasterizationDesc.cullMode = nri::CullMode::NONE;
            rasterizationDesc.frontCounterClockwise = true;

            nri::MultisampleDesc multisampleDesc = {};
            multisampleDesc.sampleNum = 1;
            multisampleDesc.sampleMask = nri::ALL_SAMPLES;

            nri::ColorAttachmentDesc colorAttachmentDesc = {};
            colorAttachmentDesc.format = swapChainFormat;
            colorAttachmentDesc.colorWriteMask = nri::ColorWriteBits::RGBA;

            nri::OutputMergerDesc outputMergerDesc = {};
            outputMergerDesc.colorNum = 1;
            outputMergerDesc.color = &colorAttachmentDesc;
            outputMergerDesc.depthStencilFormat = m_DepthFormat;
            outputMergerDesc.depth.write = true;
            outputMergerDesc.depth.compareFunc = CLEAR_DEPTH == 1.0f ? nri::CompareFunc::LESS : nri::CompareFunc::GREATER;

            nri::ShaderDesc mergeShaderStages[] =
            {
                utils::LoadShader(deviceDesc.graphicsAPI, "YMIR_TriangleMerger.vs", shaderCodeStorage),
                utils::LoadShader(deviceDesc.graphicsAPI, "YMIR_TriangleMerger.fs", shaderCodeStorage),
            };


            nri::GraphicsPipelineDesc mergePipelineDesc = {};
            mergePipelineDesc.pipelineLayout = m_mergeLayout;
            mergePipelineDesc.vertexInput = &vertexInputDesc;
            mergePipelineDesc.inputAssembly = inputAssemblyDesc;
            mergePipelineDesc.rasterization = rasterizationDesc;
            mergePipelineDesc.multisample = &multisampleDesc;
            mergePipelineDesc.outputMerger = outputMergerDesc;
            mergePipelineDesc.shaders = mergeShaderStages;
            mergePipelineDesc.shaderNum = helper::GetCountOf(mergeShaderStages);
            
            NRI_ABORT_ON_FAILURE(NRI.CreateGraphicsPipeline(*m_Device, mergePipelineDesc, m_mergePipeline));
        }
        
        // compute stages
        Log::Message("Renderer", "Create compute piplines");
        {
            // cached
            nri::ComputePipelineDesc cachedPipelineDesc = {};
            cachedPipelineDesc.pipelineLayout = m_cachedRenderLayout;
            cachedPipelineDesc.shader = utils::LoadShader(deviceDesc.graphicsAPI, "YMIR_CachedSWRasterizer.cs", shaderCodeStorage);
            NRI_ABORT_ON_FAILURE(NRI.CreateComputePipeline(*m_Device, cachedPipelineDesc, m_cachedRenderPipeline));

            // culling
            nri::ComputePipelineDesc cullingPipelineDesc = {};
            cullingPipelineDesc.pipelineLayout = m_cullingLayout;
            cullingPipelineDesc.shader = utils::LoadShader(deviceDesc.graphicsAPI, "YMIR_Culling.cs", shaderCodeStorage);
            NRI_ABORT_ON_FAILURE(NRI.CreateComputePipeline(*m_Device, cullingPipelineDesc, m_cullingPipeline));

            // vis buff
            nri::ComputePipelineDesc visBuffPipelineDesc = {};
            visBuffPipelineDesc.pipelineLayout = m_cullingLayout;
            visBuffPipelineDesc.shader = utils::LoadShader(deviceDesc.graphicsAPI, "YMIR_SWRasterizer.cs", shaderCodeStorage);
            NRI_ABORT_ON_FAILURE(NRI.CreateComputePipeline(*m_Device, visBuffPipelineDesc, m_visibilityBuffPipeline));

            // material
            nri::ComputePipelineDesc materialPipelineDesc = {};
            materialPipelineDesc.pipelineLayout = m_materialLayout;
            materialPipelineDesc.shader = utils::LoadShader(deviceDesc.graphicsAPI, "YMIR_Material.cs", shaderCodeStorage);
            NRI_ABORT_ON_FAILURE(NRI.CreateComputePipeline(*m_Device, materialPipelineDesc, m_materialPipeline));
        }

        Log::Message("Renderer", "initCam, only temporary");
        m_Camera.Initialize(float3(0,0,0), float3(0,0,-1), false);


        Log::Message("Renderer", "Need to add texture and material support");


        // visibility buffer
        Log::Message("Renderer", "Create Visibilty Buffer");
        {
            m_visBuff = VisibilityBuffer();
            


            nri::TextureDesc dataDesc = nri::Texture2D(nri::Format::R32_UINT, (uint16_t)GetWindowResolution().x, (uint16_t)GetWindowResolution().y, 10, 1, nri::TextureUsageBits::SHADER_RESOURCE_STORAGE);
            nri::TextureDesc depthDesc = nri::Texture2D(nri::Format::R32_SFLOAT, (uint16_t)GetWindowResolution().x, (uint16_t)GetWindowResolution().y, 1, 1, nri::TextureUsageBits::SHADER_RESOURCE_STORAGE);

            NRI_ABORT_ON_FAILURE(NRI.CreateTexture(*m_Device, dataDesc, m_visBuff.data));
            NRI_ABORT_ON_FAILURE(NRI.CreateTexture(*m_Device, depthDesc, m_visBuff.depth));
        }


        const uint32_t constantBufferSize = helper::Align((uint32_t)sizeof(GlobalConstants), deviceDesc.constantBufferOffsetAlignment);

        // buffers
        Log::Message("Renderer", "Create Buffer");
        {
            nri::BufferDesc bufferDesc = {};
            nri::Buffer* buffer;

            // model desc
            bufferDesc.size = helper::GetByteSizeOf(m_vGeomStreamer->modelDescs);
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            bufferDesc.structureStride = sizeof(ModelDesc);
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // meshlet desc
            bufferDesc.size = helper::GetByteSizeOf(m_vGeomStreamer->meshletDescs);
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            bufferDesc.structureStride = sizeof(MeshletDesc);
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // render data 0
            bufferDesc.size = MESHLETDATASIZE * sizeof(uint32_t);
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            bufferDesc.structureStride = sizeof(uint32_t);
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // renderdata 1
            bufferDesc.size = MESHLETDATASIZE * sizeof(uint32_t);
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            bufferDesc.structureStride = sizeof(uint32_t);
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // render cmd 0
            bufferDesc.size = sizeof(RenderCmd) * RENDERCMDS;
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
            bufferDesc.structureStride = sizeof(RenderCmd);
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // render cmd 1
            bufferDesc.size = sizeof(RenderCmd) * RENDERCMDS;
            bufferDesc.structureStride = sizeof(RenderCmd);
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // load descs buffer
            bufferDesc.size = sizeof(MeshletLoadDesc) * READBACKS;
            bufferDesc.structureStride = sizeof(MeshletLoadDesc);
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);


            // READBACK_BUFFER
            bufferDesc.size = sizeof(MeshletLoadDesc) * READBACKS;
            bufferDesc.structureStride = sizeof(MeshletLoadDesc);
            bufferDesc.usageMask = nri::BufferUsageBits::NONE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // instance buffer 
            bufferDesc.size = sizeof(InstanceDesc) * MAXMESHINSTANCES;
            bufferDesc.structureStride = sizeof(InstanceDesc);
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // last frame instance buffer 
            bufferDesc.size = sizeof(uint32_t) * MAXMESHINSTANCES;
            bufferDesc.structureStride = sizeof(uint32_t);
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            
            // constant buffer
            nri::BufferDesc constBufferDesc = {};
            constBufferDesc.size = constantBufferSize * BUFFERED_FRAME_MAX_NUM;
            constBufferDesc.usageMask = nri::BufferUsageBits::CONSTANT_BUFFER;
            //constBufferDesc.structureStride
            nri::Buffer* constBuffer;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, constBufferDesc, constBuffer));
            m_buffers.push_back(constBuffer);


            bufferDesc.size = sizeof(uint32_t) * 6;
            bufferDesc.usageMask = nri::BufferUsageBits::INDEX_BUFFER;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            bufferDesc.size = sizeof(QuatVertex) * 4;
            bufferDesc.usageMask = nri::BufferUsageBits::VERTEX_BUFFER;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);
        }

        // Memory
        Log::Message("Renderer", "Memory Allocations");
        {
            nri::ResourceGroupDesc resourceGroupDesc = {};
            size_t baseAllocation = m_memoryAllocations.size();

            
            // resident buffers
            resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
            resourceGroupDesc.bufferNum = 2;
            resourceGroupDesc.buffers = &m_buffers[MODELDESC];
            
            baseAllocation = m_memoryAllocations.size();
            m_memoryAllocations.resize(baseAllocation + 2, nullptr);
 
            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_memoryAllocations.data() + baseAllocation));


            // render data
            resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
            resourceGroupDesc.bufferNum = 2;
            resourceGroupDesc.buffers = &m_buffers[RENDERDATA0];

            baseAllocation = m_memoryAllocations.size();
            m_memoryAllocations.resize(baseAllocation + 2, nullptr);

            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_memoryAllocations.data() + baseAllocation));


            // render cmds
            resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
            resourceGroupDesc.bufferNum = 3;
            resourceGroupDesc.buffers = &m_buffers[RENDERCMD0];

            baseAllocation = m_memoryAllocations.size();
            m_memoryAllocations.resize(baseAllocation + 3, nullptr);

            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_memoryAllocations.data() + baseAllocation));
            
            // readback
            resourceGroupDesc.memoryLocation = nri::MemoryLocation::HOST_READBACK;
            resourceGroupDesc.bufferNum = 1;
            resourceGroupDesc.buffers = &m_buffers[READBACK];

            baseAllocation = m_memoryAllocations.size();
            m_memoryAllocations.resize(baseAllocation + 1, nullptr);
            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_memoryAllocations.data() + baseAllocation));

            // instances
            resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
            resourceGroupDesc.bufferNum = 2;
            resourceGroupDesc.buffers = &m_buffers[INSTANCES];

            baseAllocation = m_memoryAllocations.size();
            m_memoryAllocations.resize(baseAllocation + 2, nullptr);

            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_memoryAllocations.data() + baseAllocation));
            

            // constant buffer
            resourceGroupDesc.memoryLocation = nri::MemoryLocation::HOST_UPLOAD;
            resourceGroupDesc.bufferNum = 1;
            resourceGroupDesc.buffers = &m_buffers[CONSTANTBUFFER];

            baseAllocation = m_memoryAllocations.size();
            m_memoryAllocations.resize(baseAllocation + 1, nullptr);
            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_memoryAllocations.data() + baseAllocation));


            // index + vertex 

            resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
            resourceGroupDesc.bufferNum = 2;
            resourceGroupDesc.buffers = &m_buffers[INDEXBUFFER];

            baseAllocation = m_memoryAllocations.size();
            m_memoryAllocations.resize(baseAllocation + 2, nullptr);

            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_memoryAllocations.data() + baseAllocation));


            // visbuffer
            //  

            nri::Texture* tmpTex[2] = { m_visBuff.data, m_visBuff.depth };
            nri::Memory* tmpMem[2] = { m_visBuff.dataMem, m_visBuff.depthMem };

            resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
            resourceGroupDesc.bufferNum = 0;
            resourceGroupDesc.buffers = nullptr;
            resourceGroupDesc.textureNum = 2;
            resourceGroupDesc.textures = tmpTex;

            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, tmpMem));
            m_visBuff.dataMem = tmpMem[0];
            m_visBuff.depthMem = tmpMem[1];

            Log::Message("Renderer", "Need to add texture and material support");
        }

        // create descriptors
        Log::Message("Renderer", "Create Descriptors");
        nri::Descriptor* anisotropicSampler = nullptr;
        nri::Descriptor* constantBufferViews[BUFFERED_FRAME_MAX_NUM] = {};
        nri::Descriptor* resourceView = {};
        nri::Descriptor* resourceViews[6] = {};

        nri::Descriptor* indexVertexViews[2] = {};
        {
            Log::Message("Renderer", "Need to add texture and material support");

            // visibility buffer
            nri::Texture2DViewDesc texture2DViewDesc = { m_visBuff.depth, nri::Texture2DViewType::SHADER_RESOURCE_STORAGE_2D, nri::Format::R32_SFLOAT };
            NRI_ABORT_ON_FAILURE(NRI.CreateTexture2DView(texture2DViewDesc, m_visBuff.depthDesc));

            texture2DViewDesc = { m_visBuff.data, nri::Texture2DViewType::SHADER_RESOURCE_STORAGE_2D, nri::Format::R32_UINT };
            NRI_ABORT_ON_FAILURE(NRI.CreateTexture2DView(texture2DViewDesc, m_visBuff.dataDesc));
            
            // visbuffer Sampler
            nri::SamplerDesc samplerDesc = {};
            samplerDesc.addressModes = { nri::AddressMode::CLAMP_TO_EDGE, nri::AddressMode::CLAMP_TO_EDGE };
            samplerDesc.filters = { nri::Filter::NEAREST, nri::Filter::NEAREST, nri::Filter::NEAREST };
            samplerDesc.anisotropy = 8;
            samplerDesc.mipMax = 16.0f;
            NRI_ABORT_ON_FAILURE(NRI.CreateSampler(*m_Device, samplerDesc, m_visBuff.sampler));
            
            // texture sampler
            nri::SamplerDesc texSamplerDesc = {};
            texSamplerDesc.addressModes = { nri::AddressMode::REPEAT, nri::AddressMode::REPEAT };
            texSamplerDesc.filters = { nri::Filter::LINEAR, nri::Filter::LINEAR, nri::Filter::LINEAR };
            texSamplerDesc.anisotropy = 8;
            texSamplerDesc.mipMax = 16.0f;
            NRI_ABORT_ON_FAILURE(NRI.CreateSampler(*m_Device, texSamplerDesc, anisotropicSampler));

            // buffer n stuff
            nri::BufferViewDesc bufferViewDesc = {};
            bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE;
            bufferViewDesc.offset = 0;


            // model desc view
            bufferViewDesc.buffer = m_buffers[MODELDESC];
            bufferViewDesc.size = m_vGeomStreamer->modelDescs.size() * sizeof(ModelDesc);
            
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceView));
            m_descriptors.push_back(resourceView);
            resourceViews[0] = resourceView;

            // meshlet desc view
            bufferViewDesc.buffer = m_buffers[MESHLETDESC];
            bufferViewDesc.size = m_vGeomStreamer->meshletDescs.size() * sizeof(MeshletDesc);
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceView));
            m_descriptors.push_back(resourceView);
            resourceViews[1] = resourceView;

            // render data 0 view
            bufferViewDesc.buffer = m_buffers[RENDERDATA0];
            bufferViewDesc.size = MESHLETDATASIZE * sizeof(uint32_t);
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceView));
            m_descriptors.push_back(resourceView);
            resourceViews[2] = resourceView;

            // render data 1 view
            bufferViewDesc.buffer = m_buffers[RENDERDATA1];
            bufferViewDesc.size = MESHLETDATASIZE * sizeof(uint32_t);
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceView));
            m_descriptors.push_back(resourceView);
            resourceViews[3] = resourceView;

            // render cmd 0 view
            bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE_STORAGE;
            bufferViewDesc.buffer = m_buffers[RENDERCMD0];
            bufferViewDesc.size = sizeof(RenderCmd) * RENDERCMDS;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceView));
            m_descriptors.push_back(resourceView);

            // render cmd 1 view
            bufferViewDesc.buffer = m_buffers[RENDERCMD1];
            bufferViewDesc.size = sizeof(RenderCmd) * RENDERCMDS;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceView));
            m_descriptors.push_back(resourceView);

            // load descs buffer view
            bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE_STORAGE;
            bufferViewDesc.buffer = m_buffers[LOADDESCS];
            bufferViewDesc.size = sizeof(uint32_t) * MAXMESHINSTANCES;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceView));
            m_descriptors.push_back(resourceView);
            //resourceViews[5] = resourceView;

            // readback                                                         // mabe todo
            m_descriptors.push_back(nullptr);

            // instance view
            bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE;
            bufferViewDesc.buffer = m_buffers[INSTANCES];
            bufferViewDesc.size = sizeof(InstanceDesc) * MAXMESHINSTANCES;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceView));
            m_descriptors.push_back(resourceView);
            resourceViews[4] = resourceView;

            // old instance indexes
            bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE;
            bufferViewDesc.buffer = m_buffers[INSTANCESLAST];
            bufferViewDesc.size = sizeof(uint32_t) * MAXMESHINSTANCES;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceView));
            m_descriptors.push_back(resourceView);
            resourceViews[5] = resourceView;

            
            // constant buffer
            for (uint32_t i = 0; i < BUFFERED_FRAME_MAX_NUM; i++)
            {
                m_Frames[i].globalConstantBufferViewOffsets = i * constantBufferSize;
                bufferViewDesc.buffer = m_buffers[CONSTANTBUFFER];
                bufferViewDesc.viewType = nri::BufferViewType::CONSTANT;
                bufferViewDesc.offset = i * constantBufferSize;
                bufferViewDesc.size = constantBufferSize;
                NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, constantBufferViews[i]));
                m_descriptors.push_back(constantBufferViews[i]);
            }


            // Swap chain
            for (uint32_t i = 0; i < swapChainTextureNum; i++)
            {
                nri::Texture2DViewDesc textureViewDesc = { swapChainTextures[i], nri::Texture2DViewType::COLOR_ATTACHMENT, swapChainFormat };

                nri::Descriptor* colorAttachment;
                NRI_ABORT_ON_FAILURE(NRI.CreateTexture2DView(textureViewDesc, colorAttachment));

                const BackBuffer backBuffer = { colorAttachment, swapChainTextures[i] };
                m_SwapChainBuffers.push_back(backBuffer);
            }
        }
#define TEST 10

        // descriptor pool
        Log::Message("Renderer", "Create Descriptor pool");
        {
            nri::DescriptorPoolDesc descriptorPoolDesc = {};
                                                    
            descriptorPoolDesc.descriptorSetMaxNum = ((BUFFERED_FRAME_MAX_NUM + 1) * 4)+1;
            descriptorPoolDesc.textureMaxNum = 30 * TEXTURES_PER_MATERIAL;                       
            descriptorPoolDesc.samplerMaxNum = BUFFERED_FRAME_MAX_NUM * 10;
            descriptorPoolDesc.storageStructuredBufferMaxNum = 100;
            descriptorPoolDesc.storageBufferMaxNum = 100;
            descriptorPoolDesc.bufferMaxNum = 100;
            descriptorPoolDesc.structuredBufferMaxNum = 100;
            descriptorPoolDesc.constantBufferMaxNum = BUFFERED_FRAME_MAX_NUM * 4;
            descriptorPoolDesc.storageTextureMaxNum = 100;
            descriptorPoolDesc.dynamicConstantBufferMaxNum = BUFFERED_FRAME_MAX_NUM * 4;
            descriptorPoolDesc.accelerationStructureMaxNum = 0;

            

            NRI_ABORT_ON_FAILURE(NRI.CreateDescriptorPool(*m_Device, descriptorPoolDesc, m_DescriptorPool));
        }

        // descriptor sets
        Log::Message("Renderer", "Create Descriptor Sets");
        {
            m_cachedDescriptorSets.resize(BUFFERED_FRAME_MAX_NUM + 1);
            m_cullingDescriptorSets.resize(BUFFERED_FRAME_MAX_NUM + 1);
            m_visBuffDescriptorSets.resize(BUFFERED_FRAME_MAX_NUM + 1);
            m_materialDescriptorSets.resize(BUFFERED_FRAME_MAX_NUM + 1);
            m_mergeDescriptorSets.resize(1);

            // cached descriptor set
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_cachedRenderLayout, 0,
                &m_cachedDescriptorSets[BUFFERED_FRAME_MAX_NUM * 0], BUFFERED_FRAME_MAX_NUM, 0));
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_cachedRenderLayout, 1,
                &m_cachedDescriptorSets[BUFFERED_FRAME_MAX_NUM * 1], 1, 2));
            
            // culling descriptor set
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_cullingLayout, 0,
                &m_cullingDescriptorSets[0], BUFFERED_FRAME_MAX_NUM, 0));
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_cullingLayout, 1,
                &m_cullingDescriptorSets[BUFFERED_FRAME_MAX_NUM], 1, 2));

            // vis buff descriptor set
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_visibilityBuffLayout, 0,
                &m_visBuffDescriptorSets[0], BUFFERED_FRAME_MAX_NUM, 0));
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_visibilityBuffLayout, 1,
                &m_visBuffDescriptorSets[BUFFERED_FRAME_MAX_NUM], 1, 2));

            // material descriptor set
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_materialLayout, 0,
                &m_materialDescriptorSets[0], BUFFERED_FRAME_MAX_NUM, 0));
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_materialLayout, 1,
                &m_materialDescriptorSets[BUFFERED_FRAME_MAX_NUM], 1, 2));

            // merge descriptor set
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_mergeLayout, 0,
                &m_mergeDescriptorSets[0], 1, 2));
            /*NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_cachedRenderLayout, 0,
                &m_cachedDescriptorSets[BUFFERED_FRAME_MAX_NUM * 1], BUFFERED_FRAME_MAX_NUM, 2));*/
            
            //  populate

            for (uint32_t i = BUFFERED_FRAME_MAX_NUM * 0; i < BUFFERED_FRAME_MAX_NUM; i++)
            {
                nri::DescriptorRangeUpdateDesc globalDescriptorRangeUpdateDescs[4] = {};
                globalDescriptorRangeUpdateDescs[0].descriptorNum = 1;
                globalDescriptorRangeUpdateDescs[0].descriptors = &constantBufferViews[i];
                globalDescriptorRangeUpdateDescs[1].descriptorNum = 1;
                globalDescriptorRangeUpdateDescs[1].descriptors = &anisotropicSampler;
                globalDescriptorRangeUpdateDescs[2].descriptorNum = GetStructuredBuffCount();
                globalDescriptorRangeUpdateDescs[2].descriptors = resourceViews;
                globalDescriptorRangeUpdateDescs[3].descriptorNum = GetRWStructuredBuffCount();
                globalDescriptorRangeUpdateDescs[3].descriptors = &m_descriptors[RENDERCMD0];


                NRI.UpdateDescriptorRanges(*m_cachedDescriptorSets[i], 0, helper::GetCountOf(globalDescriptorRangeUpdateDescs), globalDescriptorRangeUpdateDescs);
                NRI.UpdateDescriptorRanges(*m_cullingDescriptorSets[i], 0, helper::GetCountOf(globalDescriptorRangeUpdateDescs), globalDescriptorRangeUpdateDescs);
                NRI.UpdateDescriptorRanges(*m_visBuffDescriptorSets[i], 0, helper::GetCountOf(globalDescriptorRangeUpdateDescs), globalDescriptorRangeUpdateDescs);
                NRI.UpdateDescriptorRanges(*m_materialDescriptorSets[i], 0, helper::GetCountOf(globalDescriptorRangeUpdateDescs), globalDescriptorRangeUpdateDescs);
                
            }


            nri::Descriptor* visBuffDesc[2] = { m_visBuff.depthDesc, m_visBuff.dataDesc };

            nri::DescriptorRangeUpdateDesc visBuffDescriptorRangeUpdateDescs[2] = {};
            visBuffDescriptorRangeUpdateDescs[0].descriptorNum = 2;
            visBuffDescriptorRangeUpdateDescs[0].descriptors = visBuffDesc;
            visBuffDescriptorRangeUpdateDescs[1].descriptorNum = 1;
            visBuffDescriptorRangeUpdateDescs[1].descriptors = &m_visBuff.sampler;

            NRI.UpdateDescriptorRanges(*m_cachedDescriptorSets[BUFFERED_FRAME_MAX_NUM], 0, helper::GetCountOf(visBuffDescriptorRangeUpdateDescs), visBuffDescriptorRangeUpdateDescs);
            NRI.UpdateDescriptorRanges(*m_cullingDescriptorSets[BUFFERED_FRAME_MAX_NUM], 0, helper::GetCountOf(visBuffDescriptorRangeUpdateDescs), visBuffDescriptorRangeUpdateDescs);
            NRI.UpdateDescriptorRanges(*m_visBuffDescriptorSets[BUFFERED_FRAME_MAX_NUM], 0, helper::GetCountOf(visBuffDescriptorRangeUpdateDescs), visBuffDescriptorRangeUpdateDescs);
            NRI.UpdateDescriptorRanges(*m_materialDescriptorSets[BUFFERED_FRAME_MAX_NUM], 0, helper::GetCountOf(visBuffDescriptorRangeUpdateDescs), visBuffDescriptorRangeUpdateDescs);
            NRI.UpdateDescriptorRanges(*m_mergeDescriptorSets[0], 0, helper::GetCountOf(visBuffDescriptorRangeUpdateDescs), visBuffDescriptorRangeUpdateDescs);
        }


        // upload data
        Log::Message("Renderer", "Upload Data");
        {
            // allocate buffer on cpu
            allRenderedEntites0 = (InstanceDesc*)malloc(sizeof(InstanceDesc) * MAXMESHINSTANCES);
            allRenderedEntites1 = (InstanceDesc*)malloc(sizeof(InstanceDesc) * MAXMESHINSTANCES);
            lastFrameEntitiesID = (uint32_t*)malloc(sizeof(uint32_t) * MAXMESHINSTANCES);

            // instances
            PopulateInstaceBuffer();
            UploadInstaceBuffer();

            // all resident buffers
            nri::BufferUploadDesc bufferData[2]{
                //            data ptr                                   size                                     buffer              offset      flags
                {m_vGeomStreamer->modelDescs.data(),  helper::GetByteSizeOf(m_vGeomStreamer->modelDescs), m_buffers[MODELDESC], 0, {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::ALL}},
                {m_vGeomStreamer->meshletDescs.data(),  helper::GetByteSizeOf(m_vGeomStreamer->meshletDescs), m_buffers[MESHLETDESC], 0, {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::ALL}},
            };


            // visBuffer
        }
    }

    Log::Message("Renderer", "Initiation Finished");
    return true;
}

void Renderer::PrepareFrame(uint32_t frameIndex)
{
    useBuffer0 = !useBuffer0;
    if (UI) {
        BeginUI();

        WindowManager::Show();

        EndUI(NRI, *m_Streamer);
    }
    NRI.CopyStreamerUpdateRequests(*m_Streamer);



    CameraDesc desc = {};
    desc.aspectRatio = float(GetWindowResolution().x) / float(GetWindowResolution().y);
    desc.horizontalFov = 90.0f;
    desc.nearZ = 0.1f;
    desc.isReversedZ = (CLEAR_DEPTH == 0.0f);
    GetCameraDescFromInputDevices(desc);

    m_Camera.Update(desc, frameIndex);
}

void Renderer::RenderFrame(uint32_t frameIndex)
{
    const uint32_t bufferedFrameIndex = frameIndex % BUFFERED_FRAME_MAX_NUM;
    const Frame& frame = m_Frames[bufferedFrameIndex];
    const uint32_t windowWidth = GetWindowResolution().x;
    const uint32_t windowHeight = GetWindowResolution().y;



    if (frameIndex >= BUFFERED_FRAME_MAX_NUM)
    {
        NRI.Wait(*m_FrameFence, 1 + frameIndex - BUFFERED_FRAME_MAX_NUM);
        NRI.ResetCommandAllocator(*frame.cmdAllocEvalStage);
        NRI.ResetCommandAllocator(*frame.cmdAllocRenderStage);
    }

    const uint32_t currentTextureIndex = NRI.AcquireNextSwapChainTexture(*m_SwapChain);
    BackBuffer& currentBackBuffer = m_SwapChainBuffers[currentTextureIndex];

    // Update constants
    const uint64_t rangeOffset = m_Frames[bufferedFrameIndex].globalConstantBufferViewOffsets;
    auto constants = (GlobalConstants*)NRI.MapBuffer(*m_buffers[CONSTANTBUFFER], rangeOffset, sizeof(GlobalConstants));
    if (constants)
    {
        constants->gWorldToClip = m_Camera.state.mWorldToClip;
        constants->gCameraPos = m_Camera.state.position;
        constants->bufferToUse = useBuffer0 ? 0 : 1;
        NRI.UnmapBuffer(*m_buffers[CONSTANTBUFFER]);
    }

    // eval stage
    nri::CommandBuffer& commandBuffer = *frame.cmdBuffEvalStage;
    NRI.BeginCommandBuffer(commandBuffer, m_DescriptorPool);
    {
        helper::Annotation annotation(NRI, commandBuffer, "Eval Stage");

        // cached
        NRI.CmdSetPipelineLayout(commandBuffer, *m_cachedRenderLayout);
        NRI.CmdSetDescriptorSet(commandBuffer, 0, *m_cachedDescriptorSets[bufferedFrameIndex], nullptr);
        NRI.CmdSetDescriptorSet(commandBuffer, 1, *m_cachedDescriptorSets[BUFFERED_FRAME_MAX_NUM], nullptr);

        NRI.CmdSetPipeline(commandBuffer, *m_cachedRenderPipeline);
        NRI.CmdDispatch(commandBuffer, { 128, 1, 1 });


        // culling
        NRI.CmdSetPipelineLayout(commandBuffer, *m_cullingLayout);
        NRI.CmdSetDescriptorSet(commandBuffer, 0, *m_cullingDescriptorSets[bufferedFrameIndex], nullptr);
        NRI.CmdSetDescriptorSet(commandBuffer, 1, *m_cullingDescriptorSets[BUFFERED_FRAME_MAX_NUM], nullptr);

        NRI.CmdSetPipeline(commandBuffer, *m_cullingPipeline);
        NRI.CmdDispatch(commandBuffer, { 512, 1, 1 });
    }
    NRI.EndCommandBuffer(commandBuffer);
    // render stage
    nri::CommandBuffer& renderCommandBuffer = *frame.cmdBuffRenderStage;
    NRI.BeginCommandBuffer(renderCommandBuffer, m_DescriptorPool);
    {
        helper::Annotation annotation(NRI, renderCommandBuffer, "Render Stage");

        // vis buff
        NRI.CmdSetPipelineLayout(renderCommandBuffer, *m_visibilityBuffLayout);
        NRI.CmdSetDescriptorSet(renderCommandBuffer, 0, *m_visBuffDescriptorSets[bufferedFrameIndex], nullptr);
        NRI.CmdSetDescriptorSet(renderCommandBuffer, 1, *m_visBuffDescriptorSets[BUFFERED_FRAME_MAX_NUM], nullptr);

        NRI.CmdSetPipeline(renderCommandBuffer, *m_visibilityBuffPipeline);
        NRI.CmdDispatch(renderCommandBuffer, { 128, 1, 1 });

        // material
        NRI.CmdSetPipelineLayout(renderCommandBuffer, *m_materialLayout);
        NRI.CmdSetDescriptorSet(renderCommandBuffer, 0, *m_materialDescriptorSets[bufferedFrameIndex], nullptr);
        NRI.CmdSetDescriptorSet(renderCommandBuffer, 1, *m_materialDescriptorSets[BUFFERED_FRAME_MAX_NUM], nullptr);

        NRI.CmdSetPipeline(renderCommandBuffer, *m_materialPipeline);
        NRI.CmdDispatch(renderCommandBuffer, { GetWindowResolution().x, GetWindowResolution().y, 1});



        // present & UI
        nri::AttachmentsDesc attachmentsDesc = {};
        attachmentsDesc.colorNum = 1;
        attachmentsDesc.colors = &currentBackBuffer.colorAttachment;
        attachmentsDesc.depthStencil = m_DepthAttachment;

        nri::TextureBarrierDesc textureBarrierDescs = {};
        textureBarrierDescs.texture = currentBackBuffer.texture;
        textureBarrierDescs.after = { nri::AccessBits::COLOR_ATTACHMENT, nri::Layout::COLOR_ATTACHMENT };
        textureBarrierDescs.arraySize = 1;
        textureBarrierDescs.mipNum = 1;


        // rendering to main render target
        NRI.CmdBeginRendering(commandBuffer, attachmentsDesc);
        {
            nri::ClearDesc clearDescs[2] = {};
            clearDescs[0].attachmentContentType = nri::AttachmentContentType::COLOR;
            clearDescs[0].value.color32f = { 0.0f, 0.63f, 1.0f };
            clearDescs[1].attachmentContentType = nri::AttachmentContentType::DEPTH;
            clearDescs[1].value.depthStencil.depth = CLEAR_DEPTH;

            NRI.CmdClearAttachments(commandBuffer, clearDescs, helper::GetCountOf(clearDescs), nullptr, 0);

            const nri::Viewport viewport = { 0.0f, 0.0f, (float)windowWidth, (float)windowHeight, 0.0f, 1.0f };
            NRI.CmdSetViewports(commandBuffer, &viewport, 1);

            const nri::Rect scissor = { 0, 0, (nri::Dim_t)windowWidth, (nri::Dim_t)windowHeight };
            NRI.CmdSetScissors(commandBuffer, &scissor, 1);

            NRI.CmdSetIndexBuffer(commandBuffer, *m_buffers[INDEXBUFFER], 0, sizeof(utils::Index) == 2 ? nri::IndexType::UINT16 : nri::IndexType::UINT32);

            NRI.CmdSetPipelineLayout(commandBuffer, *m_mergeLayout);
            NRI.CmdSetDescriptorSet(commandBuffer, 0, *m_mergeDescriptorSets[bufferedFrameIndex], nullptr);
            NRI.CmdSetDescriptorSet(commandBuffer, 1, *m_mergeDescriptorSets[BUFFERED_FRAME_MAX_NUM], nullptr);
            NRI.CmdSetPipeline(commandBuffer, *m_mergePipeline);

            constexpr uint64_t offset = 0;
            NRI.CmdSetVertexBuffers(commandBuffer, 0, 1, &m_buffers[VERTEXBUFFER], &offset);

            NRI.CmdDrawIndexed(commandBuffer, { 6, 1, 0, (int32_t)0, 0});
        }
        NRI.CmdEndRendering(commandBuffer);

        if (UI) {
            NRI.CmdBeginRendering(commandBuffer, attachmentsDesc);
            {
                RenderUI(NRI, NRI, *m_Streamer, commandBuffer, 1.0f, true);
            }
            NRI.CmdEndRendering(commandBuffer);
        }
    }
    NRI.EndCommandBuffer(renderCommandBuffer);
}

void Renderer::ReloadRenderer()
{
    WindowManager::Init();

    m_vGeomStreamer = new VirtualGeometryStreamer();
    m_vGeomStreamer->Init();


}

void Renderer::IterateChildren(entt::entity e, float4x4 pMat) {
    if (!EntityManager::GetWorld().valid(e)) {
        return;
    }
    if (!EntityManager::GetWorld().group<Transform/*, Identity*/>().contains(e)) {
        return;
    }
    if (!EntityManager::GetWorld().group<Identity/*, Identity*/>().contains(e)) {
        return;
    }
    
    auto& transform = EntityManager::GetWorld().get<Transform>(e);

    float4x4 localToWorld = pMat * transform.localMat;

    if (EntityManager::GetWorld().group<MeshInstance>().contains(e)) {
        if (useBuffer0) {
            instanceCount0++;
            if (instanceCount0 >= MAXMESHINSTANCES) {
                instanceCount0 = MAXMESHINSTANCES;
                return;
            }
            
        }
        else {
            instanceCount1++;
            if (instanceCount1 > MAXMESHINSTANCES) {
                instanceCount1 = MAXMESHINSTANCES;
                return;
            }
        }

        auto& meshInstance = EntityManager::GetWorld().get<MeshInstance>(e);
        if (meshInstance.modelID != 0xffffffff) {
            /*if (meshInstance.materialID != 0xffffffff) {

            }*/
            InstanceDesc instDesc;

            instDesc.realID = (uint32_t)e;
            instDesc.materialID = 0xffffffff;
            instDesc.transform = localToWorld;
            instDesc.modelID = meshInstance.modelID;

            if (useBuffer0) {
                memcpy((void*)&allRenderedEntites0[instanceCount0 - 1], &instDesc, sizeof(InstanceDesc));

                for (uint32_t i = 0; i < instanceCount1 && i < MAXMESHINSTANCES; i++)
                {
                    if (allRenderedEntites1[i].realID == (uint32_t)e) {
                        lastFrameEntitiesID[i] = instanceCount0;
                    }
                }
            }
            else {
                memcpy((void*)&allRenderedEntites1[instanceCount1 - 1], &instDesc, sizeof(InstanceDesc));
                for (uint32_t i = 0; i < instanceCount0 && i < MAXMESHINSTANCES; i++)
                {
                    if (allRenderedEntites0[i].realID == (uint32_t)e) {
                        lastFrameEntitiesID[i] = instanceCount1;
                    }
                }
            }
        }
        

    }
    

    auto& identity = EntityManager::GetWorld().get<Identity>(e);
    Log::Message("Entites", "Iterate: " + identity.name);
    for (entt::entity& child : identity.childs)
    {
        IterateChildren(child, localToWorld);
    }
}

CachedCluster& Renderer::Contains(uint32_t MID, uint32_t CID) {
    for (CachedCluster& cC : cachedCluster)
    {
        if (cC.modelID == MID && cC.clusterID == CID) {
            return cC;
        }
    }
    return nullCluster;
}

void Renderer::StreamGeom()
{
    // todo
    MeshletLoadDesc* head = (MeshletLoadDesc*)NRI.MapBuffer(*m_buffers[READBACK], 0, sizeof(MeshletLoadDesc));

    uint32_t requestLenght = head->clusterID;
    uint32_t bufferByteSize = head->modelID;
    if (biggestSize < bufferByteSize) {
        biggestSize = bufferByteSize;
    }
    bufferByteSize = max(bufferByteSize, (uint32_t)MESHLETDATASIZE);

    if (requestLenght == 0xffffffff || bufferByteSize == 0xffffffff) {
        return;
    }

    uint32_t* readBackData = (uint32_t*)NRI.MapBuffer(*m_buffers[READBACK], sizeof(MeshletLoadDesc), sizeof(MeshletLoadDesc) * requestLenght);

    if (sizeLast < bufferByteSize) {
        // resize buffer
        uint8_t* reall;
        if (useBuffer0) {
            // CPU
            reall = (uint8_t*)realloc(VGBuffer0, bufferByteSize);
            if (reall) {
                VGBuffer0 = reall;
                size0 = bufferByteSize;
            }

            //GPU
            //NRI.DestroyBuffer(*m_buffers[RENDERDATA0]);
            //NRI.FreeMemory(*m_memoryAllocations[RENDERDATA0]);


        }
        else {
            // CPU
            reall = (uint8_t*)realloc(VGBuffer1, bufferByteSize);
            if (reall) {
                VGBuffer1 = reall;
                size1 = bufferByteSize;
            }

            // GPU
            //NRI.DestroyBuffer(*m_buffers[RENDERDATA1]);
            //NRI.FreeMemory(*m_memoryAllocations[RENDERDATA1]);
        }


    }

    std::vector<CachedCluster> cCs(requestLenght);
    for (uint32_t i = 0; i < requestLenght; i++)
    {
        MeshletLoadDesc meshl;
        meshl.clusterID = readBackData[i * 4];
        meshl.modelID = readBackData[i * 4 + 1];
        meshl.clusterOffset = readBackData[i * 4 + 2];
        meshl.clusterLenght = readBackData[i * 4 + 3];

        CachedCluster& cC = Contains(meshl.modelID, meshl.clusterID);
        if (cC.clusterID == nullCluster.clusterID && cC.modelID == nullCluster.modelID) {
            if (useBuffer0) {
                m_vGeomStreamer->Load(meshl.modelID, &VGBuffer0[meshl.clusterOffset], meshl.clusterID, meshl.clusterLenght);
            }
            else {
                m_vGeomStreamer->Load(meshl.modelID, &VGBuffer1[meshl.clusterOffset], meshl.clusterID, meshl.clusterLenght);
            }

            continue;
        }

        if (useBuffer0) {
            memcpy(&VGBuffer0[meshl.clusterOffset], &VGBuffer1[cC.clusterOffSet], meshl.clusterLenght);
        }
        else {
            memcpy(&VGBuffer1[meshl.clusterOffset], &VGBuffer0[cC.clusterOffSet], meshl.clusterLenght);
        }
        cCs[i] = { meshl.modelID, meshl.clusterID, meshl.clusterOffset };
    }

    sizeLast = bufferByteSize;
    
    NRI.UnmapBuffer(*m_buffers[READBACK]);

    nri::BufferUploadDesc bufferData[1]{
        //            data ptr                                   size                                     buffer              offset      flags
        {useBuffer0 ? VGBuffer0 : VGBuffer1, useBuffer0 ? size0 : size1, m_buffers[useBuffer0 ? RENDERDATA0 : RENDERDATA1], 0, {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::ALL}}

    };

    NRI_ABORT_ON_FAILURE(NRI.UploadData(*m_CommandQueue, nullptr, 0, bufferData, helper::GetCountOf(bufferData)));
}

void Renderer::PopulateInstaceBuffer()
{
    if (useBuffer0) {
        instanceCount0 = 0;
    }
    else {
        instanceCount1 = 0;
    }
    memset(lastFrameEntitiesID, 0xffffffff, MAXMESHINSTANCES);
    EntityManager::UpdateMatrizes();
    IterateChildren(EntityManager::GetRoot(), float4x4::Identity());
}

void Renderer::UploadInstaceBuffer()
{

    nri::BufferUploadDesc bufferData[2]{
        //            data ptr                                   size                                     buffer              offset      flags
        {useBuffer0 ? allRenderedEntites0 : allRenderedEntites1, sizeof(InstanceDesc) * useBuffer0 ? instanceCount0 : instanceCount1, m_buffers[INSTANCES], 0, {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::ALL}},
        {lastFrameEntitiesID, sizeof(uint32_t) * useBuffer0 ? instanceCount1 : instanceCount0, m_buffers[INSTANCESLAST], 0, {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::ALL}}
    };

    NRI_ABORT_ON_FAILURE(NRI.UploadData(*m_CommandQueue, nullptr, 0, bufferData, helper::GetCountOf(bufferData)));
}


//SAMPLE_MAIN(Renderer, 0);

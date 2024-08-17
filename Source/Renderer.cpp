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

uint32_t indices[6]{
    0,1,2, // 1. triangle
    3,2,1
};

Renderer::~Renderer()
{
    // free all memory destroy all buffers...
}

bool Renderer::Initialize(nri::GraphicsAPI graphicsAPI)
{
    
    // init asset manager, log
    WindowManager::Init();
    Log::Error("Renderer", "You didnt implemented free frunction!!!!!");
    exit(EXIT_FAILURE);
    // init geometry streamer, sets model render ids, creates model desc & meshlet desc buffer      // todo meshlet childs (lod hyrachie)
    m_vGeomStreamer = new VirtualGeometryStreamer();
    m_vGeomStreamer->Init();

    

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
    deviceCreationDesc.enableAPIValidation = m_DebugAPI;
    deviceCreationDesc.enableNRIValidation = m_DebugNRI;
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
    streamerDesc.dynamicBufferUsageBits = nri::BufferUsageBits::VERTEX_BUFFER | nri::BufferUsageBits::INDEX_BUFFER;
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
            nri::DescriptorRangeDesc globalDescriptorRange[5] = {};
            globalDescriptorRange[0] = { (uint32_t)((deviceDesc.graphicsAPI == nri::GraphicsAPI::D3D12) ? 0 : 0), 1, nri::DescriptorType::CONSTANT_BUFFER, nri::StageBits::ALL };
            globalDescriptorRange[1] = { 0, 1, nri::DescriptorType::SAMPLER, nri::StageBits::ALL };
            globalDescriptorRange[2] = { 0, GetStructuredBuffCount(), nri::DescriptorType::STRUCTURED_BUFFER, nri::StageBits::ALL };
            globalDescriptorRange[3] = { 0, GetRWStructuredBuffCount(), nri::DescriptorType::STORAGE_STRUCTURED_BUFFER, nri::StageBits::ALL };
            //globalDescriptorRange[4] = { 0, 2, nri::DescriptorType::TEXTURE, nri::StageBits::ALL, true, true };


            nri::DescriptorRangeDesc visBuffDescRange[2] = {};
            visBuffDescRange[0] = { 0, 2, nri::DescriptorType::TEXTURE, nri::StageBits::ALL, true, true };
            visBuffDescRange[1] = { 0, 1, nri::DescriptorType::SAMPLER, nri::StageBits::ALL };

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
                {0, visBuffDescRange, helper::GetCountOf(visBuffDescRange)}
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

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, cullingLayoutDesc, m_cachedRenderLayout));

            // vis buff pipeline desc
            nri::PipelineLayoutDesc visBuffLayoutDesc = {};
            cullingLayoutDesc.descriptorSetNum = helper::GetCountOf(defaultDescriptorSetDescs);
            cullingLayoutDesc.descriptorSets = defaultDescriptorSetDescs;
            cullingLayoutDesc.shaderStages = nri::StageBits::COMPUTE_SHADER;
            //cullingLayoutDesc.enableD3D12DrawParametersEmulation = true;

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, visBuffLayoutDesc, m_visibilityBuffLayout));

            // material pipeline desc
            nri::PipelineLayoutDesc materialLayoutDesc = {};
            cullingLayoutDesc.descriptorSetNum = helper::GetCountOf(materialDescriptorSetDescs);
            cullingLayoutDesc.descriptorSets = materialDescriptorSetDescs;
            cullingLayoutDesc.shaderStages = nri::StageBits::COMPUTE_SHADER;
            //cullingLayoutDesc.enableD3D12DrawParametersEmulation = true;

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, materialLayoutDesc, m_materialLayout));

            // merge pipeline desc
            nri::PipelineLayoutDesc mergeLayoutDesc = {};
            cullingLayoutDesc.descriptorSetNum = helper::GetCountOf(mergeDescriptorSetDescs);
            cullingLayoutDesc.descriptorSets = mergeDescriptorSetDescs;
            cullingLayoutDesc.shaderStages = nri::StageBits::VERTEX_SHADER | nri::StageBits::FRAGMENT_SHADER;
            cullingLayoutDesc.enableD3D12DrawParametersEmulation = true;

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, mergeLayoutDesc, m_cachedRenderLayout));
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
                vertexAttributeDesc[0].format = nri::Format::RGB32_SFLOAT;
                vertexAttributeDesc[0].offset = 0;
                vertexAttributeDesc[0].d3d = { "POSITION", 0 };
                vertexAttributeDesc[0].vk = { 0 };

                vertexAttributeDesc[1].format = nri::Format::RGB32_SFLOAT;
                vertexAttributeDesc[1].offset = 0;
                vertexAttributeDesc[1].d3d = { "UV", 3 };
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
                utils::LoadShader(deviceDesc.graphicsAPI, "TriangleMerge.vs", shaderCodeStorage),
                utils::LoadShader(deviceDesc.graphicsAPI, "TriangleMerge.fs", shaderCodeStorage),
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
            NRI_ABORT_ON_FAILURE(NRI.CreateComputePipeline(*m_Device, cullingPipelineDesc, m_cullingPipeline));

            // material
            nri::ComputePipelineDesc visBuffPipelineDesc = {};
            visBuffPipelineDesc.pipelineLayout = m_materialLayout;
            visBuffPipelineDesc.shader = utils::LoadShader(deviceDesc.graphicsAPI, "YMIR_SWRasterizer.cs", shaderCodeStorage);
            NRI_ABORT_ON_FAILURE(NRI.CreateComputePipeline(*m_Device, cullingPipelineDesc, m_materialPipeline));
        }

        Log::Message("Renderer", "initCam, only temporary");
        m_Camera.Initialize(float3(0,0,0), float3(0,0,-1), false);


        Log::Message("Renderer", "Need to add texture and material support");


        // visibility buffer
        Log::Message("Renderer", "Create Visibilty Buffer");
        {
            m_visBuff = VisibilityBuffer();
            


            nri::TextureDesc dataDesc = nri::Texture2D(nri::Format::R32_UINT, (uint16_t)GetWindowResolution().x, (uint16_t)GetWindowResolution().y, 10, 1);
            nri::TextureDesc depthDesc = nri::Texture2D(nri::Format::R32_SFLOAT, (uint16_t)GetWindowResolution().x, (uint16_t)GetWindowResolution().y, 1, 1,
                nri::TextureUsageBits::DEPTH_STENCIL_ATTACHMENT);

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
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // meshlet desc
            bufferDesc.size = helper::GetByteSizeOf(m_vGeomStreamer->meshletDescs);
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // render data 0
            bufferDesc.size = 1;
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // renderdata 1
            bufferDesc.size = 1;
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // render cmd 0
            bufferDesc.size = sizeof(RenderCmd) * RENDERCMDS;
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // render cmd 1
            bufferDesc.size = sizeof(RenderCmd) * RENDERCMDS;
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);


            // READBACK_BUFFER
            bufferDesc.size = sizeof(MeshletLoadDesc) * READBACKS;
            bufferDesc.usageMask = nri::BufferUsageBits::NONE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // instance buffer 
            bufferDesc.size = sizeof(ModelDesc) * MAXMESHINSTANCES;
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);

            // last frame instance buffer 
            bufferDesc.size = sizeof(uint32_t) * MAXMESHINSTANCES;
            bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
            NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
            m_buffers.push_back(buffer);


            // constant buffer
            nri::BufferDesc bufferDesc = {};
            bufferDesc.size = constantBufferSize * BUFFERED_FRAME_MAX_NUM;
            bufferDesc.usageMask = nri::BufferUsageBits::CONSTANT_BUFFER;
            nri::Buffer* buffer;
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
            resourceGroupDesc.bufferNum = 2;
            resourceGroupDesc.buffers = &m_buffers[RENDERCMD0];

            baseAllocation = m_memoryAllocations.size();
            m_memoryAllocations.resize(baseAllocation + 2, nullptr);

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

            size_t baseAllocation = m_memoryAllocations.size();
            m_memoryAllocations.resize(baseAllocation + 1, nullptr);
            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_memoryAllocations.data() + baseAllocation));

            // visbuffer
            //  data
            resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
            resourceGroupDesc.bufferNum = 0;
            resourceGroupDesc.buffers = nullptr;
            resourceGroupDesc.textureNum = 1;
            resourceGroupDesc.textures = &m_visBuff.data;

            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, &m_visBuff.dataMem));

            //  depth
            resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
            resourceGroupDesc.bufferNum = 0;
            resourceGroupDesc.buffers = nullptr;
            resourceGroupDesc.textureNum = 1;
            resourceGroupDesc.textures = &m_visBuff.depth;

            NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, &m_visBuff.depthMem));
            Log::Message("Renderer", "Need to add texture and material support");
        }

        // create descriptors
        Log::Message("Renderer", "Create Descriptors");
        nri::Descriptor* anisotropicSampler = nullptr;
        nri::Descriptor* constantBufferViews[BUFFERED_FRAME_MAX_NUM] = {};
        nri::Descriptor* resourceViews = {};
        {
            Log::Message("Renderer", "Need to add texture and material support");

            // visibility buffer
            nri::Texture2DViewDesc texture2DViewDesc = { m_visBuff.depth, nri::Texture2DViewType::SHADER_RESOURCE_STORAGE_2D, nri::Format::R32_SFLOAT };
            NRI_ABORT_ON_FAILURE(NRI.CreateTexture2DView(texture2DViewDesc, m_visBuff.depthDesc));

            nri::Texture2DViewDesc texture2DViewDesc = { m_visBuff.data, nri::Texture2DViewType::SHADER_RESOURCE_STORAGE_2D, nri::Format::R32_UINT };
            NRI_ABORT_ON_FAILURE(NRI.CreateTexture2DView(texture2DViewDesc, m_visBuff.dataDesc));
            
            // visbuffer Sampler
            nri::SamplerDesc samplerDesc = {};
            samplerDesc.addressModes = { nri::AddressMode::CLAMP_TO_EDGE, nri::AddressMode::CLAMP_TO_EDGE };
            samplerDesc.filters = { nri::Filter::NEAREST, nri::Filter::NEAREST, nri::Filter::NEAREST };
            samplerDesc.anisotropy = 8;
            samplerDesc.mipMax = 16.0f;
            NRI_ABORT_ON_FAILURE(NRI.CreateSampler(*m_Device, samplerDesc, m_visBuff.sampler));
            


            // buffer n stuff
            nri::BufferViewDesc bufferViewDesc = {};
            bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE;
            bufferViewDesc.offset = 0;


            // model desc view
            bufferViewDesc.buffer = m_buffers[MODELDESC];
            bufferViewDesc.size = m_vGeomStreamer->modelDescs.size() * sizeof(ModelDesc);
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceViews));
            m_descriptors.push_back(resourceViews);

            // meshlet desc view
            bufferViewDesc.buffer = m_buffers[MESHLETDESC];
            bufferViewDesc.size = m_vGeomStreamer->meshletDescs.size() * sizeof(MeshletDesc);
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceViews));
            m_descriptors.push_back(resourceViews);

            // render data 0 view
            bufferViewDesc.buffer = m_buffers[RENDERDATA0];
            bufferViewDesc.size = 1;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceViews));
            m_descriptors.push_back(resourceViews);

            // render data 1 view
            bufferViewDesc.buffer = m_buffers[RENDERDATA1];
            bufferViewDesc.size = 1;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceViews));
            m_descriptors.push_back(resourceViews);

            // render cmd 0 view
            bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE_STORAGE;
            bufferViewDesc.buffer = m_buffers[RENDERCMD0];
            bufferViewDesc.size = sizeof(RenderCmd) * RENDERCMDS;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceViews));
            m_descriptors.push_back(resourceViews);

            // render cmd 1 view
            bufferViewDesc.buffer = m_buffers[RENDERCMD1];
            bufferViewDesc.size = sizeof(RenderCmd) * RENDERCMDS;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceViews));
            m_descriptors.push_back(resourceViews);

            // readback                                                         // mabe todo
            m_descriptors.push_back(nullptr);

            // instance view
            bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE;
            bufferViewDesc.buffer = m_buffers[INSTANCES];
            bufferViewDesc.size = sizeof(InstanceDesc) * MAXMESHINSTANCES;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceViews));
            m_descriptors.push_back(resourceViews);

            // old instance indexes
            bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE;
            bufferViewDesc.buffer = m_buffers[IMSTANCESLAST];
            bufferViewDesc.size = sizeof(uint32_t) * MAXMESHINSTANCES;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, resourceViews));
            m_descriptors.push_back(resourceViews);

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
        }
#define TEST 100

        // descriptor pool
        Log::Message("Renderer", "Create Descriptor pool");
        {
            nri::DescriptorPoolDesc descriptorPoolDesc = {};
            descriptorPoolDesc.descriptorSetMaxNum = TEST + BUFFERED_FRAME_MAX_NUM + 2;
            descriptorPoolDesc.textureMaxNum = TEST * TEXTURES_PER_MATERIAL;
            descriptorPoolDesc.samplerMaxNum = BUFFERED_FRAME_MAX_NUM * 2 * 5;
            descriptorPoolDesc.storageStructuredBufferMaxNum = 1 * 2 * TEST;
            descriptorPoolDesc.storageBufferMaxNum = 1 * 2 * TEST;
            descriptorPoolDesc.bufferMaxNum = 3 * 2 * TEST;
            descriptorPoolDesc.structuredBufferMaxNum = 4 * 2 * TEST;
            descriptorPoolDesc.constantBufferMaxNum = BUFFERED_FRAME_MAX_NUM;

            NRI_ABORT_ON_FAILURE(NRI.CreateDescriptorPool(*m_Device, descriptorPoolDesc, m_DescriptorPool));
        }

        // descriptor sets
        Log::Message("Renderer", "Create Descriptor Sets");
        {
            m_DescriptorSets.resize(BUFFERED_FRAME_MAX_NUM * (6/*1. 3 pipelines*/ + 2/*material pipeline*/ + 1/*merge pipeline*/));

            // cached descriptor set
            //  range 0
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_cachedRenderLayout, 0,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM * 0], BUFFERED_FRAME_MAX_NUM, 0));
            //  populate

            for (uint32_t i = BUFFERED_FRAME_MAX_NUM * 0; i < BUFFERED_FRAME_MAX_NUM; i++)
            {
                nri::DescriptorRangeUpdateDesc descriptorRangeUpdateDescs[5] = {};
                descriptorRangeUpdateDescs[0].descriptorNum = 1;
                descriptorRangeUpdateDescs[0].descriptors = &constantBufferViews[i];
                descriptorRangeUpdateDescs[1].descriptorNum = 1;
                descriptorRangeUpdateDescs[1].descriptors = &anisotropicSampler;
                descriptorRangeUpdateDescs[2].descriptorNum = GetStructuredBuffCount();
                descriptorRangeUpdateDescs[2].descriptors = resourceViews;

                NRI.UpdateDescriptorRanges(*m_DescriptorSets[i], 0, helper::GetCountOf(descriptorRangeUpdateDescs), descriptorRangeUpdateDescs);
            }

            //  range 1
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_cachedRenderLayout, 1,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM * 1], BUFFERED_FRAME_MAX_NUM, 2));

            


            // culling descriptor set
            //  range 0
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_cullingLayout, 0,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM * 2], BUFFERED_FRAME_MAX_NUM, 0));
            //  range 1
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_cullingLayout, 1,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM * 3], BUFFERED_FRAME_MAX_NUM, 2));

            // vis buff descriptor set
            //  range 0
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_visibilityBuffLayout, 0,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM * 4], BUFFERED_FRAME_MAX_NUM, 0));
            //  range 1
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_visibilityBuffLayout, 1,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM * 5], BUFFERED_FRAME_MAX_NUM, 2));

            // material descriptor set
            //  range 0
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_materialLayout, 0,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM * 6], BUFFERED_FRAME_MAX_NUM, 0));
            //  range 1
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_materialLayout, 1,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM * 7], BUFFERED_FRAME_MAX_NUM, 2));

            // merge descriptor set
            //  range 0
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_mergeLayout, 0,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM * 8], BUFFERED_FRAME_MAX_NUM, 0));

            

            // Material
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_PipelineLayout, MATERIAL_DESCRIPTOR_SET,
                &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM], 1, textureNum));
            nri::DescriptorRangeUpdateDesc descriptorRangeUpdateDesc = {};
            descriptorRangeUpdateDesc.descriptorNum = textureNum;
            descriptorRangeUpdateDesc.descriptors = m_Descriptors.data();
            NRI.UpdateDescriptorRanges(*m_DescriptorSets[BUFFERED_FRAME_MAX_NUM], 0, 1, &descriptorRangeUpdateDesc);

            // Culling
            nri::Descriptor* storageDescriptors[2] = { m_IndirectBufferCountStorageAttachement, m_IndirectBufferStorageAttachement };
            NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_ComputePipelineLayout, 0, &m_DescriptorSets[BUFFERED_FRAME_MAX_NUM + 1], 1, 0));
            nri::DescriptorRangeUpdateDesc rangeUpdateDescs[2] = {};
            rangeUpdateDescs[0].descriptorNum = helper::GetCountOf(rangeUpdateDescs);
            rangeUpdateDescs[0].descriptors = storageDescriptors;
            rangeUpdateDescs[1].descriptorNum = BUFFER_COUNT;
            rangeUpdateDescs[1].descriptors = resourceViews;
            NRI.UpdateDescriptorRanges(*m_DescriptorSets[BUFFERED_FRAME_MAX_NUM + 1], 0, 2, rangeUpdateDescs);
        }
    }


    return true;
}

void Renderer::PrepareFrame(uint32_t frameIndex)
{

}

void Renderer::RenderFrame(uint32_t frameIndex)
{

}

void Renderer::ReloadRenderer()
{
    WindowManager::Init();

    m_vGeomStreamer = new VirtualGeometryStreamer();
    m_vGeomStreamer->Init();


}

void Renderer::PopulateInstaceBuffer()
{
    // create instance buffer
    
    // create old instance buffer idx
}

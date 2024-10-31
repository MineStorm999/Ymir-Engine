#include "Renderer.hpp"
#include "NRICompatibility.hlsli"
#include "../Shaders/SceneViewerBindlessStructs.h"
#include <memory>

#include "Editor/ToolBarWindowManager.h"
#include <chrono>
#include "ECS/components.h"
#include "ECS/ECSManager.h"
#include "World/SceneManager.h"
#include "Log/Log.h"


Sample::~Sample()
{
    NRI.WaitForIdle(*m_CommandQueue);

    for (Frame1& frame : m_Frames)
    {
        NRI.DestroyCommandBuffer(*frame.commandBuffer);
        NRI.DestroyCommandAllocator(*frame.commandAllocator);
    }

    for (uint32_t i = 0; i < m_SwapChainBuffers.size(); i++)
        NRI.DestroyDescriptor(*m_SwapChainBuffers[i].colorAttachment);

    for (size_t i = 0; i < m_Descriptors.size(); i++)
        NRI.DestroyDescriptor(*m_Descriptors[i]);

    for (size_t i = 0; i < m_Textures.size(); i++)
        NRI.DestroyTexture(*m_Textures[i]);

    for (size_t i = 0; i < m_Buffers.size(); i++)
        NRI.DestroyBuffer(*m_Buffers[i]);

    for (size_t i = 0; i < m_MemoryAllocations.size(); i++)
        NRI.FreeMemory(*m_MemoryAllocations[i]);

    NRI.DestroyPipeline(*m_Pipeline);
    NRI.DestroyPipeline(*m_ComputePipeline);

    NRI.DestroyQueryPool(*m_QueryPool);
    NRI.DestroyPipelineLayout(*m_PipelineLayout);
    NRI.DestroyPipelineLayout(*m_ComputePipelineLayout);
    NRI.DestroyDescriptorPool(*m_DescriptorPool);
    NRI.DestroyFence(*m_FrameFence);
    NRI.DestroySwapChain(*m_SwapChain);
    NRI.DestroyStreamer(*m_Streamer);

    DestroyUI(NRI);

    nri::nriDestroyDevice(*m_Device);
}

bool Sample::Initialize(nri::GraphicsAPI graphicsAPI)
{
    
    Log::Init();

    EntityManager::Init();

    AssetManager::Init();
    
    if (graphicsAPI == nri::GraphicsAPI::D3D11) {
        printf("This sample supports only D3D12 and Vulkan.");
        return false;
    }

    nri::AdapterDesc bestAdapterDesc = {};
    uint32_t adapterDescsNum = 1;
    NRI_ABORT_ON_FAILURE(nri::nriEnumerateAdapters(&bestAdapterDesc, adapterDescsNum));

    // Device
    nri::DeviceCreationDesc deviceCreationDesc = {};
    deviceCreationDesc.graphicsAPI = graphicsAPI;
    deviceCreationDesc.enableGraphicsAPIValidation = m_DebugAPI;
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

    RenderScene* scene = SceneManager::GetRenderScene();//                                                              remember to create default scene
    if (!scene) {
        Log::Error("Renderer", "Initiation Failed, No scene");
        return false;
    }
    scene->Load();
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
    for (Frame1& frame : m_Frames)
    {
        NRI_ABORT_ON_FAILURE(NRI.CreateCommandAllocator(*m_CommandQueue, frame.commandAllocator));
        NRI_ABORT_ON_FAILURE(NRI.CreateCommandBuffer(*frame.commandAllocator, frame.commandBuffer));
    }

    // Pipeline
    const nri::DeviceDesc& deviceDesc = NRI.GetDeviceDesc(*m_Device);
    utils::ShaderCodeStorage shaderCodeStorage;
    {
        // batch descs
        
        {
            nri::DescriptorRangeDesc globalDescriptorRange[3] = {};
            globalDescriptorRange[0] = { (uint32_t)((deviceDesc.graphicsAPI == nri::GraphicsAPI::D3D12) ? 0 : 0), 1, nri::DescriptorType::CONSTANT_BUFFER, nri::StageBits::ALL };
            globalDescriptorRange[1] = { 0, 1, nri::DescriptorType::SAMPLER, nri::StageBits::FRAGMENT_SHADER };
            globalDescriptorRange[2] = { 0, BUFFER_COUNT, nri::DescriptorType::STRUCTURED_BUFFER, nri::StageBits::ALL };

            // Bindless descriptors
            nri::DescriptorRangeDesc textureDescriptorRange[1] = {};
            textureDescriptorRange[0] = { 0, 512, nri::DescriptorType::TEXTURE, nri::StageBits::FRAGMENT_SHADER, true, true };

            

            nri::DescriptorSetDesc descriptorSetDescs[] =
            {
                {0, globalDescriptorRange, helper::GetCountOf(globalDescriptorRange)},
                {1, textureDescriptorRange, helper::GetCountOf(textureDescriptorRange), nullptr, 0, true },
            };

            nri::PipelineLayoutDesc pipelineLayoutDesc = {};
            pipelineLayoutDesc.descriptorSetNum = helper::GetCountOf(descriptorSetDescs);
            pipelineLayoutDesc.descriptorSets = descriptorSetDescs;
            pipelineLayoutDesc.shaderStages = nri::StageBits::VERTEX_SHADER | nri::StageBits::FRAGMENT_SHADER;
            pipelineLayoutDesc.enableD3D12DrawParametersEmulation = true;

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, pipelineLayoutDesc, m_PipelineLayout));
        }

        {
            nri::DescriptorRangeDesc descriptorRange[2] = {};
            descriptorRange[0] = { 0, 2, nri::DescriptorType::STORAGE_BUFFER, nri::StageBits::COMPUTE_SHADER };
            descriptorRange[1] = { 0, BUFFER_COUNT, nri::DescriptorType::STRUCTURED_BUFFER, nri::StageBits::COMPUTE_SHADER };

            nri::DescriptorSetDesc descriptorSetDescs[] =
            {
                {0, descriptorRange, helper::GetCountOf(descriptorRange)},
            };

            nri::PushConstantDesc pushConstantDesc = {};
            pushConstantDesc.registerIndex = 0;
            pushConstantDesc.shaderStages = nri::StageBits::COMPUTE_SHADER;
            pushConstantDesc.size = sizeof(CullingConstants);

            nri::PipelineLayoutDesc pipelineLayoutDesc = {};
            pipelineLayoutDesc.pushConstantNum = 1;
            pipelineLayoutDesc.pushConstants = &pushConstantDesc;
            pipelineLayoutDesc.descriptorSetNum = helper::GetCountOf(descriptorSetDescs);
            pipelineLayoutDesc.descriptorSets = descriptorSetDescs;
            pipelineLayoutDesc.shaderStages = nri::StageBits::COMPUTE_SHADER;

            NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*m_Device, pipelineLayoutDesc, m_ComputePipelineLayout));
        }

        nri::VertexStreamDesc vertexStreamDesc = {};
        vertexStreamDesc.bindingSlot = 0;
        vertexStreamDesc.stride = sizeof(utils::Vertex);

        nri::VertexAttributeDesc vertexAttributeDesc[4] = {};
        {
            vertexAttributeDesc[0].format = nri::Format::RGB32_SFLOAT;
            vertexAttributeDesc[0].offset = helper::GetOffsetOf(&utils::Vertex::pos);
            vertexAttributeDesc[0].d3d = { "POSITION", 0 };
            vertexAttributeDesc[0].vk = { 0 };

            vertexAttributeDesc[1].format = nri::Format::RG16_SFLOAT;
            vertexAttributeDesc[1].offset = helper::GetOffsetOf(&utils::Vertex::uv);
            vertexAttributeDesc[1].d3d = { "TEXCOORD", 0 };
            vertexAttributeDesc[1].vk = { 1 };

            vertexAttributeDesc[2].format = nri::Format::R10_G10_B10_A2_UNORM;
            vertexAttributeDesc[2].offset = helper::GetOffsetOf(&utils::Vertex::N);
            vertexAttributeDesc[2].d3d = { "NORMAL", 0 };
            vertexAttributeDesc[2].vk = { 2 };

            vertexAttributeDesc[3].format = nri::Format::R10_G10_B10_A2_UNORM;
            vertexAttributeDesc[3].offset = helper::GetOffsetOf(&utils::Vertex::T);
            vertexAttributeDesc[3].d3d = { "TANGENT", 0 };
            vertexAttributeDesc[3].vk = { 3 };
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
        rasterizationDesc.cullMode = nri::CullMode::BACK;
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

        nri::ShaderDesc shaderStages[] =
        {
            utils::LoadShader(deviceDesc.graphicsAPI, "ForwardBindless.vs", shaderCodeStorage),
            utils::LoadShader(deviceDesc.graphicsAPI, "ForwardBindless.fs", shaderCodeStorage),
        };

        nri::GraphicsPipelineDesc graphicsPipelineDesc = {};
        graphicsPipelineDesc.pipelineLayout = m_PipelineLayout;
        graphicsPipelineDesc.vertexInput = &vertexInputDesc;
        graphicsPipelineDesc.inputAssembly = inputAssemblyDesc;
        graphicsPipelineDesc.rasterization = rasterizationDesc;
        graphicsPipelineDesc.multisample = &multisampleDesc;
        graphicsPipelineDesc.outputMerger = outputMergerDesc;
        graphicsPipelineDesc.shaders = shaderStages;
        graphicsPipelineDesc.shaderNum = helper::GetCountOf(shaderStages);
        NRI_ABORT_ON_FAILURE(NRI.CreateGraphicsPipeline(*m_Device, graphicsPipelineDesc, m_Pipeline));
    }

    {
        nri::ComputePipelineDesc computePipelineDesc = {};
        computePipelineDesc.pipelineLayout = m_ComputePipelineLayout;
        computePipelineDesc.shader = utils::LoadShader(deviceDesc.graphicsAPI, "GenerateSceneDrawCalls.cs", shaderCodeStorage);
        NRI_ABORT_ON_FAILURE(NRI.CreateComputePipeline(*m_Device, computePipelineDesc, m_ComputePipeline));
    }

    // Scene
    //std::string sceneFile = utils::GetFullPath(m_SceneFile, utils::DataFolder::SCENES);
    //NRI_ABORT_ON_FALSE(utils::LoadScene(sceneFile, m_Scene, false));

    // Camera
    m_Camera.Initialize({ 0,0,0 }, {0,1,0}, false);

    const uint32_t textureNum = (uint32_t)scene->texturesCPU.size();
    const uint32_t materialNum = (uint32_t)scene->materialsCPU.size();

    { // Textures
        for (const utils::Texture* textureData : scene->texturesCPU)
        {
            nri::TextureDesc textureDesc = nri::Texture2D(textureData->GetFormat(),
                textureData->GetWidth(), textureData->GetHeight(), textureData->GetMipNum(), textureData->GetArraySize());

            nri::Texture* texture;
            NRI_ABORT_ON_FAILURE(NRI.CreateTexture(*m_Device, textureDesc, texture));
            m_Textures.push_back(texture);
        }
    }

    // Depth attachment
    nri::Texture* depthTexture;
    {
        nri::TextureDesc textureDesc = nri::Texture2D(m_DepthFormat, (uint16_t)GetWindowResolution().x, (uint16_t)GetWindowResolution().y, 1, 1,
            nri::TextureUsageBits::DEPTH_STENCIL_ATTACHMENT);

        NRI_ABORT_ON_FAILURE(NRI.CreateTexture(*m_Device, textureDesc, depthTexture));
        m_Textures.push_back(depthTexture);
    }

    const uint32_t constantBufferSize = helper::Align((uint32_t)sizeof(GlobalConstants), deviceDesc.constantBufferOffsetAlignment);

    // debug
    std::cout << "Create Buffers\n";
    auto start = std::chrono::high_resolution_clock::now();


    { // Buffers
        // CONSTANT_BUFFER
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = constantBufferSize * BUFFERED_FRAME_MAX_NUM;
        bufferDesc.usageMask = nri::BufferUsageBits::CONSTANT_BUFFER;
        nri::Buffer* buffer;

        std::cout << "Create Buffers1\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);

        // READBACK_BUFFER
        bufferDesc.size = sizeof(nri::PipelineStatisticsDesc) * BUFFERED_FRAME_MAX_NUM;
        bufferDesc.usageMask = nri::BufferUsageBits::NONE;
        std::cout << "Create Buffers2\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);


        // INDEX_BUFFER
        bufferDesc.size = helper::GetByteSizeOf(scene->indicesCPU);
        bufferDesc.usageMask = nri::BufferUsageBits::INDEX_BUFFER;
        std::cout << "Create Buffers3\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);

        // VERTEX_BUFFER
        bufferDesc.size = helper::GetByteSizeOf(scene->verticesCPU);
        bufferDesc.usageMask = nri::BufferUsageBits::VERTEX_BUFFER;
        std::cout << "Create Buffers4\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);

        // MATERIAL_BUFFER
        bufferDesc.size = max(sizeof(MaterialData), helper::GetByteSizeOf(scene->materialsCPU));
        bufferDesc.structureStride = sizeof(MaterialData);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        std::cout << "Create Buffers5\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);

        // MESH_BUFFER
        bufferDesc.size = helper::GetByteSizeOf(scene->meshesCPU);
        bufferDesc.structureStride = sizeof(MeshData);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        std::cout << "Create Buffers5\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);

        // INSTANCE_BUFFER
        bufferDesc.size = MAX_TRANSFORMS * sizeof(InstanceData);
        bufferDesc.structureStride = sizeof(InstanceData);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        std::cout << "Create Buffers7\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);

        // INSTANCE_MATRIX_BUFFER
        bufferDesc.size = MAX_TRANSFORMS * sizeof(float4x4);
        bufferDesc.structureStride = sizeof(float4x4);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        std::cout << "Create Buffers8\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);


        // INDIRECT_BUFFER
        bufferDesc.size = MAX_TRANSFORMS * GetDrawIndexedCommandSize();
        bufferDesc.structureStride = 0;
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE | nri::BufferUsageBits::ARGUMENT_BUFFER;
        std::cout << "Create Buffers9\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);

        // INDIRECT_COUNT_BUFFER
        bufferDesc.size = sizeof(uint32_t);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE | nri::BufferUsageBits::ARGUMENT_BUFFER;
        std::cout << "Create Buffers10\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);
    }
    std::cout << "Buffers Created\n";
    std::cout << "Allocate Memory\n";
    { // Memory
        nri::ResourceGroupDesc resourceGroupDesc = {};
        resourceGroupDesc.memoryLocation = nri::MemoryLocation::HOST_UPLOAD;
        resourceGroupDesc.bufferNum = 1;
        resourceGroupDesc.buffers = &m_Buffers[CONSTANT_BUFFER];

        size_t baseAllocation = m_MemoryAllocations.size();
        m_MemoryAllocations.resize(baseAllocation + 1, nullptr);
        NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_MemoryAllocations.data() + baseAllocation));
        std::cout << "Allocated Memory1\n";
        resourceGroupDesc.memoryLocation = nri::MemoryLocation::HOST_READBACK;
        resourceGroupDesc.bufferNum = 1;
        resourceGroupDesc.buffers = &m_Buffers[READBACK_BUFFER];

        baseAllocation = m_MemoryAllocations.size();
        m_MemoryAllocations.resize(baseAllocation + 1, nullptr);
        NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_MemoryAllocations.data() + baseAllocation));
        std::cout << "Allocated Memory2\n";
        resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
        resourceGroupDesc.bufferNum = (uint32_t)SceneBuffers::MAX_NUM - 2;
        resourceGroupDesc.buffers = &m_Buffers[INDEX_BUFFER];
        resourceGroupDesc.textureNum = (uint32_t)m_Textures.size();
        resourceGroupDesc.textures = m_Textures.data();

        std::cout << "Allocated Texture num: " << m_Textures.size() << "\n";
        //std::cout << "Allocated buffer num: " << m_Textures.size();

        baseAllocation = m_MemoryAllocations.size();
        uint32_t allocationNum = NRI.CalculateAllocationNumber(*m_Device, resourceGroupDesc);
        m_MemoryAllocations.resize(baseAllocation + allocationNum, nullptr);

        

        std::cout << "Allocating Memory3\n";
        NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_MemoryAllocations.data() + baseAllocation));
        std::cout << "Allocated Memory3\n";
    }

    // Create descriptors
    nri::Descriptor* anisotropicSampler = nullptr;
    nri::Descriptor* constantBufferViews[BUFFERED_FRAME_MAX_NUM] = {};
    {
        // Material textures
        m_Descriptors.resize(textureNum);
        for (uint32_t i = 0; i < textureNum; i++)
        {
            const utils::Texture& texture = *scene->texturesCPU[i];
            nri::Texture2DViewDesc texture2DViewDesc = { m_Textures[i], nri::Texture2DViewType::SHADER_RESOURCE_2D, texture.GetFormat() };
            NRI_ABORT_ON_FAILURE(NRI.CreateTexture2DView(texture2DViewDesc, m_Descriptors[i]));
        }

        // Sampler
        nri::SamplerDesc samplerDesc = {};
        samplerDesc.addressModes = { nri::AddressMode::REPEAT, nri::AddressMode::REPEAT };
        samplerDesc.filters = { nri::Filter::LINEAR, nri::Filter::LINEAR, nri::Filter::LINEAR };
        samplerDesc.anisotropy = 8;
        samplerDesc.mipMax = 16.0f;
        NRI_ABORT_ON_FAILURE(NRI.CreateSampler(*m_Device, samplerDesc, anisotropicSampler));
        m_Descriptors.push_back(anisotropicSampler);

        nri::BufferViewDesc bufferViewDesc = {};
        bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE;
        bufferViewDesc.offset = 0;

        // Material buffer 
        bufferViewDesc.buffer = m_Buffers[MATERIAL_BUFFER];
        bufferViewDesc.size = max(sizeof(MaterialData), helper::GetByteSizeOf(scene->materialsCPU));
        NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, m_recourceDescs[0]));
        m_Descriptors.push_back(m_recourceDescs[0]);

        // Mesh buffer 
        bufferViewDesc.buffer = m_Buffers[MESH_BUFFER];
        bufferViewDesc.size = helper::GetByteSizeOf(scene->meshesCPU);
        NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, m_recourceDescs[1]));
        m_Descriptors.push_back(m_recourceDescs[1]);

        // Instance buffer 
        bufferViewDesc.buffer = m_Buffers[INSTANCE_BUFFER];
        bufferViewDesc.size = MAX_TRANSFORMS * sizeof(InstanceData);
        NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, m_recourceDescs[2]));
        m_Descriptors.push_back(m_recourceDescs[2]);

        // instance matrix Buffer
        bufferViewDesc.buffer = m_Buffers[INSTANCE_MATRIX_BUFFER];
        bufferViewDesc.size = MAX_TRANSFORMS * sizeof(float4x4);
        NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, m_recourceDescs[3]));
        m_Descriptors.push_back(m_recourceDescs[3]);

        // Indirect buffer 
        bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE_STORAGE;
        bufferViewDesc.buffer = m_Buffers[INDIRECT_BUFFER];
        bufferViewDesc.size = MAX_TRANSFORMS * GetDrawIndexedCommandSize();
        bufferViewDesc.format = nri::Format::R32_UINT;
        NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, m_IndirectBufferStorageAttachement));
        m_Descriptors.push_back(m_IndirectBufferStorageAttachement);

        // Indirect draw count buffer
        bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE_STORAGE;
        bufferViewDesc.buffer = m_Buffers[INDIRECT_COUNT_BUFFER];
        bufferViewDesc.size = sizeof(uint32_t);
        bufferViewDesc.format = nri::Format::R32_UINT;
        NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, m_IndirectBufferCountStorageAttachement));
        m_Descriptors.push_back(m_IndirectBufferCountStorageAttachement);

        bufferViewDesc.format = nri::Format::UNKNOWN;

        // Constant buffer
        for (uint32_t i = 0; i < BUFFERED_FRAME_MAX_NUM; i++)
        {
            m_Frames[i].globalConstantBufferViewOffsets = i * constantBufferSize;
            bufferViewDesc.buffer = m_Buffers[CONSTANT_BUFFER];
            bufferViewDesc.viewType = nri::BufferViewType::CONSTANT;
            bufferViewDesc.offset = i * constantBufferSize;
            bufferViewDesc.size = constantBufferSize;
            NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, constantBufferViews[i]));
            m_Descriptors.push_back(constantBufferViews[i]);
        }

        // Depth buffer
        nri::Texture2DViewDesc texture2DViewDesc = { depthTexture, nri::Texture2DViewType::DEPTH_STENCIL_ATTACHMENT, m_DepthFormat };

        NRI_ABORT_ON_FAILURE(NRI.CreateTexture2DView(texture2DViewDesc, m_DepthAttachment));
        m_Descriptors.push_back(m_DepthAttachment);

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

#define TEST 100

    { // Descriptor pool
        nri::DescriptorPoolDesc descriptorPoolDesc = {};
        descriptorPoolDesc.descriptorSetMaxNum = ((BUFFERED_FRAME_MAX_NUM + 1) * 3) + 1 * TEST;
        descriptorPoolDesc.textureMaxNum = materialNum * TEXTURES_PER_MATERIAL;
        descriptorPoolDesc.samplerMaxNum = BUFFERED_FRAME_MAX_NUM;
        descriptorPoolDesc.storageStructuredBufferMaxNum = 1 * 2 * TEST;
        descriptorPoolDesc.storageBufferMaxNum = 1 * 2 * TEST;
        descriptorPoolDesc.bufferMaxNum = BUFFER_COUNT * 2 * TEST;
        descriptorPoolDesc.structuredBufferMaxNum = 4 * 2 * TEST;
        descriptorPoolDesc.constantBufferMaxNum = BUFFERED_FRAME_MAX_NUM;

        NRI_ABORT_ON_FAILURE(NRI.CreateDescriptorPool(*m_Device, descriptorPoolDesc, m_DescriptorPool));
    }

    { // Descriptor sets
        m_DescriptorSets.resize(BUFFERED_FRAME_MAX_NUM + 2);

        // Global
        NRI_ABORT_ON_FAILURE(NRI.AllocateDescriptorSets(*m_DescriptorPool, *m_PipelineLayout, GLOBAL_DESCRIPTOR_SET,
            &m_DescriptorSets[0], BUFFERED_FRAME_MAX_NUM, 0));

        for (uint32_t i = 0; i < BUFFERED_FRAME_MAX_NUM; i++)
        {
            nri::DescriptorRangeUpdateDesc descriptorRangeUpdateDescs[3] = {};
            descriptorRangeUpdateDescs[0].descriptorNum = 1;
            descriptorRangeUpdateDescs[0].descriptors = &constantBufferViews[i];
            descriptorRangeUpdateDescs[1].descriptorNum = 1;
            descriptorRangeUpdateDescs[1].descriptors = &anisotropicSampler;
            descriptorRangeUpdateDescs[2].descriptorNum = BUFFER_COUNT;
            descriptorRangeUpdateDescs[2].descriptors = m_recourceDescs;

            NRI.UpdateDescriptorRanges(*m_DescriptorSets[i], 0, helper::GetCountOf(descriptorRangeUpdateDescs), descriptorRangeUpdateDescs);
        }

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
        rangeUpdateDescs[1].descriptors = m_recourceDescs;
        NRI.UpdateDescriptorRanges(*m_DescriptorSets[BUFFERED_FRAME_MAX_NUM + 1], 0, 2, rangeUpdateDescs);
    }

    { // Upload data
        // collect data
        std::vector<nri::TextureUploadDesc> textureData(1 + textureNum);

        uint32_t subresourceNum = 0;
        for (uint32_t i = 0; i < textureNum; i++)
        {
            const utils::Texture& texture = *scene->texturesCPU[i];
            subresourceNum += texture.GetArraySize() * texture.GetMipNum();
        }

        std::vector<nri::TextureSubresourceUploadDesc> subresources(subresourceNum);;
        nri::TextureSubresourceUploadDesc* subresourceBegin = subresources.data();

        textureData[0] = {};
        textureData[0].subresources = nullptr;
        textureData[0].texture = depthTexture;
        textureData[0].after = { nri::AccessBits::DEPTH_STENCIL_WRITE, nri::Layout::DEPTH_STENCIL };

        for (uint32_t i = 0; i < textureNum; i++)
        {
            const utils::Texture& texture = *scene->texturesCPU[i];

            for (uint32_t slice = 0; slice < texture.GetArraySize(); slice++)
            {
                for (uint32_t mip = 0; mip < texture.GetMipNum(); mip++)
                    texture.GetSubresource(subresourceBegin[slice * texture.GetMipNum() + mip], mip, slice);
            }

            const uint32_t j = i + 1;
            textureData[j] = {};
            textureData[j].subresources = subresourceBegin;
            textureData[j].texture = m_Textures[i];
            textureData[j].after = { nri::AccessBits::SHADER_RESOURCE, nri::Layout::SHADER_RESOURCE };

            subresourceBegin += texture.GetArraySize() * texture.GetMipNum();
        }

        void* dummyInstances = malloc(MAX_TRANSFORMS * sizeof(InstanceData));
        if (!dummyInstances) {
            return false;
        }
        memset(dummyInstances, (uint8_t)0xff, MAX_TRANSFORMS * sizeof(InstanceData));

        nri::BufferUploadDesc bufferData[] =
        {
            {nullptr, 0,  m_Buffers[INDIRECT_BUFFER], 0, {nri::AccessBits::ARGUMENT_BUFFER, nri::StageBits::INDIRECT }},
            {dummyInstances, MAX_TRANSFORMS * sizeof(InstanceData), m_Buffers[INSTANCE_BUFFER], 0,  {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::FRAGMENT_SHADER | nri::StageBits::COMPUTE_SHADER}},
            {scene->meshesCPU.data(), scene->meshesCPU.size() * sizeof(MeshData), m_Buffers[MESH_BUFFER], 0,  {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::FRAGMENT_SHADER | nri::StageBits::COMPUTE_SHADER}},
            //{scene->materialsCPU.data(), scene->materialsCPU.size() * sizeof(MaterialData), m_Buffers[MATERIAL_BUFFER], 0,  {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::FRAGMENT_SHADER | nri::StageBits::COMPUTE_SHADER}},
            {scene->verticesCPU.data(), helper::GetByteSizeOf(scene->verticesCPU), m_Buffers[VERTEX_BUFFER], 0, {nri::AccessBits::VERTEX_BUFFER}},
            {scene->indicesCPU.data(), helper::GetByteSizeOf(scene->indicesCPU), m_Buffers[INDEX_BUFFER], 0, {nri::AccessBits::INDEX_BUFFER}},
        };

        NRI_ABORT_ON_FAILURE(NRI.UploadData(*m_CommandQueue, /*textureData.data() */nullptr , 0/*(uint32_t)textureData.size()*/, bufferData, helper::GetCountOf(bufferData)));
        free(dummyInstances);
    }

    { // Pipeline statistics
        nri::QueryPoolDesc queryPoolDesc = {};
        queryPoolDesc.queryType = nri::QueryType::PIPELINE_STATISTICS;
        queryPoolDesc.capacity = 1;

        NRI_ABORT_ON_FAILURE(NRI.CreateQueryPool(*m_Device, queryPoolDesc, m_QueryPool));
    }

    scene->UnLoad();

    // Code to time here...
    auto end = std::chrono::high_resolution_clock::now();

    auto dur = end - start;
    //auto i_millis = std::chrono::duration_cast<std::chrono::milliseconds>(dur);
    auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);

    std::cout << "Buffer Creation & stuff took: " << f_secs.count() << "seconds\n";



    bool initSuccsess = InitUI(NRI, NRI, *m_Device, swapChainFormat);
    if (!initSuccsess) {
        return false;
    }
    WindowManager::Init();

    scene->rel = false;
    scene->relMeshes = false;
    scene->relMaterials = false;
    scene->relTextures = false;
    return true;
}

float lastTime = 0;

void Sample::PrepareFrame(uint32_t frameIndex)
{
    BeginUI();
    float dt = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();
    float fps = 1 / dt;
    //    exit(0);
    //}
    ProzessAddRrmRequests();

    WindowManager::Show();

    // TODO: delay is not implemented
    nri::PipelineStatisticsDesc* pipelineStats = (nri::PipelineStatisticsDesc*)NRI.MapBuffer(*m_Buffers[READBACK_BUFFER], 0, sizeof(nri::PipelineStatisticsDesc));
    {
        ImGui::SetNextWindowPos(ImVec2(30, 30), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        ImGui::Begin("Stats");
        {
            ImGui::Text("fps                          : %.3f", fps);
            ImGui::Text("Input vertices               : %llu", pipelineStats->inputVertexNum);
            ImGui::Text("Input primitives             : %llu", pipelineStats->inputPrimitiveNum);
            ImGui::Text("Vertex shader invocations    : %llu", pipelineStats->vertexShaderInvocationNum);
            ImGui::Text("Rasterizer input primitives  : %llu", pipelineStats->rasterizerInPrimitiveNum);
            ImGui::Text("Rasterizer output primitives : %llu", pipelineStats->rasterizerOutPrimitiveNum);
            ImGui::Text("Fragment shader invocations  : %llu", pipelineStats->fragmentShaderInvocationNum);
            ImGui::Checkbox("GPU draw call generation", &m_UseGPUDrawGeneration);
        }
        ImGui::End();
    }
    NRI.UnmapBuffer(*m_Buffers[READBACK_BUFFER]);

    EndUI(NRI, *m_Streamer);
    NRI.CopyStreamerUpdateRequests(*m_Streamer);

    CameraDesc desc = {};
    desc.aspectRatio = float(GetWindowResolution().x) / float(GetWindowResolution().y);
    desc.horizontalFov = 90.0f;
    desc.nearZ = 0.1f;
    desc.isReversedZ = (CLEAR_DEPTH == 0.0f);
    GetCameraDescFromInputDevices(desc);



    m_Camera.Update(desc, frameIndex);

}

void Sample::RenderFrame(uint32_t frameIndex)
{
    const uint32_t bufferedFrameIndex = frameIndex % BUFFERED_FRAME_MAX_NUM;
    const Frame1& frame = m_Frames[bufferedFrameIndex];
    const uint32_t windowWidth = GetWindowResolution().x;
    const uint32_t windowHeight = GetWindowResolution().y;

    if (frameIndex >= BUFFERED_FRAME_MAX_NUM)
    {
        NRI.Wait(*m_FrameFence, 1 + frameIndex - BUFFERED_FRAME_MAX_NUM);
        NRI.ResetCommandAllocator(*frame.commandAllocator);
    }

    const uint32_t currentTextureIndex = NRI.AcquireNextSwapChainTexture(*m_SwapChain);
    BackBuffer& currentBackBuffer = m_SwapChainBuffers[currentTextureIndex];


    uint32_t batchCount = PrepareEntities();

    // Update constants
    const uint64_t rangeOffset = m_Frames[bufferedFrameIndex].globalConstantBufferViewOffsets;
    auto constants = (GlobalConstants*)NRI.MapBuffer(*m_Buffers[CONSTANT_BUFFER], rangeOffset, sizeof(GlobalConstants));
    if (constants)
    {
        constants->gWorldToClip = m_Camera.state.mWorldToClip * m_Scene.mSceneToWorld;
        constants->gCameraPos = m_Camera.state.position;
        constants->batchCount = batchCount;
        NRI.UnmapBuffer(*m_Buffers[CONSTANT_BUFFER]);
    }



    // Record
    nri::CommandBuffer& commandBuffer = *frame.commandBuffer;
    NRI.BeginCommandBuffer(commandBuffer, m_DescriptorPool);
    {
        helper::Annotation annotation(NRI, commandBuffer, "Scene");

        nri::AttachmentsDesc attachmentsDesc = {};
        attachmentsDesc.colorNum = 1;
        attachmentsDesc.colors = &currentBackBuffer.colorAttachment;
        attachmentsDesc.depthStencil = m_DepthAttachment;

        nri::TextureBarrierDesc textureBarrierDescs = {};
        textureBarrierDescs.texture = currentBackBuffer.texture;
        textureBarrierDescs.after = { nri::AccessBits::COLOR_ATTACHMENT, nri::Layout::COLOR_ATTACHMENT };
        textureBarrierDescs.arraySize = 1;
        textureBarrierDescs.mipNum = 1;

        nri::BarrierGroupDesc computeBarrierGroupDesc = {};
        nri::BufferBarrierDesc bufferBarrierDesc = {};
        bufferBarrierDesc.buffer = m_Buffers[INDIRECT_BUFFER];
        bufferBarrierDesc.before = { nri::AccessBits::ARGUMENT_BUFFER, nri::StageBits::INDIRECT };
        bufferBarrierDesc.after = { nri::AccessBits::SHADER_RESOURCE_STORAGE, nri::StageBits::COMPUTE_SHADER };
        computeBarrierGroupDesc.bufferNum = 1;
        computeBarrierGroupDesc.buffers = &bufferBarrierDesc;

        nri::BarrierGroupDesc barrierGroupDesc = {};
        barrierGroupDesc.textureNum = 1;
        barrierGroupDesc.textures = &textureBarrierDescs;
        if (m_UseGPUDrawGeneration) {
            barrierGroupDesc.bufferNum = 1;
            barrierGroupDesc.buffers = &bufferBarrierDesc;
        }

        NRI.CmdBarrier(commandBuffer, barrierGroupDesc);

        if (m_UseGPUDrawGeneration) {
            NRI.CmdSetPipelineLayout(commandBuffer, *m_ComputePipelineLayout);
            NRI.CmdSetDescriptorSet(commandBuffer, 0, *m_DescriptorSets[BUFFERED_FRAME_MAX_NUM + 1], nullptr);

            // Culling
            CullingConstants cullingConstants = {};
            cullingConstants.DrawCount = batchCount;

            if (RenderScene* rs = SceneManager::GetRenderScene()) {
                cullingConstants.MeshCount = rs->modelCount;
            }

            NRI.CmdSetConstants(commandBuffer, 0, &cullingConstants, sizeof(cullingConstants));

            NRI.CmdSetPipeline(commandBuffer, *m_ComputePipeline);
            NRI.CmdDispatch(commandBuffer, { 1, 1, 1 });

            // Transition from UAV to indirect argument
            bufferBarrierDesc.after = { nri::AccessBits::ARGUMENT_BUFFER, nri::StageBits::INDIRECT };
            bufferBarrierDesc.before = { nri::AccessBits::SHADER_RESOURCE_STORAGE, nri::StageBits::COMPUTE_SHADER };
            NRI.CmdBarrier(commandBuffer, computeBarrierGroupDesc);
        }

        NRI.CmdResetQueries(commandBuffer, *m_QueryPool, 0, 1);
        NRI.CmdBeginQuery(commandBuffer, *m_QueryPool, 0);
        {
            NRI.CmdBeginRendering(commandBuffer, attachmentsDesc);
            {
                nri::ClearDesc clearDescs[2] = {};
                clearDescs[0].attachmentContentType = nri::AttachmentContentType::COLOR;
                clearDescs[0].value.color32f = { .0f, .0f, .5f };
                clearDescs[1].attachmentContentType = nri::AttachmentContentType::DEPTH;
                clearDescs[1].value.depthStencil.depth = CLEAR_DEPTH;

                NRI.CmdClearAttachments(commandBuffer, clearDescs, helper::GetCountOf(clearDescs), nullptr, 0);

                const nri::Viewport viewport = { 0.0f, 0.0f, (float)windowWidth, (float)windowHeight, 0.0f, 1.0f };
                NRI.CmdSetViewports(commandBuffer, &viewport, 1);

                const nri::Rect scissor = { 0, 0, (nri::Dim_t)windowWidth, (nri::Dim_t)windowHeight };
                NRI.CmdSetScissors(commandBuffer, &scissor, 1);

                NRI.CmdSetIndexBuffer(commandBuffer, *m_Buffers[INDEX_BUFFER], 0, sizeof(utils::Index) == 2 ? nri::IndexType::UINT16 : nri::IndexType::UINT32);

                NRI.CmdSetPipelineLayout(commandBuffer, *m_PipelineLayout);
                NRI.CmdSetDescriptorSet(commandBuffer, GLOBAL_DESCRIPTOR_SET, *m_DescriptorSets[bufferedFrameIndex], nullptr);
                NRI.CmdSetDescriptorSet(commandBuffer, MATERIAL_DESCRIPTOR_SET, *m_DescriptorSets[BUFFERED_FRAME_MAX_NUM], nullptr);
                NRI.CmdSetPipeline(commandBuffer, *m_Pipeline);

                constexpr uint64_t offset = 0;
                NRI.CmdSetVertexBuffers(commandBuffer, 0, 1, &m_Buffers[VERTEX_BUFFER], &offset);

                if (m_UseGPUDrawGeneration) {
                    NRI.CmdDrawIndexedIndirect(commandBuffer, *m_Buffers[INDIRECT_BUFFER], 0, MAX_TRANSFORMS, GetDrawIndexedCommandSize(), m_Buffers[INDIRECT_COUNT_BUFFER], 0);
                }
                else {
                    exit(1);
                    /*for (uint32_t i = 0; i < m_Scene.instances.size(); i++) {
                        const utils::Instance& instance = m_Scene.instances[i];
                        const utils::Mesh& mesh = m_Scene.meshes[instance.meshInstanceIndex];
                        NRI.CmdDrawIndexed(commandBuffer, { mesh.indexNum, 1, mesh.indexOffset, (int32_t)mesh.vertexOffset, i });
                    }*/
                }
            }
            NRI.CmdEndRendering(commandBuffer);
        }
        NRI.CmdEndQuery(commandBuffer, *m_QueryPool, 0);
        NRI.CmdCopyQueries(commandBuffer, *m_QueryPool, 0, 1, *m_Buffers[READBACK_BUFFER], 0);

        attachmentsDesc.depthStencil = nullptr;

        NRI.CmdBeginRendering(commandBuffer, attachmentsDesc);
        {
            RenderUI(NRI, NRI, *m_Streamer, commandBuffer, 1.0f, true);
        }
        NRI.CmdEndRendering(commandBuffer);

        textureBarrierDescs.before = textureBarrierDescs.after;
        textureBarrierDescs.after = { nri::AccessBits::UNKNOWN, nri::Layout::PRESENT };

        barrierGroupDesc.bufferNum = 0;
        NRI.CmdBarrier(commandBuffer, barrierGroupDesc);
    }
    NRI.EndCommandBuffer(commandBuffer);

    { // Submit
        nri::QueueSubmitDesc queueSubmitDesc = {};
        queueSubmitDesc.commandBuffers = &frame.commandBuffer;
        queueSubmitDesc.commandBufferNum = 1;

        NRI.QueueSubmit(*m_CommandQueue, queueSubmitDesc);
    }

    // Present
    NRI.QueuePresent(*m_SwapChain);

    { // Signaling after "Present" improves D3D11 performance a bit
        nri::FenceSubmitDesc signalFence = {};
        signalFence.fence = m_FrameFence;
        signalFence.value = 1 + frameIndex;

        nri::QueueSubmitDesc queueSubmitDesc = {};
        queueSubmitDesc.signalFences = &signalFence;
        queueSubmitDesc.signalFenceNum = 1;

        NRI.QueueSubmit(*m_CommandQueue, queueSubmitDesc);
    }
}

std::vector<InstanceData> dummys;
std::vector<nri::BufferUploadDesc> descs;

void Sample::UpdateEntityTransform(entt::entity e, TransformComponent& transform, IdentityComponent& identity) {
    RenderScene* rS = SceneManager::GetRenderScene();
    if (!rS) {
        return;
    }

    // transform
    float4x4 transformMat, rotationMap, scaleMap;

    transformMat.SetupByTranslation(transform.localPos);
    //float4x4 tempRot;
    
    float3 radRot = radians(transform.localRot);
    rotationMap.SetupByRotationYPR(radRot.x, radRot.y, radRot.z);

    scaleMap.SetupByScale(transform.localScale);

    transform.localMat = transformMat * rotationMap * scaleMap;
    if (EntityManager::GetWorld().valid(identity.parent)) {
        transform.localToWorldMat = EntityManager::GetWorld().get<TransformComponent>(identity.parent).localToWorldMat * transform.localMat;
    }
    else
    {
        transform.localToWorldMat = transform.localMat;
    }
    //transform.localMat.Invert();

    // mesh
    MeshInstanceComponent* mesh = EntityManager::GetWorld().try_get<MeshInstanceComponent>(e);
    if (mesh) {
        dummys.resize(dummys.size() + 1);
        
        InstanceData& dummy = dummys.back();
        if (identity.instanceGPUID == INVALID_RENDER_ID) {
            identity.instanceGPUID = GetFreeGPUInstance();
        }
        if (identity.instanceGPUID == INVALID_RENDER_ID) {
            //EntityManager::GetWorld().destroy(e);
            EntityManager::GetWorld().remove<Dirty>(e);
            dummys.pop_back();
            return;
        }

        dummy.transform = transform.localToWorldMat;
        dummy.entityID = (uint32_t)e;
        dummy.meshIndex = rS->renderIds[mesh->modelID];
        dummy.materialIndex = 0;//rS->renderIds[mesh->materialID];


        // parent
        dummy.parent = INVALID_RENDER_ID;
        // upload
        nri::BufferUploadDesc bufferData[] =
        {
            {&dummy, sizeof(InstanceData), m_Buffers[INSTANCE_BUFFER], sizeof(InstanceData) * identity.instanceGPUID,  {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::FRAGMENT_SHADER | nri::StageBits::COMPUTE_SHADER}},
        };

        descs.push_back({ dummys.data() + dummys.size() - 1, sizeof(InstanceData), m_Buffers[INSTANCE_BUFFER], sizeof(InstanceData) * identity.instanceGPUID,  {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::FRAGMENT_SHADER | nri::StageBits::COMPUTE_SHADER} });
        //NRI.UploadData(*m_CommandQueue, nullptr, 0, bufferData, helper::GetCountOf(bufferData));
    }

    EntityManager::GetWorld().remove<Dirty>(e);
    for (auto& child : identity.childs)
    {
        UpdateEntityTransform(child, EntityManager::GetWorld().get<TransformComponent>(child), EntityManager::GetWorld().get<IdentityComponent>(child));
    }
}


bool CmpInstance(InstanceData& a, InstanceData& b) {
    return (a.materialIndex < b.materialIndex);
}

uint32_t Sample::PrepareEntities()
{
    float timeBef = glfwGetTime();

    descs.clear();
    dummys.clear();
    //float uploadTime = 0;

    float iterTime = glfwGetTime();
    for (auto&& [e, t, id] : EntityManager::GetWorld().group<TransformComponent, IdentityComponent, Dirty>().each())
    {
        UpdateEntityTransform(e, t, id);
    }
    iterTime = glfwGetTime() - iterTime;
    //Log::Message("Renderer", "Iterating Took " + std::to_string(iterTime * 1000) + "ms");

    if (descs.size() > 0) {
        float uploadTime = glfwGetTime();
        NRI.UploadData(*m_CommandQueue, nullptr, 0, descs.data(), helper::GetCountOf(descs));
        uploadTime = glfwGetTime() - uploadTime;
        //Log::Message("Renderer", "Uploading Took " + std::to_string(uploadTime * 1000) + "ms");
    }

    float dt = glfwGetTime() - timeBef;
    //Log::Message("Renderer", "Preparing Took " + std::to_string(dt * 1000) + "ms");
    
    return MAX_TRANSFORMS;
}

void Sample::ProzessAddRrmRequests()
{
    RenderScene* scene = SceneManager::GetRenderScene();
    if (!scene) {
        return;
    }
    if (scene->rel) {
        scene->rel = false;
        Reload();
        return;
    }

    if (scene->relMeshes) {
        scene->relMeshes = false;
        NRI.WaitForIdle(*m_CommandQueue);
        ReloadMeshes();
    }

    if (scene->relMaterials) {
        NRI.WaitForIdle(*m_CommandQueue);
        scene->relMaterials = false;
        ReloadMaterials();
    }

    if (scene->relTextures) {
        NRI.WaitForIdle(*m_CommandQueue);
        scene->relTextures = false;
        ReloadTextures();
    }
}

void Sample::ReloadReq()
{
    RenderScene* scene = SceneManager::GetRenderScene();
    if (!scene) {
        return;
    }
    scene->rel = true;
}

void Sample::ReloadMeshesReq()
{
    RenderScene* scene = SceneManager::GetRenderScene();
    if (!scene) {
        return;
    }

    scene->relMeshes = true;
}

void Sample::ReloadTexturesReq()
{
    RenderScene* scene = SceneManager::GetRenderScene();
    if (!scene) {
        return;
    }
    scene->relTextures = true;
}

void Sample::ReloadMaterialsReq()
{
    RenderScene* scene = SceneManager::GetRenderScene();
    if (!scene) {
        return;
    }
    scene->relMaterials = true;
}

void Sample::Reload()
{
    return;
    RenderScene* rScene = SceneManager::GetRenderScene();
    if (!rScene) {
        return;
    }
    NRI.WaitForIdle(*m_CommandQueue);
    rScene->Load();
    ReloadMeshes();
}

void Sample::ReloadMeshes()
{
    return;
    RenderScene* rScene = SceneManager::GetRenderScene();
    if (!rScene) {
        return;
    }
    nri::BufferDesc bufferDesc = {};

    // delet old
    NRI.DestroyDescriptor(*m_Descriptors[INDEX_BUFFER]);
    NRI.DestroyBuffer(*m_Buffers[INDEX_BUFFER]);


    NRI.DestroyDescriptor(*m_Descriptors[VERTEX_BUFFER]);
    NRI.DestroyBuffer(*m_Buffers[VERTEX_BUFFER]);

    NRI.DestroyDescriptor(*m_Descriptors[MESH_BUFFER]);
    NRI.DestroyBuffer(*m_Buffers[MESH_BUFFER]);


    NRI.FreeMemory(*m_MemoryAllocations[2]);
    m_MemoryAllocations.pop_back();

    // buffer
    // 
    // INDEX_BUFFER
    bufferDesc.size = helper::GetByteSizeOf(rScene->indicesCPU);
    bufferDesc.usageMask = nri::BufferUsageBits::INDEX_BUFFER;
    NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, m_Buffers[INDEX_BUFFER]));


    // VERTEX_BUFFER
    bufferDesc.size = helper::GetByteSizeOf(rScene->verticesCPU);
    bufferDesc.usageMask = nri::BufferUsageBits::VERTEX_BUFFER;
    NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, m_Buffers[VERTEX_BUFFER]));

    // MESH_BUFFER
    bufferDesc.size = helper::GetByteSizeOf(rScene->meshesCPU);
    bufferDesc.structureStride = sizeof(MeshData);
    bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
    NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, m_Buffers[MESH_BUFFER]));

    // memory
    nri::ResourceGroupDesc resourceGroupDesc = {};
    resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
    resourceGroupDesc.bufferNum = (uint32_t)SceneBuffers::MAX_NUM - 2;
    resourceGroupDesc.buffers = &m_Buffers[INDEX_BUFFER];
    resourceGroupDesc.textureNum = (uint32_t)m_Textures.size();
    resourceGroupDesc.textures = m_Textures.data();

    uint32_t baseAllocation = m_MemoryAllocations.size();
    uint32_t allocationNum = NRI.CalculateAllocationNumber(*m_Device, resourceGroupDesc);
    m_MemoryAllocations.resize(baseAllocation + allocationNum, nullptr);
    NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*m_Device, resourceGroupDesc, m_MemoryAllocations.data() + baseAllocation));


    // descriptor
    // 
    // Mesh buffer
    nri::BufferViewDesc bufferViewDesc = {};
    bufferViewDesc.buffer = m_Buffers[MESH_BUFFER];
    bufferViewDesc.size = helper::GetByteSizeOf(rScene->meshesCPU);
    NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, m_recourceDescs[1]));

    for (size_t i = 0; i < BUFFERED_FRAME_MAX_NUM; i++)
    {
        nri::DescriptorRangeUpdateDesc rangeUpdateDescs[1] = {};
        rangeUpdateDescs[0].descriptorNum = BUFFER_COUNT;
        rangeUpdateDescs[0].descriptors = m_recourceDescs;
        rangeUpdateDescs[0].offsetInRange = 2;

        NRI.UpdateDescriptorRanges(*m_DescriptorSets[i], 2, 1, rangeUpdateDescs);
    }

    nri::DescriptorRangeUpdateDesc rangeUpdateDescs[1] = {};
    rangeUpdateDescs[0].descriptorNum = BUFFER_COUNT;
    rangeUpdateDescs[0].descriptors = m_recourceDescs;
    NRI.UpdateDescriptorRanges(*m_DescriptorSets[BUFFERED_FRAME_MAX_NUM + 1], 1, 1, rangeUpdateDescs);


    nri::BufferUploadDesc bufferData[] =
    {
        {rScene->meshesCPU.data(), rScene->meshesCPU.size() * sizeof(MeshData), m_Buffers[MESH_BUFFER], 0,  {nri::AccessBits::SHADER_RESOURCE, nri::StageBits::FRAGMENT_SHADER | nri::StageBits::COMPUTE_SHADER}},
        {rScene->verticesCPU.data(), helper::GetByteSizeOf(rScene->verticesCPU), m_Buffers[VERTEX_BUFFER], 0, {nri::AccessBits::VERTEX_BUFFER}},
        {rScene->indicesCPU.data(), helper::GetByteSizeOf(rScene->verticesCPU), m_Buffers[INDEX_BUFFER], 0, {nri::AccessBits::INDEX_BUFFER}},
    };

    NRI_ABORT_ON_FAILURE(NRI.UploadData(*m_CommandQueue, nullptr, 0, bufferData, helper::GetCountOf(bufferData)));
}

void Sample::ReloadTextures()                                       /// todo                todo                            todo
{

}

void Sample::ReloadMaterials()                                      /// todo                todo                            todo
{

}

RenderID Sample::GetFreeGPUInstance()
{
    static RenderID last = -1;

    last++;
    if (last >= MAX_TRANSFORMS) {
        last = INVALID_RENDER_ID;
        if (m_FreeInstances.size() > 0) {
            RenderID id = m_FreeInstances[m_FreeInstances.size()-1];
            m_FreeInstances.pop_back();
            return id;
        }
    }
    return last;
}

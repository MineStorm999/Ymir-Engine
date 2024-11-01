#include "OdPhysics.h"
#include "Assets/AssetManager.h"

#include "Log/Log.h"


OdPhysics::OdPhysics(NRIInterface& NRI, nri::Device* device)
{
    Log::Message("Physics", "Init");
    {
        nri::DescriptorRangeDesc descriptorRange[2] = {};
        descriptorRange[0] = { 0, 4, nri::DescriptorType::STRUCTURED_BUFFER, nri::StageBits::COMPUTE_SHADER };
        descriptorRange[1] = { 0, 3, nri::DescriptorType::STORAGE_BUFFER, nri::StageBits::COMPUTE_SHADER };

        nri::DescriptorSetDesc descriptorSetDescs[] =
        {
            {0, descriptorRange, helper::GetCountOf(descriptorRange)},
        };

        nri::PipelineLayoutDesc pipelineLayoutDesc = {};
        pipelineLayoutDesc.descriptorSetNum = helper::GetCountOf(descriptorSetDescs);
        pipelineLayoutDesc.descriptorSets = descriptorSetDescs;
        pipelineLayoutDesc.shaderStages = nri::StageBits::COMPUTE_SHADER;

        NRI_ABORT_ON_FAILURE(NRI.CreatePipelineLayout(*device, pipelineLayoutDesc, m_PhysicsPipelineLayout));
    }

    const nri::DeviceDesc& deviceDesc = NRI.GetDeviceDesc(*device);
    utils::ShaderCodeStorage shaderCodeStorage;
    {
        nri::ComputePipelineDesc computePipelineDesc = {};
        computePipelineDesc.pipelineLayout = m_PhysicsPipelineLayout;
        computePipelineDesc.shader = utils::LoadShader(deviceDesc.graphicsAPI, "OdPhysics.cs", shaderCodeStorage);
        NRI_ABORT_ON_FAILURE(NRI.CreateComputePipeline(*device, computePipelineDesc, m_PhysicsPipeline));
    }

    Log::Message("Physics", "Init0");
    // buffer
    {
        // index buffer
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = max(sizeof(float3), helper::GetByteSizeOf(indexes));
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(uint32_t);
        nri::Buffer* buffer;
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);
        

        // vertex buffer
        bufferDesc.size = max(sizeof(float3), helper::GetByteSizeOf(vertices));
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(float3);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Od Collider buffer
        bufferDesc.size = max(sizeof(float3), helper::GetByteSizeOf(colls));
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(OdCollider);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Od Cluster buffer
        bufferDesc.size = max(sizeof(float3), helper::GetByteSizeOf(cluster));
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(OdCluster);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Od RigidBody buffer
        bufferDesc.size = sizeof(OdRigidBody) * MAX_RIGID_BODIES;
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(OdRigidBody);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // READBACKS   READBACKS   READBACKS

        // Return Positions
        bufferDesc.size = sizeof(PositionRB) * MAX_READBACKS;
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
        bufferDesc.structureStride = sizeof(PositionRB);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Return Rigidbodies
        bufferDesc.size = sizeof(RigidBodyRB) * MAX_READBACKS;
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
        bufferDesc.structureStride = sizeof(RigidBodyRB);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Return Collision Events
        bufferDesc.size = sizeof(CollisionEvent) * MAX_READBACKS;
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
        bufferDesc.structureStride = sizeof(CollisionEvent);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Readback Buffer
        bufferDesc.size = sizeof(CollisionEvent) * MAX_READBACKS;
        bufferDesc.usageMask = nri::BufferUsageBits::NONE;
        bufferDesc.structureStride = sizeof(CollisionEvent);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);
    }

    Log::Message("Physics", "Init1");
    // Memory
    {
        nri::ResourceGroupDesc resourceGroupDesc = {};
        resourceGroupDesc.memoryLocation = nri::MemoryLocation::HOST_READBACK;
        resourceGroupDesc.bufferNum = 1;
        resourceGroupDesc.buffers = &m_buffer[OdBufferOrder::READBACK];
        size_t baseAllocation = m_memory.size();
        m_memory.resize(baseAllocation + 1, nullptr);
        NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*device, resourceGroupDesc, m_memory.data() + baseAllocation));

        resourceGroupDesc.memoryLocation = nri::MemoryLocation::DEVICE;
        resourceGroupDesc.bufferNum = 8;
        resourceGroupDesc.buffers = &m_buffer[OdBufferOrder::COLLIDER_DATA_INDEX];
        size_t baseAllocation = m_memory.size();
        m_memory.resize(baseAllocation + 1, nullptr);
        NRI_ABORT_ON_FAILURE(NRI.AllocateAndBindMemory(*device, resourceGroupDesc, m_memory.data() + baseAllocation));
    }

    // descriptors
    Log::Message("Physics", "Init1");
    {
        nri::BufferViewDesc bufferViewDesc = {};
        bufferViewDesc.viewType = nri::BufferViewType::SHADER_RESOURCE;
        bufferViewDesc.offset = 0;

        // index buffer 
        bufferViewDesc.buffer = m_buffer[OdBufferOrder::COLLIDER_DATA_INDEX];
        bufferViewDesc.size = max(sizeof(uint32_t), helper::GetByteSizeOf(indexes));
        NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, m_rescourceDescs[0]));
        m_Descriptors.push_back(m_rescourceDescs[0]);

        // vertex buffer 
        bufferViewDesc.buffer = m_buffer[OdBufferOrder::COLLIDER_DATA_VERTEX];
        bufferViewDesc.size = max(sizeof(float3), helper::GetByteSizeOf(vertices));
        NRI_ABORT_ON_FAILURE(NRI.CreateBufferView(bufferViewDesc, m_rescourceDescs[0]));
        m_Descriptors.push_back(m_rescourceDescs[0]);


    }
}

OdPhysics::~OdPhysics()
{
}

void OdPhysics::Update(float dt)
{
}

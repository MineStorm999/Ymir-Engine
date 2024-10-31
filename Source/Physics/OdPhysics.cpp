#include "OdPhysics.h"

#include "Assets/AssetManager.h"


OdPhysics::OdPhysics(NRIInterface& NRI, nri::Device* device)
{
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

    // buffer
    {
        // index buffer
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = helper::GetByteSizeOf(indexes);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(uint32_t);
        nri::Buffer* buffer;
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);
        std::cout << "Create Buffers1\n";

        // vertex buffer
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = helper::GetByteSizeOf(vertices);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(float3);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Od Collider buffer
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = helper::GetByteSizeOf(colls);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(OdCollider);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Od Cluster buffer
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = helper::GetByteSizeOf(cluster);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(OdCluster);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Od Cluster buffer
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = sizeof(OdRigidBody) * MAX_RIGID_BODIES;
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        bufferDesc.structureStride = sizeof(OdRigidBody);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);


        // Return Positions
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = sizeof(PositionRB) * MAX_READBACKS;
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
        bufferDesc.structureStride = sizeof(PositionRB);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);

        // Return Rigidbodies
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = sizeof(RigidBodyRB) * MAX_READBACKS;
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE_STORAGE;
        bufferDesc.structureStride = sizeof(RigidBodyRB);
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*device, bufferDesc, buffer));
        m_buffer.push_back(buffer);
    }
}

OdPhysics::~OdPhysics()
{
}

void OdPhysics::Update(float dt)
{
}

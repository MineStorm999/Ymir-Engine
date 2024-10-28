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
        // 
        nri::BufferDesc bufferDesc = {};
        bufferDesc.size = helper::GetByteSizeOf(scene->indicesCPU);
        bufferDesc.usageMask = nri::BufferUsageBits::SHADER_RESOURCE;
        nri::Buffer* buffer;

        std::cout << "Create Buffers1\n";
        NRI_ABORT_ON_FAILURE(NRI.CreateBuffer(*m_Device, bufferDesc, buffer));
        m_Buffers.push_back(buffer);
    }
}

OdPhysics::~OdPhysics()
{
}

void OdPhysics::Update(float dt)
{
}


#include "NRIFramework.h"



#include <array>


constexpr uint32_t GLOBAL_DESCRIPTOR_SET = 0;
constexpr uint32_t MATERIAL_DESCRIPTOR_SET = 1;
constexpr float CLEAR_DEPTH = 0.0f;
constexpr uint32_t TEXTURES_PER_MATERIAL = 4;
constexpr uint32_t BUFFER_COUNT = 3;



enum SceneBuffers
{
    // HOST_UPLOAD
    CONSTANT_BUFFER,

    // READBACK
    READBACK_BUFFER,

    // DEVICE
    INDEX_BUFFER,
    VERTEX_BUFFER,
    MATERIAL_BUFFER,
    MESH_BUFFER,
    INSTANCE_BUFFER,
    INDIRECT_BUFFER,
    INDIRECT_COUNT_BUFFER,

    MAX_NUM
};

struct NRIInterface
    : public nri::CoreInterface
    , public nri::HelperInterface
    , public nri::StreamerInterface
    , public nri::SwapChainInterface
{};

struct Frame1
{
    nri::CommandAllocator* commandAllocator;
    nri::CommandBuffer* commandBuffer;
    uint32_t globalConstantBufferViewOffsets;
};

class Sample : public SampleBase
{
public:

    Sample()
    {}

    ~Sample();

    inline uint32_t GetDrawIndexedCommandSize()
    {
        return NRI.GetDeviceDesc(*m_Device).isDrawParametersEmulationEnabled ? sizeof(nri::DrawIndexedBaseDesc) : sizeof(nri::DrawIndexedDesc);
    }

    bool Initialize(nri::GraphicsAPI graphicsAPI) override;
    void PrepareFrame(uint32_t frameIndex) override;
    void RenderFrame(uint32_t frameIndex) override;

    
    void PrepareEntities();

private:

    NRIInterface NRI = {};
    nri::Device* m_Device = nullptr;
    nri::Streamer* m_Streamer = nullptr;
    nri::SwapChain* m_SwapChain = nullptr;
    nri::CommandQueue* m_CommandQueue = nullptr;
    nri::Fence* m_FrameFence = nullptr;
    nri::DescriptorPool* m_DescriptorPool = nullptr;
    nri::PipelineLayout* m_PipelineLayout = nullptr;
    nri::PipelineLayout* m_ComputePipelineLayout = nullptr;
    nri::Descriptor* m_DepthAttachment = nullptr;
    nri::Descriptor* m_IndirectBufferCountStorageAttachement = nullptr;
    nri::Descriptor* m_IndirectBufferStorageAttachement = nullptr;
    nri::QueryPool* m_QueryPool = nullptr;
    nri::Pipeline* m_Pipeline = nullptr;
    nri::Pipeline* m_ComputePipeline = nullptr;

    std::array<Frame1, BUFFERED_FRAME_MAX_NUM> m_Frames = {};
    std::vector<BackBuffer> m_SwapChainBuffers;
    std::vector<nri::DescriptorSet*> m_DescriptorSets;
    std::vector<nri::Texture*> m_Textures;
    std::vector<nri::Buffer*> m_Buffers;
    std::vector<nri::Memory*> m_MemoryAllocations;
    std::vector<nri::Descriptor*> m_Descriptors;



    bool m_UseGPUDrawGeneration = true;
    nri::Format m_DepthFormat = nri::Format::UNKNOWN;

    utils::Scene m_Scene;
};
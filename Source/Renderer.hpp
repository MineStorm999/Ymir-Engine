#include "NRIFramework.h"

#include "NRICompatibility.hlsli"
#include "../Shaders/cpu_gpu_shared.h"
#include "Rendering/VirtualGeometry/VirtualGeometryStreamer.h"

constexpr uint32_t GLOBAL_DESCRIPTOR_SET = 0;
constexpr uint32_t MATERIAL_DESCRIPTOR_SET = 1;
constexpr float CLEAR_DEPTH = 0.0f;
constexpr uint32_t TEXTURES_PER_MATERIAL = 4;
constexpr uint32_t BUFFER_COUNT = 3;

struct NRIInterface
    : public nri::CoreInterface
    , public nri::HelperInterface
    , public nri::StreamerInterface
    , public nri::SwapChainInterface
{};

struct ResidentBuffers {
    nri::Buffer* ModelBuffer;
    nri::Buffer* MeshletDescBuffer;
    //nri::Buffer* MeshletChildIDBuffer;
};

struct VisibilityBuffer
{
    nri::Texture* depth;
    nri::Texture* data;
};

struct BufferSet {
    nri::Buffer* RenderDataBuff;
    nri::Buffer* RenderCMDBuff;
};

struct Frame
{
    nri::CommandAllocator* commandAllocator;
    nri::CommandBuffer* commandBuffer;
    uint32_t globalConstantBufferViewOffsets;
};

class Renderer : public SampleBase
{
public:
    bool Initialize(nri::GraphicsAPI graphicsAPI) override;
    void PrepareFrame(uint32_t frameIndex) override;
    void RenderFrame(uint32_t frameIndex) override;

    void ReloadRenderer();

    inline uint32_t GetDrawIndexedCommandSize()
    {
        return NRI.GetDeviceDesc(*m_Device).isDrawParametersEmulationEnabled ? sizeof(nri::DrawIndexedBaseDesc) : sizeof(nri::DrawIndexedDesc);
    }
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


    // pipelines
    nri::Pipeline* m_cachedRenderPass = nullptr;
    nri::Pipeline* m_cullingPass = nullptr;
    nri::Pipeline* m_visibilityBuffRenderPass = nullptr;
    nri::Pipeline* m_materialRenderPass = nullptr;
    nri::Pipeline* m_mergePass = nullptr;


    std::array<Frame, BUFFERED_FRAME_MAX_NUM> m_Frames = {};
    std::vector<BackBuffer> m_SwapChainBuffers;
    std::vector<nri::DescriptorSet*> m_DescriptorSets;
    std::vector<nri::Texture*> m_Textures;
    std::vector<nri::Buffer*> m_Buffers;
    std::vector<nri::Memory*> m_MemoryAllocations;
    std::vector<nri::Descriptor*> m_Descriptors;



    
    nri::Format m_DepthFormat = nri::Format::UNKNOWN;

private:
    VirtualGeometryStreamer* m_vGeomStreamer;

    BufferSet firstBuff;
    BufferSet secondBuff;

    ResidentBuffers residentBuffer;

    VisibilityBuffer visBuff;
};
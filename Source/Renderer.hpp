#include "NRIFramework.h"

#include "NRICompatibility.hlsli"
#include "../Shaders/cpu_gpu_shared.h"
#include "Rendering/VirtualGeometry/VirtualGeometryStreamer.h"

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

struct BufferSet {
    nri::Buffer* RenderCMDBuff;
    nri::Buffer* RenderDataBuff;
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
    nri::Pipeline* m_Pipeline = nullptr;
    nri::Pipeline* m_ComputePipeline = nullptr;

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
};
#include "NRIFramework.h"

#include "NRICompatibility.hlsli"
#include "../Shaders/YMIR_cpu_gpu_shared.h"
#include "Rendering/VirtualGeometry/VirtualGeometryStreamer.h"

#include "ECS/ECSManager.h"

constexpr uint32_t GLOBAL_DESCRIPTOR_SET = 0;
constexpr uint32_t MATERIAL_DESCRIPTOR_SET = 1;
constexpr float CLEAR_DEPTH = 0.0f;
constexpr uint32_t TEXTURES_PER_MATERIAL = 4;
constexpr uint32_t BUFFER_COUNT = 6;

struct NRIInterface
    : public nri::CoreInterface
    , public nri::HelperInterface
    , public nri::StreamerInterface
    , public nri::SwapChainInterface
{};

enum BufferLocs {

    // resident buffs
    MODELDESC = 0,
    MESHLETDESC = 1,

    // buff0
    RENDERDATA0 = 2,
    RENDERDATA1 = 3,

    // buff1
    RENDERCMD0 = 4,
    RENDERCMD1 = 5,

    // read back
    READBACK = 6,

    // instance buffer
    INSTANCES = 7,
    IMSTANCESLAST = 8,

    CONSTANTBUFFER = 9
};



struct VisibilityBuffer
{
    nri::Texture* depth;
    nri::Texture* data;

    nri::Memory* depthMem;
    nri::Memory* dataMem;

    nri::Descriptor* depthDesc;
    nri::Descriptor* dataDesc;

    nri::Descriptor* sampler;
};

struct Frame
{

    nri::CommandAllocator* cmdAllocEvalStage;
    nri::CommandBuffer* cmdBuffEvalStage;

    nri::CommandAllocator* cmdAllocRenderStage;
    nri::CommandBuffer* cmdBuffRenderStage;

    uint32_t globalConstantBufferViewOffsets;
};

struct CachedCluster {
    uint32_t modelID, clusterID;
    uint32_t clusterOffSet;
};

class Renderer : public SampleBase
{
public:
    Renderer() {};
    ~Renderer();


    bool Initialize(nri::GraphicsAPI graphicsAPI) override;
    void PrepareFrame(uint32_t frameIndex) override;
    void RenderFrame(uint32_t frameIndex) override;

    void ReloadRenderer();

    void PopulateInstaceBuffer();
    void UploadInstaceBuffer();

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
    
    nri::Descriptor* m_DepthAttachment = nullptr;
    nri::Descriptor* m_IndirectBufferCountStorageAttachement = nullptr;
    nri::Descriptor* m_IndirectBufferStorageAttachement = nullptr;
    nri::QueryPool* m_QueryPool = nullptr;

    // pipeline layouts
    nri::PipelineLayout* m_cachedRenderLayout = nullptr;
    nri::PipelineLayout* m_cullingLayout = nullptr;
    nri::PipelineLayout* m_visibilityBuffLayout = nullptr;
    nri::PipelineLayout* m_materialLayout = nullptr;
    nri::PipelineLayout* m_mergeLayout = nullptr;

    // pipelines
    nri::Pipeline* m_cachedRenderPipeline = nullptr;
    nri::Pipeline* m_cullingPipeline = nullptr;
    nri::Pipeline* m_visibilityBuffPipeline = nullptr;
    nri::Pipeline* m_materialPipeline = nullptr;
    nri::Pipeline* m_mergePipeline = nullptr;
    
    std::vector<nri::DescriptorSet*> m_cachedDescriptorSets;
    std::vector<nri::DescriptorSet*> m_cullingDescriptorSets;
    std::vector<nri::DescriptorSet*> m_visBuffDescriptorSets;
    std::vector<nri::DescriptorSet*> m_materialDescriptorSets;
    std::vector<nri::DescriptorSet*> m_mergeDescriptorSets;
    

    std::array<Frame, BUFFERED_FRAME_MAX_NUM> m_Frames = {};
    std::vector<BackBuffer> m_SwapChainBuffers;
    
    std::vector<nri::Texture*> m_Textures;
    //std::vector<nri::Buffer*> m_Buffers;
    //std::vector<nri::Memory*> m_MemoryAllocations;
    //std::vector<nri::Descriptor*> m_Descriptors;



    
    nri::Format m_DepthFormat = nri::Format::UNKNOWN;

private:
    std::vector<nri::Buffer*> m_buffers;
    std::vector<nri::Memory*> m_memoryAllocations;
    std::vector<nri::Descriptor*> m_descriptors;
    
    
    VisibilityBuffer m_visBuff;

    
    bool useBuffer0 = true;

    uint32_t GetStructuredBuffCount() { return 6; };
    uint32_t GetRWStructuredBuffCount() { return 2; };

    
    // instance collection
private:
    void IterateChildren(entt::entity, float4x4 pMat);
    uint32_t instanceCount0 = 0;
    uint32_t instanceCount1 = 0;

    InstanceDesc* allRenderedEntites0;
    InstanceDesc* allRenderedEntites1;
    
    
    uint32_t* lastFrameEntitiesID;
    
    // streaming
private:
    VirtualGeometryStreamer* m_vGeomStreamer;

    std::vector<CachedCluster> cachedCluster;
    uint32_t size0, size1;
    void* VGBuffer0;
    void* VGBuffer1;

    void StreamGeom();
    CachedCluster nullCluster;
    CachedCluster& Contains(uint32_t MID, uint32_t CID);
};
// © 2021 NVIDIA Corporation

Declare_PartiallyFillFunctionTable_Functions(D3D12);

#pragma region[  Core  ]

static const DeviceDesc& NRI_CALL GetDeviceDesc(const Device& device) {
    return ((const DeviceD3D12&)device).GetDesc();
}

static const BufferDesc& NRI_CALL GetBufferDesc(const Buffer& buffer) {
    return ((const BufferD3D12&)buffer).GetDesc();
}

static const TextureDesc& NRI_CALL GetTextureDesc(const Texture& texture) {
    return ((const TextureD3D12&)texture).GetDesc();
}

static FormatSupportBits NRI_CALL GetFormatSupport(const Device& device, Format format) {
    return ((const DeviceD3D12&)device).GetFormatSupport(format);
}

static void NRI_CALL GetBufferMemoryDesc(const Device& device, const BufferDesc& bufferDesc, MemoryLocation memoryLocation, MemoryDesc& memoryDesc) {
    D3D12_RESOURCE_DESC desc = {};
    GetResourceDesc(&desc, bufferDesc);

    ((const DeviceD3D12&)device).GetMemoryDesc(memoryLocation, desc, memoryDesc);
}

static void NRI_CALL GetTextureMemoryDesc(const Device& device, const TextureDesc& textureDesc, MemoryLocation memoryLocation, MemoryDesc& memoryDesc) {
    D3D12_RESOURCE_DESC desc = {};
    GetResourceDesc(&desc, textureDesc);

    ((const DeviceD3D12&)device).GetMemoryDesc(memoryLocation, desc, memoryDesc);
}

static Result NRI_CALL GetCommandQueue(Device& device, CommandQueueType commandQueueType, CommandQueue*& commandQueue) {
    return ((DeviceD3D12&)device).GetCommandQueue(commandQueueType, commandQueue);
}

static Result NRI_CALL CreateCommandAllocator(const CommandQueue& commandQueue, CommandAllocator*& commandAllocator) {
    DeviceD3D12& device = ((CommandQueueD3D12&)commandQueue).GetDevice();
    return device.CreateCommandAllocator(commandQueue, commandAllocator);
}

static Result NRI_CALL CreateDescriptorPool(Device& device, const DescriptorPoolDesc& descriptorPoolDesc, DescriptorPool*& descriptorPool) {
    return ((DeviceD3D12&)device).CreateDescriptorPool(descriptorPoolDesc, descriptorPool);
}

static Result NRI_CALL CreateBuffer(Device& device, const BufferDesc& bufferDesc, Buffer*& buffer) {
    return ((DeviceD3D12&)device).CreateBuffer(bufferDesc, buffer);
}

static Result NRI_CALL CreateTexture(Device& device, const TextureDesc& textureDesc, Texture*& texture) {
    return ((DeviceD3D12&)device).CreateTexture(textureDesc, texture);
}

static Result NRI_CALL CreateBufferView(const BufferViewDesc& bufferViewDesc, Descriptor*& bufferView) {
    DeviceD3D12& device = ((const BufferD3D12*)bufferViewDesc.buffer)->GetDevice();
    return device.CreateDescriptor(bufferViewDesc, bufferView);
}

static Result NRI_CALL CreateTexture1DView(const Texture1DViewDesc& textureViewDesc, Descriptor*& textureView) {
    DeviceD3D12& device = ((const TextureD3D12*)textureViewDesc.texture)->GetDevice();
    return device.CreateDescriptor(textureViewDesc, textureView);
}

static Result NRI_CALL CreateTexture2DView(const Texture2DViewDesc& textureViewDesc, Descriptor*& textureView) {
    DeviceD3D12& device = ((const TextureD3D12*)textureViewDesc.texture)->GetDevice();
    return device.CreateDescriptor(textureViewDesc, textureView);
}

static Result NRI_CALL CreateTexture3DView(const Texture3DViewDesc& textureViewDesc, Descriptor*& textureView) {
    DeviceD3D12& device = ((const TextureD3D12*)textureViewDesc.texture)->GetDevice();
    return device.CreateDescriptor(textureViewDesc, textureView);
}

static Result NRI_CALL CreateSampler(Device& device, const SamplerDesc& samplerDesc, Descriptor*& sampler) {
    return ((DeviceD3D12&)device).CreateDescriptor(samplerDesc, sampler);
}

static Result NRI_CALL CreatePipelineLayout(Device& device, const PipelineLayoutDesc& pipelineLayoutDesc, PipelineLayout*& pipelineLayout) {
    return ((DeviceD3D12&)device).CreatePipelineLayout(pipelineLayoutDesc, pipelineLayout);
}

static Result NRI_CALL CreateGraphicsPipeline(Device& device, const GraphicsPipelineDesc& graphicsPipelineDesc, Pipeline*& pipeline) {
    return ((DeviceD3D12&)device).CreatePipeline(graphicsPipelineDesc, pipeline);
}

static Result NRI_CALL CreateComputePipeline(Device& device, const ComputePipelineDesc& computePipelineDesc, Pipeline*& pipeline) {
    return ((DeviceD3D12&)device).CreatePipeline(computePipelineDesc, pipeline);
}

static Result NRI_CALL CreateFence(Device& device, uint64_t initialValue, Fence*& fence) {
    return ((DeviceD3D12&)device).CreateFence(initialValue, fence);
}

static Result NRI_CALL CreateQueryPool(Device& device, const QueryPoolDesc& queryPoolDesc, QueryPool*& queryPool) {
    return ((DeviceD3D12&)device).CreateQueryPool(queryPoolDesc, queryPool);
}

static void NRI_CALL DestroyCommandAllocator(CommandAllocator& commandAllocator) {
    if (!(&commandAllocator))
        return;

    DeviceD3D12& device = ((CommandAllocatorD3D12&)commandAllocator).GetDevice();
    device.DestroyCommandAllocator(commandAllocator);
}

static void NRI_CALL DestroyDescriptorPool(DescriptorPool& descriptorPool) {
    if (!(&descriptorPool))
        return;

    DeviceD3D12& device = ((DescriptorPoolD3D12&)descriptorPool).GetDevice();
    device.DestroyDescriptorPool(descriptorPool);
}

static void NRI_CALL DestroyBuffer(Buffer& buffer) {
    if (!(&buffer))
        return;

    DeviceD3D12& device = ((BufferD3D12&)buffer).GetDevice();
    device.DestroyBuffer(buffer);
}

static void NRI_CALL DestroyTexture(Texture& texture) {
    if (!(&texture))
        return;

    DeviceD3D12& device = ((TextureD3D12&)texture).GetDevice();
    device.DestroyTexture(texture);
}

static void NRI_CALL DestroyDescriptor(Descriptor& descriptor) {
    if (!(&descriptor))
        return;

    DeviceD3D12& device = ((DescriptorD3D12&)descriptor).GetDevice();
    device.DestroyDescriptor(descriptor);
}

static void NRI_CALL DestroyPipelineLayout(PipelineLayout& pipelineLayout) {
    if (!(&pipelineLayout))
        return;

    DeviceD3D12& device = ((PipelineLayoutD3D12&)pipelineLayout).GetDevice();
    device.DestroyPipelineLayout(pipelineLayout);
}

static void NRI_CALL DestroyPipeline(Pipeline& pipeline) {
    if (!(&pipeline))
        return;

    DeviceD3D12& device = ((PipelineD3D12&)pipeline).GetDevice();
    device.DestroyPipeline(pipeline);
}

static void NRI_CALL DestroyFence(Fence& fence) {
    if (!(&fence))
        return;

    DeviceD3D12& device = ((FenceD3D12&)fence).GetDevice();
    device.DestroyFence(fence);
}

static void NRI_CALL DestroyQueryPool(QueryPool& queryPool) {
    if (!(&queryPool))
        return;

    DeviceD3D12& device = ((QueryPoolD3D12&)queryPool).GetDevice();
    device.DestroyQueryPool(queryPool);
}

static Result NRI_CALL AllocateMemory(Device& device, MemoryType memoryType, uint64_t size, Memory*& memory) {
    return ((DeviceD3D12&)device).AllocateMemory(memoryType, size, memory);
}

static Result NRI_CALL BindBufferMemory(Device& device, const BufferMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) {
    return ((DeviceD3D12&)device).BindBufferMemory(memoryBindingDescs, memoryBindingDescNum);
}

static Result NRI_CALL BindTextureMemory(Device& device, const TextureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) {
    return ((DeviceD3D12&)device).BindTextureMemory(memoryBindingDescs, memoryBindingDescNum);
}

static void NRI_CALL FreeMemory(Memory& memory) {
    if (!(&memory))
        return;

    DeviceD3D12& device = ((MemoryD3D12&)memory).GetDevice();
    device.FreeMemory(memory);
}

static void NRI_CALL SetDeviceDebugName(Device& device, const char* name) {
    ((DeviceD3D12&)device).SetDebugName(name);
}

static void NRI_CALL SetPipelineDebugName(Pipeline& pipeline, const char* name) {
    ((PipelineD3D12&)pipeline).SetDebugName(name);
}

static void NRI_CALL SetPipelineLayoutDebugName(PipelineLayout& pipelineLayout, const char* name) {
    ((PipelineLayoutD3D12&)pipelineLayout).SetDebugName(name);
}

static void NRI_CALL SetMemoryDebugName(Memory& memory, const char* name) {
    ((MemoryD3D12&)memory).SetDebugName(name);
}

static void* NRI_CALL GetDeviceNativeObject(const Device& device) {
    if (!(&device))
        return nullptr;

    return ((DeviceD3D12&)device).GetNativeObject();
}

Result DeviceD3D12::FillFunctionTable(CoreInterface& coreInterface) const {
    coreInterface = {};
    coreInterface.GetDeviceDesc = ::GetDeviceDesc;
    coreInterface.GetBufferDesc = ::GetBufferDesc;
    coreInterface.GetTextureDesc = ::GetTextureDesc;
    coreInterface.GetFormatSupport = ::GetFormatSupport;
    coreInterface.GetBufferMemoryDesc = ::GetBufferMemoryDesc;
    coreInterface.GetTextureMemoryDesc = ::GetTextureMemoryDesc;
    coreInterface.GetCommandQueue = ::GetCommandQueue;
    coreInterface.CreateCommandAllocator = ::CreateCommandAllocator;
    coreInterface.CreateDescriptorPool = ::CreateDescriptorPool;
    coreInterface.CreateBuffer = ::CreateBuffer;
    coreInterface.CreateTexture = ::CreateTexture;
    coreInterface.CreateBufferView = ::CreateBufferView;
    coreInterface.CreateTexture1DView = ::CreateTexture1DView;
    coreInterface.CreateTexture2DView = ::CreateTexture2DView;
    coreInterface.CreateTexture3DView = ::CreateTexture3DView;
    coreInterface.CreateSampler = ::CreateSampler;
    coreInterface.CreatePipelineLayout = ::CreatePipelineLayout;
    coreInterface.CreateGraphicsPipeline = ::CreateGraphicsPipeline;
    coreInterface.CreateComputePipeline = ::CreateComputePipeline;
    coreInterface.CreateQueryPool = ::CreateQueryPool;
    coreInterface.CreateFence = ::CreateFence;
    coreInterface.DestroyCommandAllocator = ::DestroyCommandAllocator;
    coreInterface.DestroyDescriptorPool = ::DestroyDescriptorPool;
    coreInterface.DestroyBuffer = ::DestroyBuffer;
    coreInterface.DestroyTexture = ::DestroyTexture;
    coreInterface.DestroyDescriptor = ::DestroyDescriptor;
    coreInterface.DestroyPipelineLayout = ::DestroyPipelineLayout;
    coreInterface.DestroyPipeline = ::DestroyPipeline;
    coreInterface.DestroyFence = ::DestroyFence;
    coreInterface.DestroyQueryPool = ::DestroyQueryPool;
    coreInterface.AllocateMemory = ::AllocateMemory;
    coreInterface.BindBufferMemory = ::BindBufferMemory;
    coreInterface.BindTextureMemory = ::BindTextureMemory;
    coreInterface.FreeMemory = ::FreeMemory;
    coreInterface.SetDeviceDebugName = ::SetDeviceDebugName;
    coreInterface.SetPipelineDebugName = ::SetPipelineDebugName;
    coreInterface.SetPipelineLayoutDebugName = ::SetPipelineLayoutDebugName;
    coreInterface.SetMemoryDebugName = ::SetMemoryDebugName;
    coreInterface.GetDeviceNativeObject = ::GetDeviceNativeObject;

    Core_Buffer_PartiallyFillFunctionTableD3D12(coreInterface);
    Core_CommandAllocator_PartiallyFillFunctionTableD3D12(coreInterface);
    Core_CommandBuffer_PartiallyFillFunctionTableD3D12(coreInterface);
    Core_CommandQueue_PartiallyFillFunctionTableD3D12(coreInterface);
    Core_Descriptor_PartiallyFillFunctionTableD3D12(coreInterface);
    Core_DescriptorPool_PartiallyFillFunctionTableD3D12(coreInterface);
    Core_DescriptorSet_PartiallyFillFunctionTableD3D12(coreInterface);
    Core_Fence_PartiallyFillFunctionTableD3D12(coreInterface);
    Core_QueryPool_PartiallyFillFunctionTableD3D12(coreInterface);
    Core_Texture_PartiallyFillFunctionTableD3D12(coreInterface);

    return ValidateFunctionTable(coreInterface);
}

#pragma endregion

#pragma region[  Helper  ]

static uint32_t NRI_CALL CalculateAllocationNumber(const Device& device, const ResourceGroupDesc& resourceGroupDesc) {
    return ((DeviceD3D12&)device).CalculateAllocationNumber(resourceGroupDesc);
}

static Result NRI_CALL AllocateAndBindMemory(Device& device, const ResourceGroupDesc& resourceGroupDesc, Memory** allocations) {
    return ((DeviceD3D12&)device).AllocateAndBindMemory(resourceGroupDesc, allocations);
}

static Result NRI_CALL QueryVideoMemoryInfo(const Device& device, MemoryLocation memoryLocation, VideoMemoryInfo& videoMemoryInfo) {
    uint64_t luid = ((DeviceD3D12&)device).GetDesc().adapterDesc.luid;

    return QueryVideoMemoryInfoDXGI(luid, memoryLocation, videoMemoryInfo);
}

Result DeviceD3D12::FillFunctionTable(HelperInterface& helperInterface) const {
    helperInterface = {};
    helperInterface.CalculateAllocationNumber = ::CalculateAllocationNumber;
    helperInterface.AllocateAndBindMemory = ::AllocateAndBindMemory;
    helperInterface.QueryVideoMemoryInfo = ::QueryVideoMemoryInfo;

    Helper_CommandQueue_PartiallyFillFunctionTableD3D12(helperInterface);

    return ValidateFunctionTable(helperInterface);
}

#pragma endregion

#pragma region[  WrapperD3D12  ]

static Result NRI_CALL CreateCommandBuffer(Device& device, const CommandBufferD3D12Desc& commandBufferD3D12Desc, CommandBuffer*& commandBuffer) {
    return ((DeviceD3D12&)device).CreateCommandBuffer(commandBufferD3D12Desc, commandBuffer);
}

static Result NRI_CALL CreateDescriptorPool(Device& device, const DescriptorPoolD3D12Desc& descriptorPoolD3D12Desc, DescriptorPool*& descriptorPool) {
    return ((DeviceD3D12&)device).CreateDescriptorPool(descriptorPoolD3D12Desc, descriptorPool);
}

static Result NRI_CALL CreateBuffer(Device& device, const BufferD3D12Desc& bufferD3D12Desc, Buffer*& buffer) {
    return ((DeviceD3D12&)device).CreateBuffer(bufferD3D12Desc, buffer);
}

static Result NRI_CALL CreateTexture(Device& device, const TextureD3D12Desc& textureD3D12Desc, Texture*& texture) {
    return ((DeviceD3D12&)device).CreateTexture(textureD3D12Desc, texture);
}

static Result NRI_CALL CreateMemory(Device& device, const MemoryD3D12Desc& memoryD3D12Desc, Memory*& memory) {
    return ((DeviceD3D12&)device).CreateMemory(memoryD3D12Desc, memory);
}

static Result NRI_CALL CreateAccelerationStructure(Device& device, const AccelerationStructureD3D12Desc& accelerationStructureD3D12Desc, AccelerationStructure*& memory) {
    return ((DeviceD3D12&)device).CreateAccelerationStructure(accelerationStructureD3D12Desc, memory);
}

Result DeviceD3D12::FillFunctionTable(WrapperD3D12Interface& wrapperD3D12Interface) const {
    wrapperD3D12Interface = {};
    wrapperD3D12Interface.CreateCommandBufferD3D12 = ::CreateCommandBuffer;
    wrapperD3D12Interface.CreateDescriptorPoolD3D12 = ::CreateDescriptorPool;
    wrapperD3D12Interface.CreateBufferD3D12 = ::CreateBuffer;
    wrapperD3D12Interface.CreateTextureD3D12 = ::CreateTexture;
    wrapperD3D12Interface.CreateMemoryD3D12 = ::CreateMemory;
    wrapperD3D12Interface.CreateAccelerationStructureD3D12 = ::CreateAccelerationStructure;

    return ValidateFunctionTable(wrapperD3D12Interface);
}

#pragma endregion

#pragma region[  SwapChain  ]

static Result NRI_CALL CreateSwapChain(Device& device, const SwapChainDesc& swapChainDesc, SwapChain*& swapChain) {
    return ((DeviceD3D12&)device).CreateSwapChain(swapChainDesc, swapChain);
}

static void NRI_CALL DestroySwapChain(SwapChain& swapChain) {
    if (!(&swapChain))
        return;

    DeviceD3D12& device = ((SwapChainD3D12&)swapChain).GetDevice();
    device.DestroySwapChain(swapChain);
}

Result DeviceD3D12::FillFunctionTable(SwapChainInterface& swapChainInterface) const {
    swapChainInterface = {};
    if (!m_Desc.isSwapChainSupported)
        return Result::UNSUPPORTED;

    swapChainInterface.CreateSwapChain = ::CreateSwapChain;
    swapChainInterface.DestroySwapChain = ::DestroySwapChain;

    SwapChain_PartiallyFillFunctionTableD3D12(swapChainInterface);

    return ValidateFunctionTable(swapChainInterface);
}

#pragma endregion

#pragma region[  RayTracing  ]

static Result NRI_CALL CreateRayTracingPipeline(Device& device, const RayTracingPipelineDesc& rayTracingPipelineDesc, Pipeline*& pipeline) {
    return ((DeviceD3D12&)device).CreatePipeline(rayTracingPipelineDesc, pipeline);
}

static Result NRI_CALL CreateAccelerationStructure(Device& device, const AccelerationStructureDesc& accelerationStructureDesc, AccelerationStructure*& accelerationStructure) {
    return ((DeviceD3D12&)device).CreateAccelerationStructure(accelerationStructureDesc, accelerationStructure);
}

static void NRI_CALL DestroyAccelerationStructure(AccelerationStructure& accelerationStructure) {
    if (!(&accelerationStructure))
        return;

    DeviceD3D12& device = ((AccelerationStructureD3D12&)accelerationStructure).GetDevice();
    device.DestroyAccelerationStructure(accelerationStructure);
}

static Result NRI_CALL BindAccelerationStructureMemory(Device& device, const AccelerationStructureMemoryBindingDesc* memoryBindingDescs, uint32_t memoryBindingDescNum) {
    return ((DeviceD3D12&)device).BindAccelerationStructureMemory(memoryBindingDescs, memoryBindingDescNum);
}

void FillFunctionTablePipelineD3D12(RayTracingInterface& rayTracingInterface);

Result DeviceD3D12::FillFunctionTable(RayTracingInterface& rayTracingInterface) const {
    rayTracingInterface = {};
    if (!m_Desc.isRayTracingSupported)
        return Result::UNSUPPORTED;

    FillFunctionTablePipelineD3D12(rayTracingInterface);
    RayTracing_AccelerationStructure_PartiallyFillFunctionTableD3D12(rayTracingInterface);
    RayTracing_CommandBuffer_PartiallyFillFunctionTableD3D12(rayTracingInterface);

    rayTracingInterface.CreateRayTracingPipeline = ::CreateRayTracingPipeline;
    rayTracingInterface.CreateAccelerationStructure = ::CreateAccelerationStructure;
    rayTracingInterface.BindAccelerationStructureMemory = ::BindAccelerationStructureMemory;
    rayTracingInterface.DestroyAccelerationStructure = ::DestroyAccelerationStructure;

    return ValidateFunctionTable(rayTracingInterface);
}

#pragma endregion

#pragma region[  MeshShader  ]

Result DeviceD3D12::FillFunctionTable(MeshShaderInterface& meshShaderInterface) const {
    meshShaderInterface = {};
    if (!m_Desc.isMeshShaderSupported)
        return Result::UNSUPPORTED;

    MeshShader_CommandBuffer_PartiallyFillFunctionTableD3D12(meshShaderInterface);

    return ValidateFunctionTable(meshShaderInterface);
}

#pragma endregion

#pragma region[  LowLatency  ]

Result DeviceD3D12::FillFunctionTable(LowLatencyInterface& lowLatencyInterface) const {
    lowLatencyInterface = {};
    if (!m_Desc.isLowLatencySupported)
        return Result::UNSUPPORTED;

    LowLatency_CommandQueue_PartiallyFillFunctionTableD3D12(lowLatencyInterface);
    LowLatency_SwapChain_PartiallyFillFunctionTableD3D12(lowLatencyInterface);

    return ValidateFunctionTable(lowLatencyInterface);
}

#pragma endregion

#pragma region[  Streamer  ]

static Result CreateStreamer(Device& device, const StreamerDesc& streamerDesc, Streamer*& streamer) {
    DeviceD3D12& deviceD3D12 = (DeviceD3D12&)device;

    StreamerImpl* implementation = Allocate<StreamerImpl>(deviceD3D12.GetStdAllocator(), device, deviceD3D12.GetCoreInterface());
    Result res = implementation->Create(streamerDesc);

    if (res == Result::SUCCESS) {
        streamer = (Streamer*)implementation;
        return Result::SUCCESS;
    }

    Deallocate(deviceD3D12.GetStdAllocator(), implementation);

    return res;
}

static void DestroyStreamer(Streamer& streamer) {
    Deallocate(((DeviceBase&)((StreamerImpl&)streamer).GetDevice()).GetStdAllocator(), (StreamerImpl*)&streamer);
}

static Buffer* GetStreamerConstantBuffer(Streamer& streamer) {
    return ((StreamerImpl&)streamer).GetConstantBuffer();
}

static uint32_t UpdateStreamerConstantBuffer(Streamer& streamer, const void* data, uint32_t dataSize) {
    return ((StreamerImpl&)streamer).UpdateStreamerConstantBuffer(data, dataSize);
}

static uint64_t AddStreamerBufferUpdateRequest(Streamer& streamer, const BufferUpdateRequestDesc& bufferUpdateRequestDesc) {
    return ((StreamerImpl&)streamer).AddStreamerBufferUpdateRequest(bufferUpdateRequestDesc);
}

static uint64_t AddStreamerTextureUpdateRequest(Streamer& streamer, const TextureUpdateRequestDesc& textureUpdateRequestDesc) {
    return ((StreamerImpl&)streamer).AddStreamerTextureUpdateRequest(textureUpdateRequestDesc);
}

static Result CopyStreamerUpdateRequests(Streamer& streamer) {
    return ((StreamerImpl&)streamer).CopyStreamerUpdateRequests();
}

static Buffer* GetStreamerDynamicBuffer(Streamer& streamer) {
    return ((StreamerImpl&)streamer).GetDynamicBuffer();
}

static void CmdUploadStreamerUpdateRequests(CommandBuffer& commandBuffer, Streamer& streamer) {
    ((StreamerImpl&)streamer).CmdUploadStreamerUpdateRequests(commandBuffer);
}

Result DeviceD3D12::FillFunctionTable(StreamerInterface& streamerInterface) const {
    streamerInterface = {};
    streamerInterface.CreateStreamer = ::CreateStreamer;
    streamerInterface.DestroyStreamer = ::DestroyStreamer;
    streamerInterface.GetStreamerConstantBuffer = ::GetStreamerConstantBuffer;
    streamerInterface.UpdateStreamerConstantBuffer = ::UpdateStreamerConstantBuffer;
    streamerInterface.AddStreamerBufferUpdateRequest = ::AddStreamerBufferUpdateRequest;
    streamerInterface.AddStreamerTextureUpdateRequest = ::AddStreamerTextureUpdateRequest;
    streamerInterface.CopyStreamerUpdateRequests = ::CopyStreamerUpdateRequests;
    streamerInterface.GetStreamerDynamicBuffer = ::GetStreamerDynamicBuffer;
    streamerInterface.CmdUploadStreamerUpdateRequests = ::CmdUploadStreamerUpdateRequests;

    return ValidateFunctionTable(streamerInterface);
}

#pragma endregion

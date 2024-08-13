#include "SharedExternal.h"

#include "HelperDeviceMemoryAllocator.h"

using namespace nri;

HelperDeviceMemoryAllocator::MemoryHeap::MemoryHeap(MemoryType memoryType, const StdAllocator<uint8_t>& stdAllocator) :
    buffers(stdAllocator), bufferOffsets(stdAllocator), textures(stdAllocator), textureOffsets(stdAllocator), size(0), type(memoryType) {
}

HelperDeviceMemoryAllocator::HelperDeviceMemoryAllocator(const CoreInterface& NRI, Device& device) :
    m_NRI(NRI),
    m_Device(device),
    m_Heaps(((DeviceBase&)device).GetStdAllocator()),
    m_DedicatedBuffers(((DeviceBase&)device).GetStdAllocator()),
    m_DedicatedTextures(((DeviceBase&)device).GetStdAllocator()),
    m_BufferBindingDescs(((DeviceBase&)device).GetStdAllocator()),
    m_TextureBindingDescs(((DeviceBase&)device).GetStdAllocator()) {
}

uint32_t HelperDeviceMemoryAllocator::CalculateAllocationNumber(const ResourceGroupDesc& resourceGroupDesc) {
    GroupByMemoryType(resourceGroupDesc.memoryLocation, resourceGroupDesc);

    size_t allocationNum = m_Heaps.size() + m_DedicatedBuffers.size() + m_DedicatedTextures.size();

    return (uint32_t)allocationNum;
}

Result HelperDeviceMemoryAllocator::AllocateAndBindMemory(const ResourceGroupDesc& resourceGroupDesc, Memory** allocations) {
    size_t allocationNum = 0;
    Result result = TryToAllocateAndBindMemory(resourceGroupDesc, allocations, allocationNum);

    if (result != Result::SUCCESS) {
        for (size_t i = 0; i < allocationNum; i++) {
            m_NRI.FreeMemory(*allocations[i]);
            allocations[i] = nullptr;
        }
    }

    return result;
}

Result HelperDeviceMemoryAllocator::TryToAllocateAndBindMemory(const ResourceGroupDesc& resourceGroupDesc, Memory** allocations, size_t& allocationNum) {
    GroupByMemoryType(resourceGroupDesc.memoryLocation, resourceGroupDesc);

    for (MemoryHeap& heap : m_Heaps) {
        Memory*& memory = allocations[allocationNum];

        Result result = m_NRI.AllocateMemory(m_Device, heap.type, heap.size, memory);
        if (result != Result::SUCCESS)
            return result;

        FillMemoryBindingDescs(heap.buffers.data(), heap.bufferOffsets.data(), (uint32_t)heap.buffers.size(), *memory);
        FillMemoryBindingDescs(heap.textures.data(), heap.textureOffsets.data(), (uint32_t)heap.textures.size(), *memory);

        allocationNum++;
    }

    Result result = ProcessDedicatedResources(resourceGroupDesc.memoryLocation, allocations, allocationNum);
    if (result != Result::SUCCESS)
        return result;

    result = m_NRI.BindBufferMemory(m_Device, m_BufferBindingDescs.data(), (uint32_t)m_BufferBindingDescs.size());
    if (result != Result::SUCCESS)
        return result;

    result = m_NRI.BindTextureMemory(m_Device, m_TextureBindingDescs.data(), (uint32_t)m_TextureBindingDescs.size());

    return result;
}

Result HelperDeviceMemoryAllocator::ProcessDedicatedResources(MemoryLocation memoryLocation, Memory** allocations, size_t& allocationNum) {
    constexpr uint64_t zeroOffset = 0;
    MemoryDesc memoryDesc = {};

    for (size_t i = 0; i < m_DedicatedBuffers.size(); i++) {
        const BufferDesc& bufferDesc = m_NRI.GetBufferDesc(*m_DedicatedBuffers[i]);
        m_NRI.GetBufferMemoryDesc(m_Device, bufferDesc, memoryLocation, memoryDesc);

        Memory*& memory = allocations[allocationNum];

        Result result = m_NRI.AllocateMemory(m_Device, memoryDesc.type, memoryDesc.size, memory);
        if (result != Result::SUCCESS)
            return result;

        FillMemoryBindingDescs(m_DedicatedBuffers.data() + i, &zeroOffset, 1, *memory);

        allocationNum++;
    }

    for (size_t i = 0; i < m_DedicatedTextures.size(); i++) {
        const TextureDesc& textureDesc = m_NRI.GetTextureDesc(*m_DedicatedTextures[i]);
        m_NRI.GetTextureMemoryDesc(m_Device, textureDesc, memoryLocation, memoryDesc);

        Memory*& memory = allocations[allocationNum];

        Result result = m_NRI.AllocateMemory(m_Device, memoryDesc.type, memoryDesc.size, memory);
        if (result != Result::SUCCESS)
            return result;

        FillMemoryBindingDescs(m_DedicatedTextures.data() + i, &zeroOffset, 1, *memory);

        allocationNum++;
    }

    return Result::SUCCESS;
}

HelperDeviceMemoryAllocator::MemoryHeap& HelperDeviceMemoryAllocator::FindOrCreateHeap(nri::MemoryDesc& memoryDesc, uint64_t preferredMemorySize) {
    if (preferredMemorySize == 0)
        preferredMemorySize = 256 * 1024 * 1024;

    size_t j = 0;
    for (; j < m_Heaps.size(); j++) {
        const MemoryHeap& heap = m_Heaps[j];
        
        uint64_t offset = Align(heap.size, memoryDesc.alignment);
        uint64_t newSize = offset + memoryDesc.size;

        if (heap.type == memoryDesc.type && newSize <= preferredMemorySize)
            break;
    }

    if (j == m_Heaps.size())
        m_Heaps.push_back(MemoryHeap(memoryDesc.type, ((DeviceBase&)m_Device).GetStdAllocator()));

    return m_Heaps[j];
}

void HelperDeviceMemoryAllocator::GroupByMemoryType(MemoryLocation memoryLocation, const nri::ResourceGroupDesc& resourceGroupDesc) {
    for (uint32_t i = 0; i < resourceGroupDesc.bufferNum; i++) {
        Buffer* buffer = resourceGroupDesc.buffers[i];
        const BufferDesc& bufferDesc = m_NRI.GetBufferDesc(*buffer);

        MemoryDesc memoryDesc = {};
        m_NRI.GetBufferMemoryDesc(m_Device, bufferDesc, memoryLocation, memoryDesc);

        if (memoryDesc.mustBeDedicated)
            m_DedicatedBuffers.push_back(buffer);
        else {
            MemoryHeap& heap = FindOrCreateHeap(memoryDesc, resourceGroupDesc.preferredMemorySize);

            uint64_t offset = Align(heap.size, memoryDesc.alignment);

            heap.buffers.push_back(buffer);
            heap.bufferOffsets.push_back(offset);
            heap.size = offset + memoryDesc.size;
        }
    }

    for (uint32_t i = 0; i < resourceGroupDesc.textureNum; i++) {
        Texture* texture = resourceGroupDesc.textures[i];
        const TextureDesc& textureDesc = m_NRI.GetTextureDesc(*texture);

        MemoryDesc memoryDesc = {};
        m_NRI.GetTextureMemoryDesc(m_Device, textureDesc, memoryLocation, memoryDesc);

        if (memoryDesc.mustBeDedicated)
            m_DedicatedTextures.push_back(texture);
        else {
            MemoryHeap& heap = FindOrCreateHeap(memoryDesc, resourceGroupDesc.preferredMemorySize);

            if (heap.textures.empty()) {
                const DeviceDesc& deviceDesc = m_NRI.GetDeviceDesc(m_Device);
                heap.size = Align(heap.size, deviceDesc.bufferTextureGranularity);
            }

            uint64_t offset = Align(heap.size, memoryDesc.alignment);

            heap.textures.push_back(texture);
            heap.textureOffsets.push_back(offset);
            heap.size = offset + memoryDesc.size;
        }
    }
}

void HelperDeviceMemoryAllocator::FillMemoryBindingDescs(Buffer* const* buffers, const uint64_t* bufferOffsets, uint32_t bufferNum, Memory& memory) {
    for (uint32_t i = 0; i < bufferNum; i++) {
        BufferMemoryBindingDesc desc = {};
        desc.memory = &memory;
        desc.buffer = buffers[i];
        desc.offset = bufferOffsets[i];

        m_BufferBindingDescs.push_back(desc);
    }
}

void HelperDeviceMemoryAllocator::FillMemoryBindingDescs(Texture* const* textures, const uint64_t* textureOffsets, uint32_t textureNum, Memory& memory) {
    for (uint32_t i = 0; i < textureNum; i++) {
        TextureMemoryBindingDesc desc = {};
        desc.memory = &memory;
        desc.texture = textures[i];
        desc.offset = textureOffsets[i];

        m_TextureBindingDescs.push_back(desc);
    }
}

// © 2021 NVIDIA Corporation

#pragma once

namespace nri {

struct DeviceD3D11;
struct MemoryD3D11;

struct TextureD3D11 {
    inline TextureD3D11(DeviceD3D11& device) : m_Device(device) {
    }

    TextureD3D11(DeviceD3D11& device, const TextureDesc& textureDesc) : m_Device(device), m_Desc(textureDesc) {
    }

    inline ~TextureD3D11() {
    }

    inline DeviceD3D11& GetDevice() const {
        return m_Device;
    }

    inline const TextureDesc& GetDesc() const {
        return m_Desc;
    }

    inline operator ID3D11Resource*() const {
        return m_Texture;
    }

    inline operator ID3D11Texture1D*() const {
        return (ID3D11Texture1D*)m_Texture.GetInterface();
    }

    inline operator ID3D11Texture2D*() const {
        return (ID3D11Texture2D*)m_Texture.GetInterface();
    }

    inline operator ID3D11Texture3D*() const {
        return (ID3D11Texture3D*)m_Texture.GetInterface();
    }

    inline uint32_t GetSubresourceIndex(const TextureRegionDesc& regionDesc) const {
        return regionDesc.mipOffset + regionDesc.arrayOffset * m_Desc.mipNum;
    }

    inline Dim_t GetSize(Dim_t dimensionIndex, Mip_t mip = 0) const {
        return GetDimension(GraphicsAPI::D3D11, m_Desc, dimensionIndex, mip);
    }

    Result Create(const MemoryD3D11* memory);
    Result Create(const TextureD3D11Desc& textureDesc);

    static uint32_t GetMipmappedSize(const TextureDesc& textureDesc);

    //================================================================================================================
    // NRI
    //================================================================================================================

    inline void SetDebugName(const char* name) {
        SET_D3D_DEBUG_OBJECT_NAME(m_Texture, name);
    }

private:
    DeviceD3D11& m_Device;
    ComPtr<ID3D11Resource> m_Texture;
    TextureDesc m_Desc = {};
};

} // namespace nri

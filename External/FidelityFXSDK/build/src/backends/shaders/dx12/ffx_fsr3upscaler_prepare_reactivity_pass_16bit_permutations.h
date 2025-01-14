#include "ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558.h"

typedef union ffx_fsr3upscaler_prepare_reactivity_pass_16bit_PermutationKey {
    struct {
        uint32_t FFX_FSR3UPSCALER_OPTION_JITTERED_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_INVERTED_DEPTH : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_REPROJECT_USE_LANCZOS_TYPE : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_HDR_COLOR_INPUT : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_LOW_RESOLUTION_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_APPLY_SHARPENING : 1;
    };
    uint32_t index;
} ffx_fsr3upscaler_prepare_reactivity_pass_16bit_PermutationKey;

typedef struct ffx_fsr3upscaler_prepare_reactivity_pass_16bit_PermutationInfo {
    const uint32_t       blobSize;
    const unsigned char* blobData;


    const uint32_t  numConstantBuffers;
    const char**    constantBufferNames;
    const uint32_t* constantBufferBindings;
    const uint32_t* constantBufferCounts;
    const uint32_t* constantBufferSpaces;

    const uint32_t  numSRVTextures;
    const char**    srvTextureNames;
    const uint32_t* srvTextureBindings;
    const uint32_t* srvTextureCounts;
    const uint32_t* srvTextureSpaces;

    const uint32_t  numUAVTextures;
    const char**    uavTextureNames;
    const uint32_t* uavTextureBindings;
    const uint32_t* uavTextureCounts;
    const uint32_t* uavTextureSpaces;

    const uint32_t  numSRVBuffers;
    const char**    srvBufferNames;
    const uint32_t* srvBufferBindings;
    const uint32_t* srvBufferCounts;
    const uint32_t* srvBufferSpaces;

    const uint32_t  numUAVBuffers;
    const char**    uavBufferNames;
    const uint32_t* uavBufferBindings;
    const uint32_t* uavBufferCounts;
    const uint32_t* uavBufferSpaces;

    const uint32_t  numSamplers;
    const char**    samplerNames;
    const uint32_t* samplerBindings;
    const uint32_t* samplerCounts;
    const uint32_t* samplerSpaces;

    const uint32_t  numRTAccelerationStructures;
    const char**    rtAccelerationStructureNames;
    const uint32_t* rtAccelerationStructureBindings;
    const uint32_t* rtAccelerationStructureCounts;
    const uint32_t* rtAccelerationStructureSpaces;
} ffx_fsr3upscaler_prepare_reactivity_pass_16bit_PermutationInfo;

static const uint32_t g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_IndirectionTable[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

static const ffx_fsr3upscaler_prepare_reactivity_pass_16bit_PermutationInfo g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_PermutationInfo[] = {
    { g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_size, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_data, 1, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_CBVResourceNames, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_CBVResourceSpaces, 9, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_TextureSRVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_SamplerResourceNames, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_SamplerResourceBindings, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_SamplerResourceCounts, g_ffx_fsr3upscaler_prepare_reactivity_pass_16bit_cd3cbe29ebf2594f5772c58097387558_SamplerResourceSpaces, 0, 0, 0, 0, 0, },
};


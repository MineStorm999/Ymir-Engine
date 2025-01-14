#include "ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d.h"

typedef union ffx_fsr3upscaler_luma_instability_pass_PermutationKey {
    struct {
        uint32_t FFX_FSR3UPSCALER_OPTION_JITTERED_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_INVERTED_DEPTH : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_REPROJECT_USE_LANCZOS_TYPE : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_HDR_COLOR_INPUT : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_LOW_RESOLUTION_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_APPLY_SHARPENING : 1;
    };
    uint32_t index;
} ffx_fsr3upscaler_luma_instability_pass_PermutationKey;

typedef struct ffx_fsr3upscaler_luma_instability_pass_PermutationInfo {
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
} ffx_fsr3upscaler_luma_instability_pass_PermutationInfo;

static const uint32_t g_ffx_fsr3upscaler_luma_instability_pass_IndirectionTable[] = {
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

static const ffx_fsr3upscaler_luma_instability_pass_PermutationInfo g_ffx_fsr3upscaler_luma_instability_pass_PermutationInfo[] = {
    { g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_size, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_data, 1, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_CBVResourceNames, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_CBVResourceBindings, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_CBVResourceCounts, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_CBVResourceSpaces, 5, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_TextureSRVResourceNames, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_TextureSRVResourceBindings, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_TextureSRVResourceCounts, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_TextureSRVResourceSpaces, 2, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_TextureUAVResourceNames, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_TextureUAVResourceBindings, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_TextureUAVResourceCounts, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_SamplerResourceNames, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_SamplerResourceBindings, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_SamplerResourceCounts, g_ffx_fsr3upscaler_luma_instability_pass_f983fb9c673e165a657c70e65be42b6d_SamplerResourceSpaces, 0, 0, 0, 0, 0, },
};


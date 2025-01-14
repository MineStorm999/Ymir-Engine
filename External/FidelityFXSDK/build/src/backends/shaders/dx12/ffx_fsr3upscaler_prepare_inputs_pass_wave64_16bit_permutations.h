#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698.h"

typedef union ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_PermutationKey {
    struct {
        uint32_t FFX_FSR3UPSCALER_OPTION_LOW_RESOLUTION_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_JITTERED_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_INVERTED_DEPTH : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_REPROJECT_USE_LANCZOS_TYPE : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_HDR_COLOR_INPUT : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_APPLY_SHARPENING : 1;
    };
    uint32_t index;
} ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_PermutationKey;

typedef struct ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_PermutationInfo {
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
} ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_PermutationInfo;

static const uint32_t g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_IndirectionTable[] = {
    7,
    0,
    5,
    1,
    2,
    6,
    3,
    4,
    7,
    0,
    5,
    1,
    2,
    6,
    3,
    4,
    7,
    0,
    5,
    1,
    2,
    6,
    3,
    4,
    7,
    0,
    5,
    1,
    2,
    6,
    3,
    4,
    7,
    0,
    5,
    1,
    2,
    6,
    3,
    4,
    7,
    0,
    5,
    1,
    2,
    6,
    3,
    4,
    7,
    0,
    5,
    1,
    2,
    6,
    3,
    4,
    7,
    0,
    5,
    1,
    2,
    6,
    3,
    4,
};

static const ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_PermutationInfo g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_PermutationInfo[] = {
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_c865e7d8faee76fd066224c5f9bb59b2_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_ee86541c8770f6bc45e49712bd1f1ab5_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_7f477e84ebdb0fe893cdcd01f152a2d7_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_2ceb22902aede91e8ea9a0a6a535e5d7_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_397616dfa1524db13fe1e7117e021ccc_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_e7fba91c53af6df745f55fa276d720be_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_05b5d64beb5018ec4451f1a463c9a74b_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_16bit_9beebb3dfc8bfb063a82b829c6621698_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};


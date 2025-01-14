#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5.h"

typedef union ffx_fsr3upscaler_prepare_inputs_pass_wave64_PermutationKey {
    struct {
        uint32_t FFX_FSR3UPSCALER_OPTION_LOW_RESOLUTION_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_JITTERED_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_INVERTED_DEPTH : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_REPROJECT_USE_LANCZOS_TYPE : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_HDR_COLOR_INPUT : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_APPLY_SHARPENING : 1;
    };
    uint32_t index;
} ffx_fsr3upscaler_prepare_inputs_pass_wave64_PermutationKey;

typedef struct ffx_fsr3upscaler_prepare_inputs_pass_wave64_PermutationInfo {
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
} ffx_fsr3upscaler_prepare_inputs_pass_wave64_PermutationInfo;

static const uint32_t g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_IndirectionTable[] = {
    3,
    2,
    1,
    4,
    0,
    6,
    7,
    5,
    3,
    2,
    1,
    4,
    0,
    6,
    7,
    5,
    3,
    2,
    1,
    4,
    0,
    6,
    7,
    5,
    3,
    2,
    1,
    4,
    0,
    6,
    7,
    5,
    3,
    2,
    1,
    4,
    0,
    6,
    7,
    5,
    3,
    2,
    1,
    4,
    0,
    6,
    7,
    5,
    3,
    2,
    1,
    4,
    0,
    6,
    7,
    5,
    3,
    2,
    1,
    4,
    0,
    6,
    7,
    5,
};

static const ffx_fsr3upscaler_prepare_inputs_pass_wave64_PermutationInfo g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_PermutationInfo[] = {
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_af0a59e58a8d940a17d98a28e5f51713_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_92924d58d4c2df969bb4bdfc6ad83010_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5de3c295afe550032a4ca32bcf99b264_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_7bad7ed744b121191570baf9808568bc_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_305341899f60a6639dd5122ce63c7748_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_5ad2ce130e7e846e5280bc1e62376184_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_aebf388d6c5345e279590e8f55761899_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_size, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_wave64_15f8a768b5e8155ec83be2d029007cd5_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};


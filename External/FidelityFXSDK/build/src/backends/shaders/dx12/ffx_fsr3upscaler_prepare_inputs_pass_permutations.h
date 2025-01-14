#include "ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce.h"

typedef union ffx_fsr3upscaler_prepare_inputs_pass_PermutationKey {
    struct {
        uint32_t FFX_FSR3UPSCALER_OPTION_LOW_RESOLUTION_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_JITTERED_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_INVERTED_DEPTH : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_REPROJECT_USE_LANCZOS_TYPE : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_HDR_COLOR_INPUT : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_APPLY_SHARPENING : 1;
    };
    uint32_t index;
} ffx_fsr3upscaler_prepare_inputs_pass_PermutationKey;

typedef struct ffx_fsr3upscaler_prepare_inputs_pass_PermutationInfo {
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
} ffx_fsr3upscaler_prepare_inputs_pass_PermutationInfo;

static const uint32_t g_ffx_fsr3upscaler_prepare_inputs_pass_IndirectionTable[] = {
    7,
    0,
    3,
    2,
    6,
    1,
    4,
    5,
    7,
    0,
    3,
    2,
    6,
    1,
    4,
    5,
    7,
    0,
    3,
    2,
    6,
    1,
    4,
    5,
    7,
    0,
    3,
    2,
    6,
    1,
    4,
    5,
    7,
    0,
    3,
    2,
    6,
    1,
    4,
    5,
    7,
    0,
    3,
    2,
    6,
    1,
    4,
    5,
    7,
    0,
    3,
    2,
    6,
    1,
    4,
    5,
    7,
    0,
    3,
    2,
    6,
    1,
    4,
    5,
};

static const ffx_fsr3upscaler_prepare_inputs_pass_PermutationInfo g_ffx_fsr3upscaler_prepare_inputs_pass_PermutationInfo[] = {
    { g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_size, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_fa879fc72cc03832a2ed4c70a0221e45_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_size, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_9dfe54b336ded92eb834d2bebdfda636_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_size, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_56ca6e48520b66a392f014c115db4277_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_size, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_589fbfdd000243089fa7d3ade84d0a64_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_size, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_5f7140ed873b46937f34064eb7b31412_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_size, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_0516b337e882ff6eb65e6b8ffa26da4b_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_size, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_fd5a6e404d355fb934c70945d49cdd99_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_size, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_0c0bae758512abf9c7c580fc154e84ce_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};


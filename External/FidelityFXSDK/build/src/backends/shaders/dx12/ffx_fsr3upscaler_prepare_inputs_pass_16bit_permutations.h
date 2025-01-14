#include "ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1.h"
#include "ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38.h"

typedef union ffx_fsr3upscaler_prepare_inputs_pass_16bit_PermutationKey {
    struct {
        uint32_t FFX_FSR3UPSCALER_OPTION_LOW_RESOLUTION_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_JITTERED_MOTION_VECTORS : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_INVERTED_DEPTH : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_REPROJECT_USE_LANCZOS_TYPE : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_HDR_COLOR_INPUT : 1;
        uint32_t FFX_FSR3UPSCALER_OPTION_APPLY_SHARPENING : 1;
    };
    uint32_t index;
} ffx_fsr3upscaler_prepare_inputs_pass_16bit_PermutationKey;

typedef struct ffx_fsr3upscaler_prepare_inputs_pass_16bit_PermutationInfo {
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
} ffx_fsr3upscaler_prepare_inputs_pass_16bit_PermutationInfo;

static const uint32_t g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_IndirectionTable[] = {
    7,
    1,
    4,
    0,
    6,
    3,
    2,
    5,
    7,
    1,
    4,
    0,
    6,
    3,
    2,
    5,
    7,
    1,
    4,
    0,
    6,
    3,
    2,
    5,
    7,
    1,
    4,
    0,
    6,
    3,
    2,
    5,
    7,
    1,
    4,
    0,
    6,
    3,
    2,
    5,
    7,
    1,
    4,
    0,
    6,
    3,
    2,
    5,
    7,
    1,
    4,
    0,
    6,
    3,
    2,
    5,
    7,
    1,
    4,
    0,
    6,
    3,
    2,
    5,
};

static const ffx_fsr3upscaler_prepare_inputs_pass_16bit_PermutationInfo g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_PermutationInfo[] = {
    { g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_size, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_742e4200c9057cb974669180bee32333_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_size, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_54de0a47c2f91d64f12e42c54cc1157b_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_size, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_676ad0fef82766662b7258e3acc8fe77_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_size, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5b925254657f990b598cc59f683b92e0_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_size, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_d47b77714f94cb07b526f9ac6270eb60_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_size, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_70a484d481d455a3ce9cb08ed6a13034_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_size, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_5346caf08e3417e365327e529df392a1_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_size, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_data, 1, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_CBVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_CBVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_CBVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_CBVResourceSpaces, 3, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_TextureSRVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_TextureSRVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_TextureSRVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_TextureSRVResourceSpaces, 5, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_TextureUAVResourceNames, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_TextureUAVResourceBindings, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_TextureUAVResourceCounts, g_ffx_fsr3upscaler_prepare_inputs_pass_16bit_a9eb5bf3abb7bb425e359f5c53160b38_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};


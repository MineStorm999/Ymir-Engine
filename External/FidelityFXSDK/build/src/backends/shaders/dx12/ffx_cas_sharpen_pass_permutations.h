#include "ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27.h"
#include "ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a.h"
#include "ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855.h"
#include "ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87.h"
#include "ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90.h"
#include "ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd.h"
#include "ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680.h"
#include "ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e.h"
#include "ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863.h"
#include "ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a.h"

typedef union ffx_cas_sharpen_pass_PermutationKey {
    struct {
        uint32_t FFX_CAS_OPTION_SHARPEN_ONLY : 1;
        uint32_t FFX_CAS_COLOR_SPACE_CONVERSION : 3;
    };
    uint32_t index;
} ffx_cas_sharpen_pass_PermutationKey;

typedef struct ffx_cas_sharpen_pass_PermutationInfo {
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
} ffx_cas_sharpen_pass_PermutationInfo;

static const uint32_t g_ffx_cas_sharpen_pass_IndirectionTable[] = {
    6,
    1,
    8,
    3,
    5,
    4,
    2,
    0,
    7,
    9,
    0,
    0,
    0,
    0,
    0,
    0,
};

static const ffx_cas_sharpen_pass_PermutationInfo g_ffx_cas_sharpen_pass_PermutationInfo[] = {
    { g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_size, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_data, 1, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_CBVResourceNames, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_CBVResourceBindings, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_CBVResourceCounts, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_666fe85c8636ca95495760be6e293c27_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_size, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_data, 1, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_CBVResourceNames, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_CBVResourceBindings, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_CBVResourceCounts, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_2e70e03006a9df2cb61b17721cb88a1a_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_size, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_data, 1, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_CBVResourceNames, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_CBVResourceBindings, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_CBVResourceCounts, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_9b2f4ada785d1e3c0101cf6a944a4855_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_size, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_data, 1, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_CBVResourceNames, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_CBVResourceBindings, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_CBVResourceCounts, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_58e1d7b8755f8cbec3cbee6c84893c87_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_size, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_data, 1, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_CBVResourceNames, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_CBVResourceBindings, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_CBVResourceCounts, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_22f8a552bd25ce3d10ad45b78ffa3b90_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_size, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_data, 1, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_CBVResourceNames, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_CBVResourceBindings, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_CBVResourceCounts, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_0ed57d6e4b7a8e3de55b88928d1d4ebd_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_size, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_data, 1, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_CBVResourceNames, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_CBVResourceBindings, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_CBVResourceCounts, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_559a009f753b9b079c8330018217c680_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_size, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_data, 1, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_CBVResourceNames, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_CBVResourceBindings, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_CBVResourceCounts, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_3c6304391cfe32dd34447fcca134fd9e_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_size, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_data, 1, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_CBVResourceNames, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_CBVResourceBindings, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_CBVResourceCounts, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_ea7a6bcc406c56fff3a4379e0898e863_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
    { g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_size, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_data, 1, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_CBVResourceNames, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_CBVResourceBindings, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_CBVResourceCounts, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_CBVResourceSpaces, 1, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_TextureSRVResourceNames, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_TextureSRVResourceBindings, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_TextureSRVResourceCounts, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_TextureSRVResourceSpaces, 1, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_TextureUAVResourceNames, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_TextureUAVResourceBindings, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_TextureUAVResourceCounts, g_ffx_cas_sharpen_pass_2d9df8469772ace7883deba1c37c385a_TextureUAVResourceSpaces, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
};


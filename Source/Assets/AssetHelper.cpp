#include "AssetHelper.h"


nlohmann::json JSONHelper::TJson(float2& v)
{
    nlohmann::json j;
    j["X"] = v.x;
    j["Y"] = v.x;
    return j;
}

float2 JSONHelper::FrmJsonf2(nlohmann::json& j)
{
    float2 f;
    f.x = j["X"];
    f.y = j["Y"];
    return f;
}

nlohmann::json JSONHelper::TJson(float3& v)
{
    nlohmann::json j;
    j["X"] = v.x;
    j["Y"] = v.x;
    j["Z"] = v.z;
    return j;
}

float3 JSONHelper::FrmJsonf3(nlohmann::json& j)
{
    float3 f;
    f.x = j["X"];
    f.y = j["Y"];
    f.z = j["Z"];
    return f;
}

nlohmann::json JSONHelper::TJson(float4& v)
{
    nlohmann::json j;
    j["X"] = v.x;
    j["Y"] = v.x;
    j["Z"] = v.z;
    j["W"] = v.w;
    return j;
}

float4 JSONHelper::FrmJsonf4(nlohmann::json& j)
{
    float4 f;
    f.x = j["X"];
    f.y = j["Y"];
    f.z = j["Z"];
    f.w = j["W"];
    return f;
}

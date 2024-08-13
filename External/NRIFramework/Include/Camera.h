// © 2021 NVIDIA Corporation

#pragma once

struct CameraDesc
{
    cBoxf limits = {};
    float3 dLocal = float3(0.0f);
    float3 dUser = float3(0.0f);
    float dYaw = 0.0f; // deg
    float dPitch = 0.0f; // deg
    float aspectRatio = 1.0f;
    float horizontalFov = 90.0f; // deg
    float nearZ = 0.1f;
    float farZ = 10000.0f;
    float orthoRange = 0.0f;
    float timeScale = 0.5f;
    float backwardOffset = 0.0f;
    bool isReversedZ = false;
    bool isPositiveZ = true;
    bool isCustomMatrixSet = false;
    float4x4 customMatrix = float4x4::Identity();
};

struct CameraState
{
    double3 globalPosition = {};
    float4x4 mViewToClip = float4x4::Identity();
    float4x4 mClipToView = float4x4::Identity();
    float4x4 mWorldToView = float4x4::Identity();
    float4x4 mViewToWorld = float4x4::Identity();
    float4x4 mWorldToClip = float4x4::Identity();
    float4x4 mClipToWorld = float4x4::Identity();
    float3 position = {};
    float3 rotation = {};
    float2 viewportJitter = {};
    float motionScale = 0.015f;
};

class Camera
{
public:
    void Update(const CameraDesc& desc, uint32_t frameIndex);
    void Initialize(const float3& position, const float3& lookAt, bool isRelative = false);
    void InitializeWithRotation(const float3& position, const float3& rotationDegrees, bool isRelative);

    inline void SavePreviousState()
    {
        statePrev = state;
    }

    inline const float3 GetRelative(const double3& origin) const
    {
        double3 position = m_IsRelative ? state.globalPosition : double3(0.0);

        return float3(origin - position);
    }

    inline void* GetState()
    { return &state; }

    static inline uint32_t GetStateSize()
    { return sizeof(CameraState); }

public:
    CameraState state = {};
    CameraState statePrev = {};
private:
    bool m_IsRelative = false;
};

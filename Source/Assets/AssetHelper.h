#pragma once
#include "MathLib/ml.h"
#include "JSON/json.hpp"

class JSONHelper {
public:
	// float2
	static nlohmann::json TJson(float2& v);
	static float2 FrmJsonf2(nlohmann::json& j);

	// float3
	static nlohmann::json TJson(float3& v);
	static float3 FrmJsonf3(nlohmann::json& j);

	// float4 
	static nlohmann::json TJson(float4& v);
	static float4 FrmJsonf4(nlohmann::json& j);

	// bool 
	static nlohmann::json TJson(bool b);
	static bool FrmJsonb(nlohmann::json& j);

};
#pragma once

#include "NRIFramework.h"

struct NRIInterface
	: public nri::CoreInterface
	, public nri::HelperInterface
	, public nri::StreamerInterface
	, public nri::SwapChainInterface
{};

enum class OdBufferOrder : uint8_t {
	// structured buffer
	COLLIDER_DATA,
	COLLIDER_DESCS,
	COLLIDER_CLUSTER_DESCS,
	RIGID_BODIES,

	// Read Write
	RETURN_POSITIONS,
	RETURN_RB,
	RETURN_COLLISION_EVENTS,

	// Readback
	READBACK
};

class OdPhysics {
public:
	OdPhysics(NRIInterface& NRI, nri::Device* device);
	~OdPhysics();

	void Update(float dt);

private:
	nri::CommandQueue* m_CommandQueue = nullptr;

	nri::PipelineLayout* m_PhysicsPipelineLayout = nullptr;
	nri::Pipeline* m_PhysicsPipeline = nullptr;

	nri::DescriptorPool* m_DescriptorPool = nullptr;

	std::vector<nri::DescriptorSet*> m_DescriptorSets;
	std::vector<nri::Buffer*> m_buffer;
	std::vector<nri::Descriptor*> m_Descriptors;
	nri::Memory* m_memory;
};
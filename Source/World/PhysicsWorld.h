#pragma once


#ifdef PHYSX_PHYSICS

//#include "../Assets/assetTypes.h"
//#include "Assets/AssetManager.h"
#include "../External/PhysX_5/physx/include/PxPhysicsAPI.h"
//#include <thread>

#define STEPS_PER_FRAME 2



class PhyisicsWorld {
public:
	PhyisicsWorld(uint32_t id) { Init(id); };
	~PhyisicsWorld();
	void Init(uint32_t id);
	void Step(float dt);
	
	void Sync();

	physx::PxPhysics* GetPhysicsSystem() { return m_Physics; };

	void AddBody(physx::PxActor& body);
	void DestroyBody(physx::PxActor& body);


private:
	physx::PxDefaultAllocator		m_Allocator;
	physx::PxDefaultErrorCallback	m_ErrorCallback;
	physx::PxFoundation* m_Foundation = NULL;
	physx::PxPhysics* m_Physics = NULL;
	physx::PxDefaultCpuDispatcher* m_Dispatcher = NULL;
	physx::PxScene* m_Scene = NULL;

	
	// adding removing
	void HandleRequests();
	std::vector<physx::PxActor*> addRequests;

	std::vector<physx::PxActor*> removeRequests;
};
#endif // PHYSX_PHYSICS
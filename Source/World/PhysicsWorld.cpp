#include "PhysicsWorld.h"
#include "Log/Log.h"

#ifdef PHYSX_PHYSICS
using namespace physx;

PhyisicsWorld::~PhyisicsWorld()
{
	if (m_Physics) {
		m_Physics->release();
	}
}

void PhyisicsWorld::Init(/*AssetID id*/)
{
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;

	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback,
		gDefaultErrorCallback);
	if (!m_Foundation)
		Log::Error("PhysX", "PxCreateFoundation failed!");

	
	/*
	mPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
	*/

	bool recordMemoryAllocations = true;
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation,
		PxTolerancesScale(), recordMemoryAllocations, nullptr);
	if (!m_Physics)
		Log::Error("PxCreatePhysics failed!");

	if (!PxInitExtensions(*m_Physics, nullptr))
		Log::Error("PxInitExtensions failed!");

	m_Scene = m_Physics->createScene(PxSceneDesc(PxTolerancesScale()));
	if (!m_Scene) {
		Log::Error("Creation of PxScene failed!");
	}
}

void PhyisicsWorld::Step(float dt)
{
	if (!m_Scene) {
		return;
	}
	for (size_t i = 0; i < STEPS_PER_FRAME; i++)
	{
		m_Scene->simulate(dt / (float)STEPS_PER_FRAME);
		if (i < (STEPS_PER_FRAME - 1)) {
			m_Scene->fetchResults(true);
		}
	}
	
}

void PhyisicsWorld::Sync()
{
	m_Scene->fetchResults(true);
}

void PhyisicsWorld::AddBody(physx::PxActor& body)
{
}

void PhyisicsWorld::DestroyBody(physx::PxActor& body)
{
}
/*
void PhyisicsWorld::HandleRequests()
{
}*/

#endif //PHYSX_PHYSICS
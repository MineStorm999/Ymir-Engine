#include "PhyisicsWorld.h"

#ifdef PHYSICS
#include <thread>

#define MAX_BODIES 0xffff
#define MAX_BODY_PAIRS 0xffff
#define MAX_CONTACT_CONSTRAINTS 10240
#define MAX_BODY_MUTEXES 0

// threads
// has to be more than 1
#define MAX_PHYYSICS_THREADS 4
#define MAX_PHYSICS_JOBS cMaxPhysicsJobs
#define MAX_PHYSICS_BARRIERS cMaxPhysicsBarriers

namespace {
	// threads
	JobSystemThreadPool job_system(MAX_PHYSICS_JOBS, MAX_PHYSICS_BARRIERS, MAX_PHYYSICS_THREADS - 1);
	std::thread* t{nullptr};

	TempAllocatorImpl temp_allocator(10 * 1024 * 1024);
	PhysicsSystem physics_system;

	// interfaces
	BPLayerInterfaceImpl broad_phase_layer_interface;
	ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;
	ObjectLayerPairFilterImpl object_vs_object_layer_filter;

	// listener
	MyBodyActivationListener body_activation_listener;
	MyContactListener contact_listener;
	BodyInterface* body_interface{nullptr};
}

void PhyisicsWorld::Init()
{
	// Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
	// This needs to be done before any other Jolt function is called.
	RegisterDefaultAllocator();

	// Install trace and assert callbacks
	Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl;)

	// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
	// It is not directly used in this example but still required.
	Factory::sInstance = new Factory();

	// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
	// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
	// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
	RegisterTypes();


	// Now we can create the actual physics system.
	physics_system.Init(MAX_BODIES, MAX_BODY_MUTEXES, MAX_BODY_PAIRS, MAX_CONTACT_CONSTRAINTS, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);

	physics_system.SetBodyActivationListener(&body_activation_listener);
	physics_system.SetContactListener(&contact_listener);

	body_interface = &physics_system.GetBodyInterface();

	// Next we can create a rigid body to serve as the floor, we make a large box
	// Create the settings for the collision volume (the shape).
	// Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
	BoxShapeSettings floor_shape_settings(Vec3(100.0f, 1.0f, 100.0f));
	floor_shape_settings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

	// Create the shape
	ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
	ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

	// Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
	BodyCreationSettings floor_settings(floor_shape, RVec3(0.0_r, -1.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Static, Layers::NON_MOVING);

	// Create the actual rigid body
	Body* floor = body_interface->CreateBody(floor_settings); // Note that if we run out of bodies this can return nullptr

	// Add it to the world
	body_interface->AddBody(floor->GetID(), EActivation::DontActivate);

	// Now create a dynamic body to bounce on the floor
	// Note that this uses the shorthand version of creating and adding a body to the world
	BodyCreationSettings sphere_settings(new SphereShape(0.5f), RVec3(0.0_r, 2.0_r, 0.0_r), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
	BodyID sphere_id = body_interface->CreateAndAddBody(sphere_settings, EActivation::Activate);

	// Now you can interact with the dynamic body, in this case we're going to give it a velocity.
	// (note that if we had used CreateBody then we could have set the velocity straight on the body before adding it to the physics system)
	body_interface->SetLinearVelocity(sphere_id, Vec3(0.0f, -5.0f, 0.0f));

	// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.
	const float cDeltaTime = 1.0f / 60.0f;

	// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
	// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
	// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
	physics_system.OptimizeBroadPhase();
}

void StepImpl(float dt) {
	// 1 step per 0.016 // step = dt / 0.016
	physics_system.Update(dt, std::ceil(dt / (float)(1.0f / 60)), &temp_allocator, &job_system);
}

void PhyisicsWorld::Step(float dt)
{
	delete t;
	t = new thread(StepImpl, dt);
}

void PhyisicsWorld::Sync()
{
	if (t) {
		t->join();
	}
}
#endif //PHYSICS
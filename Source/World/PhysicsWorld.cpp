#include "PhysicsWorld.h"

#ifdef JOLT_PHYSICS

#include <thread>
using namespace JPH;

// If you want your code to compile using single or double precision write 0.0_r to get a Real value that compiles to double or float depending if JPH_DOUBLE_PRECISION is set or not.
using namespace JPH::literals;

// We're also using STL classes in this example
using namespace std;
void PhyisicsWorld::Init()
{
	// Register allocation hook. In this example we'll just let Jolt use malloc / free but you can override these if you want (see Memory.h).
	// This needs to be done before any other Jolt function is called.
	RegisterDefaultAllocator();

	temp_allocator = new TempAllocatorImpl(10 * 1024 * 1024);

	// Install trace and assert callbacks
	Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(AssertFailed = AssertFailedImpl);

	// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
	// It is not directly used in this example but still required.
	Factory::sInstance = new Factory();

	// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
	// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
	// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
	RegisterTypes();

	job_system = new JobSystemThreadPool(MAX_PHYSICS_JOBS, MAX_PHYSICS_BARRIERS, (MAX_PHYYSICS_THREADS - 1));

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

	// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
	// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
	// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
	physics_system.OptimizeBroadPhase();
}

struct Load {
	PhysicsSystem* sys;
	TempAllocatorImpl* tmpAlloc;
	JobSystemThreadPool* jobSys;
	//float dt;

	Load(PhysicsSystem* s, TempAllocatorImpl* t, JobSystemThreadPool* j) { 
		sys = s;
		tmpAlloc = t;
		jobSys = j;
		//dt = d;
	}
};
bool step = false;
bool prozessing = false;
float deltaT = 0.0f;
static void StepImpl(Load l) {
	while (!step)
	{
		std::this_thread::sleep_for(std::chrono::duration<double>(.1ms));
	}
	prozessing = true;
	step = false;
	l.sys->Update(deltaT, (int)deltaT / (1.0f / 60.f), l.tmpAlloc, l.jobSys);
	prozessing = false;
}

void PhyisicsWorld::Step(float dt)
{
	if (!job_system) {
		return;
	}
	if (!t) {
		t = new thread(StepImpl, Load(&physics_system, temp_allocator, job_system));
	}
	Sync();
	deltaT = dt;
	step = true;
}

void PhyisicsWorld::Sync()
{
	while (!prozessing)
	{
		std::this_thread::sleep_for(std::chrono::duration<double>(.1ms));
	}
}
#endif //JOLT_PHYSICS
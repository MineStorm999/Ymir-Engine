# Ymir Engine

Currently in work, nothing to see jet.
(have a look at the "Dev_Using_PhysX5" Branch)


# Installation:
- Windows
 1. clone recursively ```git clone --recursive https://github.com/MineStorm999/Ymir-Engine```
 2. install Windows SDK (at least v.10.0.20348.0)
 3. Install Vulkan SDK
 4. run deploy.bat
 5. your solution file should be in _Build
 6. build+run with your IDE or run build.bat and run the .exe in _Bin/Debug or _Bin/Realese 
# Compability
- ✓ Tested devices (passing) 3 
- ✓ Windows 2
- ✓X Linux  1
- X MacOS(will never happen)


# Bug To Fix
- Crash when Going back too often in Asset Manager




# Roadmap:
- Rendering
  - ✓ Bindless
  - X Deferred 
  - X Runtime 3d model add/rmf
  - X Point Lights
  - ✓ Dir Light
  - X Shadows
  - X Global Illumination
  - X PathTracing
  - X Skinned Mesh
  - ✓ 10k moving cubes
  - X HZB culling
  - X GPU frustum culling
- Assets
  - ✓ Virtual File System
  - ✓ Asset Manager
- Editor UI
  - ✓ Log
  - ✓ Asset Browser
  - ✓ Inspector
  - ✓ Struct Editor
  - X Drag + Drop
  - ✓ Entity Hirachie visualizer
- Visual Scripting
  - ✓ Struct Editor 
  - ✓ Node Editor 
  - ✓ Base Node
  - X Variables
  - ✓ Bool Ops
- ECS
  - ✓ Macros for Transform
  - X Macros for RigidBody

- Physics 
  - X Impl of Physx5 
  - X Custom engine
     - X collider creation 
    - X ECS components
    - X broad phase
    - X narrow phase
    - X springs
  

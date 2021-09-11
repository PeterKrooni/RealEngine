# Real Engine 

Real Engine is a work in progress game engine written i C++ with SDL2.0, with continuous updates and changes.
Features include: 
- A basic physics engine with simple ray-casting based collisions
- Rendering system with rendering layers and priorites 
- A small suite of debugging tools
- Map handling

The Source.cpp file will contain different types of demos of features currently in progress, but is not meant as an "editor" in any way. 
More in-depth documentation is coming soon.

## How to build
Right now there aren't really any easy ways to compile the project, but I am working on it. An alternative solution is to clone the repository into a new VS project (or IDE of your choice):

Requirements: 
- SDL2.0, SDL2_image, SDL2_ttf, SDL2_mixer
-	C++17 or higher. 
---
1. Add SDL2 libraries and include directories to VS
2. Clone into this repository
3. Run Source.cpp

## Overview
#### Design pattern: Entity-Component System 
The engine's structure is based upon ECS, Entity-Component System. 
Every entity in the engine gets initialized with an EntityPositionComponent, and the entities can implement further features by adding different components to it, like a RenderableComponent, or a DebuggingComponent. 
These entities have their components managed by the Managers, which handle entity interactions between their respective components. 

#### Assets
No assets are currently included in the engine, as asset handling still needs some work before that becomes a priority.

#### Documentation
Custom real engine objects are prepended with 'VX_ + object type_ + name'

VX_C ||  Class

VX_S || Struct

VX_E || Enum

VX_EC || Enum Class

VX_N  || Namespace


More coming soon.

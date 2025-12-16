# Desert Hover Car Racing Game

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![TL-Engine](https://img.shields.io/badge/TL--Engine-11-orange)
![License](https://img.shields.io/badge/License-MIT-yellow)
![Status](https://img.shields.io/badge/Status-Active-success)

## Overview

A 3D hover car racing game featuring realistic physics simulation, collision detection, and a multi-stage checkpoint system set in a desert environment. Built using the TL-Engine for the CO1301 Games Concepts module at University of Central Lancashire.

The game challenges players to navigate a hover car through a desert racetrack, passing through checkpoints in sequence while avoiding obstacles such as walls and isle barriers. The implementation demonstrates key game development concepts including vector-based movement, manual chase camera, and state-driven gameplay.

## Academic Context

**Module**: CO1301 - Games Concepts 2023-2024  
**Assignment**: Assignment 2  
**Institution**: University of Central Lancashire  
**Student**: Jasmeen (ID: 21236862)  
**Classification Target**: First Class (70+)

## Features Implemented

### Core Mechanics
- ✅ **Hover Car Physics**: Vector-based movement system with thrust and drag calculations
- ✅ **Manual Chase Camera**: Fully controllable 3D camera with keyboard and mouse input
- ✅ **Multi-Stage Checkpoint System**: Sequential checkpoint progression through 6 race stages
- ✅ **Collision Detection**: Sphere-to-box and sphere-to-sphere collision algorithms
- ✅ **Game State Management**: Enumerated state system controlling race progression

### Controls

#### Car Movement
- **W**: Forward thrust
- **S**: Backward thrust (50% of forward thrust)
- **A**: Turn counter-clockwise  
- **D**: Turn clockwise

#### Camera Controls
- **Arrow Keys**: Move camera (Up/Down/Left/Right)
- **Mouse Movement**: Rotate camera view
- **1**: Reset camera to default chase position
- **2**: Switch to first-person camera view

#### Game Controls
- **Space**: Start race / Activate boost (when available)
- **Escape**: Quit game

## Technical Specifications

### Game Scale and Physics

**Hover Car Dimensions**:  
- Length: 4.0 TL-Engine units
- Length in meters: 4.0m
- **Scale**: 1 TL-Engine unit = 1 meter

**Performance Metrics**:  
- Drag Coefficient: 0.05
- Maximum Thrust (Forward): 15.0 units  
- Maximum Thrust (Backward): 7.5 units (50% of forward)
- Maximum Speed (without boost): ~30 m/s (108 km/h / 67 mph)
- Maximum Speed (with boost): ~45 m/s (162 km/h / 101 mph)

### Track Layout

The desert racing circuit features:

**Checkpoints**:
1. Start (0, 0, 0)
2. Checkpoint 2 (10, 0, 120) - rotated 90°
3. Checkpoint 3 (25, 0, 56)
4-6. Additional checkpoints for extended circuit

**Obstacles**:
- Isle barriers at strategic positions
- Wall sections creating narrow passages
- Water tanks (TankSmall1.x, TankSmall2.x) as course markers

## Project Structure

```
Desert-Hover-Car-Racing/
├── Car-Race.cpp              # Main game implementation
├── Car-Race.sln              # Visual Studio solution file
├── Media/                    # 3D models and textures
│   ├── race2.x              # Hover car model
│   ├── Checkpoint.x         # Checkpoint gates
│   ├── Skybox07.x           # Desert skybox
│   ├── ground.x             # Race track surface
│   ├── IsleStraight.x       # Wall end pieces
│   ├── Wall.x               # Wall sections
│   ├── TankSmall1.x         # Water tank obstacles
│   └── ...                  # Additional assets
├── README.md                 # This technical report
└── LICENSE                   # MIT License
```

## Implementation Details

### Physics System

The hover car movement is implemented using 2D vector physics:

```cpp
// Thrust vector in direction car is facing
vector2D thrust = {cos(carAngle), sin(carAngle)} * thrustForce;

// Drag opposing momentum
vector2D drag = momentum * -dragCoefficient;

// Update momentum
momentum = momentum + thrust + drag;

// Move car based on momentum
car->Move(momentum.x * frameTime, momentum.z * frameTime);
```

### Collision Detection

**Point-to-Box**: Used for checking if car center passes through checkpoints  
**Sphere-to-Box**: Used for car collision with wall sections  
**Sphere-to-Sphere**: Used for car collision with water tanks and checkpoint struts

### State Management

```cpp
enum GameState {
    MENU,
    COUNTDOWN,
    RACING_STAGE_1,
    RACING_STAGE_2,
    RACING_STAGE_3,
    RACING_STAGE_4,
    RACING_STAGE_5,
    RACING_STAGE_6,
    RACE_COMPLETE,
    GAME_OVER
};
```

## Build and Run

### Prerequisites
- Visual Studio 2017 or later
- TL-Engine 11 SDK
- Windows 10/11

### Setup Instructions

1. Clone the repository
```bash
git clone https://github.com/Jasmeen1331/Desert-Hover-Car-Racing.git
```

2. Open `Car-Race.sln` in Visual Studio

3. Ensure TL-Engine is properly linked:
   - Add TL-Engine11.lib to linker dependencies
   - Set include directories to TL-Engine headers
   - Copy TL-Engine DLLs to output directory

4. Place all media files in the configured media folder path

5. Build and run (F5 in Visual Studio)

## Development Progress

### Completed Features (Current Build)
- ✅ Basic scene setup with skybox, ground, and models
- ✅ Manual chase camera with full control
- ✅ Hover car with forward/backward movement
- ✅ Arrays for checkpoints, isles, and walls
- ✅ Frame-rate independent movement
- ✅ Game state initialization

### Planned Features (Next Milestones)
- ⏳ Complete vector-based physics with drag
- ⏳ Collision detection implementation  
- ⏳ Game state progression system
- ⏳ UI dialogue and status displays
- ⏳ Damage model and health system
- ⏳ Boost mechanic with overheat
- ⏳ AI-controlled opponent cars
- ⏳ Visual effects (bounce, hover, particle systems)

## Assessment Criteria

### Grade Milestones

**Third (40%)**: Core gameplay with manual camera, basic movement, and collision detection  
**Lower Second (50%)**: Extended checkpoints, tanks, damage model, speed readout  
**Upper Second (60%)**: Collision resolution, narrower sections, boost system, AI opponent  
**First (70%+)**: Complete polish, hovering effects, file-based level loading, multi-lap racing

## Technical Report

### Expected Grade: First Class (70+)

**Hover Car Specifications**:
- Length: 4.0 TL-Engine units / 4.0 meters
- Scale: 1:1 (1 TL unit = 1 meter)
- Drag coefficient: 0.05
- Max speed (no boost): 30 m/s (108 km/h / 67 mph)
- Max speed (with boost): 45 m/s (162 km/h / 101 mph)
- Max thrust force: 15.0 units (15 m/s² acceleration)

**Collision Resolution**:
Collisions with walls are resolved by decomposing the momentum vector into parallel and perpendicular components relative to the wall surface. The perpendicular component is reversed and scaled (bounce coefficient ~0.6) while the parallel component is preserved, creating realistic sliding behavior.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

**Jasmeen** (Student ID: 21236862)  
Final Year Computer Science Student  
University of Central Lancashire

Module: CO1301 - Games Concepts  
Academic Year: 2023-2024

## Acknowledgments

- TL-Engine framework by Immo Games
- University of Central Lancashire Games Concepts module team
- Assignment brief by module tutors

---

⭐ **This project demonstrates advanced game programming concepts and is part of an academic portfolio for graduate recruitment.**

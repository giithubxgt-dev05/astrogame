# Project Cosmos

**Project Cosmos** is a cinematic open-world solar system exploration game inspired by the aesthetic of Mass Effect. Embark on an epic journey across visually stunning planets, establish survival bases, and uncover the mysteries of the solar system.

## Vision Statement
To create an immersive, scientifically grounded yet cinematic space exploration experience where players can seamlessly transition from walking on alien surfaces to piloting advanced spacecraft through the cosmos.

## Engine & Plugin Requirements
- **Unreal Engine Version:** UE 5.3+
- **Required Features:** 
  - Nanite (Virtual Geometry)
  - Lumen (Global Illumination & Reflections)
  - World Partition & Large World Coordinates (LWC)
  - Chaos Physics
  - Gameplay Ability System (GAS)
  - Enhanced Input System

## Setup Instructions
1. Clone the repository to your local machine.
2. Right-click on the `ProjectCosmos.uproject` file.
3. Select **Generate Visual Studio project files**.
4. Open `ProjectCosmos.sln` in Visual Studio or JetBrains Rider.
5. Compile the project in `Development Editor` mode.
6. Open the `.uproject` file in Unreal Engine 5.3.

## Project Structure Overview
- `Source/ProjectCosmos/Core/` - Game instance, game modes, and foundational systems.
- `Source/ProjectCosmos/Player/` - Character, controller, and survival components.
- `Source/ProjectCosmos/UI/` - HUD, inventory, and widget implementations.
- `Source/ProjectCosmos/BaseBuilding/` - Subsystems and actors for establishing planetary bases.
- `Source/ProjectCosmos/Environment/` - Procedural planetary generation and weather systems.
- `Content/Data/` - Data assets for planets, ships, items, and missions.

## Architecture Overview
The project architecture leverages Unreal Engine 5's modern systems:
- **Subsystems:** `UBaseManagerSubsystem`, `UPlanetManager` are utilized for scalable, persistent management of bases and celestial bodies across the game world.
- **Components:** Modular character functionality via `USurvivalComponent`, `UInventoryComponent`.
- **UI:** Event-driven UI updates through `UCosmosHUDWidget` responding to component states.

## Key Systems Description
- **Survival System:** Manages oxygen, temperature, radiation, and suit integrity on extreme worlds.
- **Base Building:** Snap-based modular construction. Bases recalculate power grids dynamically and provide life support.
- **Ship Navigation:** Seamless atmospheric and vacuum flight models utilizing Chaos Physics and Enhanced Input.
- **Procedural Planets:** Data-driven planet definitions modifying atmospheric density, gravity, and terrain generation.

## Art Direction
- **Style:** Cinematic-stylized realism, akin to Mass Effect.
- **Lighting:** High-contrast atmospheric lighting leveraging Lumen.
- **Design:** Industrial, rugged near-future aesthetics for human tech, juxtaposed against sweeping natural planetary landscapes.

## Contributing Guidelines
1. Ensure all C++ adheres strictly to Epic's coding standard.
2. All new properties must be appropriately marked with `UPROPERTY` and organized into `Cosmos|CategoryName`.
3. Provide full implementations; do not submit stubs.
4. Always test in a networked context (Listen Server & Client) if modifying replicated variables.

## License
This project is licensed under the MIT License - see the LICENSE file for details.

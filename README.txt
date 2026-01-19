ID 207765314 - Dekel Yigal

Features:
- 2 players
- Movement
- Walls
- Doors
- Keys

- Obstecals
- Spring
- Switch
- Torch and dar rooms
- Bomb
- Riddles


Architecture:
--------------------------------------
# Architecture and Project Structure

This document describes the high-level architecture, module responsibilities, and on-disk structure for the game project, with emphasis on the separation between Props, `Room` composition/logic, and `MapObject` subclasses.

## Overview
The game is a small 2D puzzle/interaction engine:
- 2 players, movement, collision, and environment interactions
- Props: walls, doors, keys, switches, springs, torches (dark rooms), bombs, obstacles, riddles
- Levels described by `.screen` files and auxiliary text assets

Design goals:
- Explicit separation of responsibilities (data, rendering, logic)
- `Room` as a composition root for scene-level behavior and event routing
- Clear difference between lightweight map objects (tiles / data holders) and active Props (behavior/logic)

## Key Concepts

- Room (composition + event handling)
  - Implemented in `Room.h` / `Room.cpp` (the room file is the scene-level container).
  - A `Room` composes `MapObject` instances and `Prop` instances rather than inheriting from them.
  - Responsibilities:
    - Own lifecycle of contained objects (create/destroy)
    - Route events and messages between contained objects (uses `Msg.h` / `Msg.cpp`)
    - Maintain scene-level state (lighting, timers, active players)
    - Provide hooks for custom room logic (special triggers, scripted sequences)
  - Rationale: composition keeps Rooms flexible and allows adding per-room orchestration without polluting prop implementations.

- Props (behavioral components)
  - Props are the classes that encapsulate active behavior: `BaseProp`, `DoorProp`, `TorchProp`, `PlayersProp`, etc.
  - Each Prop implements lifecycle methods
  - Props:
    - Own behavior and small internal state (timers, scores, activation flags)
    - Are colocated in the `Room` and operate on `MapObject` data when needed
  - Rationale: Props encapsulate behavior so they can be reused across different rooms and test levels.

- MapObject subclasses (tile / data representations)
  - MapObjects describe what occupies a map cell: walls, obstecale, doors, riddle, etc..
  - MapObject subclasses are lightweight and focused on representation and rules for collision and placement.
  - They are not responsible for extended behaviors or scene orchestration.
  - Examples: `Wall`, `Floor`, `Door`, `Collectible` (Key, Bomb, ..), etc..

## Interaction model
- Composition: `Room` contains both `MapObject` instances (static/struct-like) and `Prop` instances (active/behavioral).
- Update order (per frame):
  1. Poll input (`Console`)
  2. `handle_tick` for every mapobject on the screen
  2. Players update (movement intents, collisions against `MapObject`)
  3. Props update (timers, state machines)
  5. Render pass (`GameView` -> `DrawingBuffer`)

This order keeps the separation of concerns intact: `MapObject` decides collision/placement, `Props` decide behavior, `Room` coordinates.

## File / Class map (focused)
- Scene / orchestration:
  - `Room.h` — composition root, event routing, room-level logic
  - `LevelParser.*` — instantiates `MapObject`s and `Prop`s into a `Room` from `.screen` files
- Behavioral Props:
  - `BaseProp.h`, `PlayersProp.*`, `DoorProp.*`, `TorchProp.*`, `Spring.*`, `Riddle.*`, `Collectible.*`
- Map objects / static types:
  - `Object.h`, `Object.cpp`, `ObjectTypes.cpp` — definitions and factory mapping for `MapObject` subclasses
- Messaging:
  - `Msg.h`, `Msg.cpp` — notification primitives used by `Room` and Props
- Rendering & input:
  - `GameView.*`, `DrawingBuffer.*`, `Console.*`

## Extending the system

- Add a new Prop (behavioral):
  1. Derive from `BaseProp` and implement any event handlers.
  2. Register the prop in `Room.h` or the parser.
  3. Add a call inside `Room.h` functions to the prop methods when needed (to be resolved with event handling)
  4. Add a test placement in an `adv-world_*.screen` file.

- Add a new MapObject subclass (representation/collision):
  1. Create a lightweight subclass undofer `MapObject` that defines placement and collision metadata.
  2. Add a symbol in `ObjTypes::ObjType`.
  3. Add factory mapping in `LevelParser.cpp` so `LevelParser` can instantiate it from `.screen`.
  4. Keep it free of complex behavioral logic — delegate that to a Prop if needed.

## Why this separation?
- Testability: Props can be unit-tested independently of room orchestration.
- Reuse: Same Prop can be reused in different rooms without duplicating room-specific code.
- Clear responsibilities: `MapObject` = data/placement, `Prop` = behavior, `Room` = orchestration & event routing.

## Where to look
- `Room.h` / `LevelParser.cpp` — how a room is composed from `.screen` files.
- Prop implementations (e.g., `TorchProp.cpp`, `Door.cpp`, `Spring.cpp`) — pattern examples for Prop behavior.
- `Msg.h` / `Msg.cpp` — event/messaging primitives used for decoupled interactions.

Build & run: open the solution in __Solution Explorer__ and use __Build > Build Solution__. Ensure MSVC toolset is correctly selected in project properties.

For diagrams or a small refactor that enforces stricter interfaces between `Room`, `Prop`, and `MapObject`, indicate the scope and I will generate class/sequence diagrams plus code changes. concise guide. For specific diagrams, sequence details, or a request to generate class/sequence diagrams, provide the desired scope and a diagram will be added.
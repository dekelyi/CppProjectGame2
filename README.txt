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

## Runners (input / timing abstractions)

The game uses a `GameRunner` abstraction that encapsulates input sourcing, tick timing, and optional recording/replay behavior.

Implemented runners (now split into dedicated headers):
- `KeyboardGameRunner.h` -- reads keypresses from the console and collects an internal steps buffer; supports saving steps.
- `SavingGameRunner.h` -- extends the keyboard runner and records keypresses and emitted events to files for later replay and verification.
- `LoadedThenExitGameRunner.h` -- replays keypresses from a steps file and exits when the file ends.
- `LoadedThenContinueGameRunner.h` -- replays keypresses from a file and then falls back to live keyboard input.
- `TestGameRunner.h` -- replays input and validates runtime events against a reference log; throws on mismatch.

These runner headers are included from `GameRunner.h` for convenience. Use whichever runner suits your needs by creating it in `main` and passing it to `GameView`.

Example usage:

- Run with recording enabled: `app.exe -save` (uses `SavingGameRunner`).
- Replay a recording and continue: `app.exe -load` (uses `LoadedThenContinueGameRunner`).
- Run tests in silent mode: `app.exe -load -silent` (uses `TestGameRunner`).
# CFB DS Game

## Objective

Build a college football DS game from scratch as a learning project. The goal is a playable, top-down game on the Nintendo DS using devkitPro/libnds where you directly control the player with the ball — not a play-selection/animation system.

Reference implementation in `../cfb-old/` — shelved, but the build setup, DS video modes, sprite handling, and toolchain config are directly reusable.

## How to Build

```
make        # builds cfb.nds
make clean  # removes build artifacts
```

Requires `DEVKITARM` set in your environment (devkitPro installation).

## Development Philosophy

**Seth writes the code.** This is a practice project — the goal is for Seth to author the game logic, not Claude.

Claude's role:
- Answer questions about libnds APIs, DS hardware, devkitPro toolchain
- Explain how things work in `cfb-old` if referenced
- Help debug compiler errors or build issues
- Suggest approaches when asked, but don't write the implementation unprompted
- Read devkitPro headers/examples when needed to answer questions accurately

Do **not** proactively generate game code, fill in functions, or add features unless explicitly asked.

## Project Structure

```
cfb/
├── Makefile              # root build — packages cfb.nds
├── CLAUDE.md             # this file
├── .claude/
│   └── settings.local.json
└── arm9/
    ├── Makefile          # ARM9 build (main game CPU)
    ├── graphics/         # PNG sprites (converted by grit at build time)
    └── source/
        ├── main.cpp      # entry point
        ├── screen.h      # shared VIEWPORT_WIDTH / VIEWPORT_HEIGHT constants (256×192)
        ├── field/        # Field class — geometry, game state, update(), draw()
        ├── football/     # Football class — flight arc, fumble state
        ├── physics/      # Physics namespace — move(), distance()
        ├── renderer/     # Renderer namespace — drawRect(), drawOffensePlayer(), drawDefensePlayer(), drawField()
        └── players/
            ├── player.h/cpp          # Base Player — position, speed, move(), goTo()
            ├── offense/
            │   ├── offensive_player  # OffensivePlayer — d-pad input, hasBall
            │   ├── quarterback/
            │   ├── running_back/
            │   ├── wide_receiver/
            │   ├── tight_end/
            │   └── offensive_line/
            └── defense/
                ├── defensive_player  # DefensivePlayer — runAI() stub, hasBall
                ├── cornerback/
                ├── linebacker/
                ├── safety/
                └── defensive_line/
```

The ARM7 CPU uses the pre-built `ds7_maine.elf` stub from devkitPro/calico (same as cfb-old), so there's no arm7/ source to maintain.

The ARM9 Makefile uses `find source -type d` to collect all source subdirectories automatically — no Makefile changes needed when adding new directories.

## Architecture

- **Field** — owns game state: `drawPosition`, `lineOfScrimmage`, `firstDown`, the 11-player offense/defense arrays, and a `Football*` pointer. Drives the game loop via `update()` (input + AI) and `draw()` (clear + render all). Static constants define field geometry (`DRAW_WIDTH`, `TOP`, `BOTTOM`, etc.). `PIXELS_PER_YARD` and `convertToPixelYards(float)` live in `utils.h`.
- **Renderer** — owns all colors as macros (`OFFENSE_COLOR`, `DEFENSE_COLOR`, field/line colors) and a static `backbuffer[VIEWPORT_WIDTH * VIEWPORT_HEIGHT]`. All `drawRect` calls write to the backbuffer; `flush()` copies it to `VRAM_A` via `dmaCopy` at the end of each frame. `drawField(scrollOffset, lineOfScrimmage, firstDown)` fills the backbuffer with `FIELD_COLOR`, draws endzones, scrolling 5-yard markers, and the two special lines. Sidelines are drawn by `Field::draw()` after `drawField()`, before `flush()`. Field constants are accessed via `field.h` included in `renderer.cpp` only — not in `renderer.h` — to avoid circular includes. Football is drawn directly in `Field::draw()` using `football->color` and `football->drawSize`.
- **Player** — base class with `move(direction)` (angle-based) and `goTo(x, y)`. No color field — colors are renderer concerns. OffensivePlayer has `hasBall` — when true, gates d-pad input in `runAI(Football*)`. DefensivePlayer has `hasBall` (default false) for fumble/interception possession; defense is never user-controlled.
- **Football** — HIDDEN/FLYING/FUMBLED state machine. FLYING animates a parabolic arc based on travel distance. `update()` computes `drawSize` each frame (used by `Field::draw()` for visual arc effect) but does not draw directly.
- **Scrolling** — `drawPosition` is derived from the ball carrier's field-space X, anchored so the player appears at `PLAYER_SCREEN_X` (1/4 screen width = 64px), clamped at field edges.

## Units

- **Pixel yards (px-yd)** — the standard unit for all field-space coordinates and distances. Conversion: `yards * PIXELS_PER_YARD`. Use `convertToPixelYards(float yards)` from `utils.h` to convert. Player positions, line of scrimmage, first down marker, and draw position are all stored in pixel yards.

## Game Design

- **Perspective**: top-down view of the field
- **Top screen**: game field (framebuffer mode, VRAM bank A)
- **Bottom screen**: debug console (`consoleDemoInit()`), MODE_5_2D
- **Control**: d-pad moves the ball carrier; A throws the ball
- **Field**: 120 yards total (10 endzone + 100 playing + 10 endzone), narrower than real life by design
- **Goal**: get the logic working first — real control, real movement, real defense AI — before worrying about presentation polish

## DS Basics (Quick Reference)

- **ARM9** — main CPU, runs game logic (this codebase)
- **ARM7** — auxiliary CPU, handles sound/touch; we use devkitPro's prebuilt stub
- **VBlank loop** — `swiWaitForVBlank()` syncs to 60fps display refresh
- **libnds** — the homebrew DS library; `#include <nds.h>` gets everything
- **Video modes** — configured via `videoSetMode()` / `videoSetModeSub()`
- **OAM** — Object Attribute Memory, controls hardware sprites
- **VRAM** — Video RAM banks (A–I), must be mapped before use
- **grit** — converts PNG graphics to DS sprite/tile data at build time
- **SCREEN_WIDTH / SCREEN_HEIGHT** — defined by libnds (`nds/arm9/video.h`); use `VIEWPORT_WIDTH` / `VIEWPORT_HEIGHT` from `screen.h` instead (same values, but safe for static const initializers)

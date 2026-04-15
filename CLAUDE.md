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
        ├── status_mixin.h  # StatusMixin base — statusFlags, setStatus/clearStatus/hasStatus/resetStatus
        ├── field/        # Field class — geometry, game state, update(), draw()
        ├── football/     # Football class — flight arc, fumble state
        ├── physics/      # Physics namespace — move(), distance()
        ├── renderer/     # Renderer namespace — drawRect(), drawOffensePlayer(), drawDefensePlayer(), drawField(), flush()
        ├── perf/         # Perf namespace — startFrame(), endFrame(), draw(); FPS/ms/heap stats on bottom screen
        ├── behaviors/
        │   ├── behavior.h            # Base Behavior struct — virtual update(Player*, GameContext&)
        │   ├── offensiveBehaviors/
        │   │   ├── ball_carrier      # BallCarrier — d-pad movement for any ball carrier
        │   │   ├── ballCarrier/
        │   │   │   ├── throwing_ball_carrier  # ThrowingBallCarrier : BallCarrier — throw logic
        │   │   │   └── running_ball_carrier   # RunningBallCarrier : BallCarrier — post-catch running; exposes runningBallCarrierBehavior singleton
        │   │   └── route_runner      # RouteRunner — runs route, catches ball, switches to RunningBallCarrier on catch
        │   └── defensiveBehaviors/
        │       └── blitz             # Blitz — chase ball carrier
        └── players/
            ├── player.h/cpp          # Base Player — pos, speed, catchRadius, behavior*, move(), goTo(), runAI()
            ├── offense/
            │   ├── offensive_player  # OffensivePlayer — runAI() delegates to Player base
            │   ├── quarterback/
            │   ├── running_back/
            │   ├── wide_receiver/
            │   ├── tight_end/
            │   └── offensive_line/
            └── defense/
                ├── defensive_player  # DefensivePlayer — runAI() delegates to Player base
                ├── cornerback/
                ├── linebacker/
                ├── safety/
                └── defensive_line/
```

The ARM7 CPU uses the pre-built `ds7_maine.elf` stub from devkitPro/calico (same as cfb-old), so there's no arm7/ source to maintain.

The ARM9 Makefile uses `find source -type d` to collect all source subdirectories automatically — no Makefile changes needed when adding new directories.

## Architecture

- **Field** (continued) — resolved `ballCarrier` each frame before AI runs. Drives the game loop via `update()` (input + AI) and `draw()` (clear + render all). Static constants define field geometry (`DRAW_WIDTH`, `TOP`, `BOTTOM`, etc.). `PIXELS_PER_YARD` and `convertToPixelYards(float)` live in `utils.h`.
- **Renderer** — owns all colors as macros (`OFFENSE_COLOR`, `DEFENSE_COLOR`, field/line colors); all color macros include `| BIT(15)` for the alpha bit required by `BgType_Bmp16`. Has a static `backbuffer[VIEWPORT_WIDTH * VIEWPORT_HEIGHT]` and a `bgGfxPtr` set via `Renderer::init(u16*)`. All `drawRect` calls write to the backbuffer; `flush()` copies it to `bgGfxPtr` via `dmaCopy` at the end of each frame. `drawField` clears the backbuffer with `dmaFillHalfWords`. Sidelines are drawn by `Field::draw()` after `drawField()`, before `flush()`. Field constants are accessed via `field.h` included in `renderer.cpp` only — not in `renderer.h` — to avoid circular includes. Football is drawn directly in `Field::draw()` using `football->color` and `football->drawSize`.
- **StatusMixin** — base class in `status_mixin.h` providing `uint16_t statusFlags` and templated `setStatus`/`clearStatus`/`hasStatus`/`resetStatus`. Inherited by `Player`, `Football`, and `Field`. Each class defines its own `Status` enum class; bits are logically independent per class.
- **Behavior** — base struct in `behaviors/behavior.h` with a single pure virtual `update(Player* self, const GameContext& ctx)`. Concrete behaviors live under `behaviors/offensiveBehaviors/` and `behaviors/defensiveBehaviors/`. Stateless behaviors are exposed as singletons (e.g. `runningBallCarrierBehavior`). `Field` assigns behaviors to players at snap; behaviors can reassign `self->behavior` mid-play (e.g. `RouteRunner` switches to `RunningBallCarrier` on catch).
- **Player** — inherits `StatusMixin`. Has `Vector2 pos`, `float catchRadius`, `Behavior* behavior`, `move(direction)` (angle-based), `goTo(Vector2)`. `runAI()` handles fumble pickup first (early return), then delegates to `behavior->update(this, ctx)` if a behavior is assigned. `isOffense` and `position` (a `Position` enum) are set at construction. Ball possession tracked via `Player::Status::BALL_CARRIER` (bit 0).
- **WideReceiver** — `catchRadius` is now on `Player` base (passed up via `OffensivePlayer` constructor). No `runAI` override — behavior is assigned by `Field` at snap.
- **Football** — inherits `StatusMixin`. `Football::Status`: `HIDDEN` (bit 0), `FLYING` (bit 1), `FUMBLED` (bit 2) — mutually exclusive, set one at a time. Uses `Vector2 pos`, `start`, and `destination`. FLYING animates a parabolic arc based on travel distance. `update()` computes `drawSize` each frame (used by `Field::draw()` for visual arc effect) but does not draw directly.
- **Field** — inherits `StatusMixin`. `Field::Status`: `PRESNAP` (bit 0), `IN_PLAY` (bit 1). Owns game state: `drawPosition`, `lineOfScrimmage`, `firstDown`, the 11-player offense/defense arrays, a `Football*` pointer, a `Player* ballCarrier`, an `OffensivePlayContext offensePlay`, and `u16* buttonGfxPtrs[5]`. Allocates OAM sprite gfx and loads button tile/palette data in the constructor. `draw()` calls `oamSet`/`oamUpdate` to position button label sprites above each pass catcher before `flush()`. In `update()`, `football->update()` is called before player AI runs. `endPlay()` clears `BALL_CARRIER` status on all offense and defense players. At snap (KEY_L), assigns static behavior instances: QB → `ThrowingBallCarrier`, other offense → `RouteRunner`, defense → `Blitz`.
- **Scrolling** — `drawPosition` is derived from the ball carrier's field-space X, anchored so the player appears at `PLAYER_SCREEN_X` (1/4 screen width = 64px), clamped at field edges.
- **Vector2** — `struct Vector2 { float x, y; }` defined in `utils.h` alongside `angleTo(a, b)` and `distanceTo(a, b)` free functions. Used for all player and football positions.

## Units

- **Pixel yards (px-yd)** — the standard unit for all field-space coordinates and distances. Conversion: `yards * PIXELS_PER_YARD`. Use `convertToPixelYards(float yards)` from `utils.h` to convert. Player positions, line of scrimmage, first down marker, and draw position are all stored in pixel yards.

## Game Design

- **Perspective**: top-down view of the field
- **Top screen**: game field (MODE_5_2D, BG2 16-bit bitmap on VRAM bank A, OAM sprites on VRAM bank B)
- **Bottom screen**: debug console (`consoleDemoInit()`), MODE_5_2D
- **Control**: d-pad moves the ball carrier; L snaps the ball; A/B/X/R throws to the assigned receiver (QB leads the pass using interceptPoint on the receiver's route)
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

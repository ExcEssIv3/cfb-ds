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
        ├── field_geometry.h  # global field constants — TOP, BOTTOM, DRAW_WIDTH, DRAW_HEIGHT, END_ZONE_PX, PLAYER_SCREEN_X
        ├── status_mixin.h  # StatusMixin base — statusFlags, setStatus/clearStatus/hasStatus/resetStatus
        ├── field/        # Field class — game state coordinator, update(), draw(), endPlay()
        ├── football/     # Football class — flight arc, fumble state
        ├── physics/      # Physics namespace — move(), distance()
        ├── renderer/     # Renderer namespace — drawRect(), drawOffensePlayer(), drawDefensePlayer(), drawField(), flush()
        ├── perf/         # Perf namespace — startFrame(), endFrame(), draw(); FPS/ms/heap stats on bottom screen
        ├── roster/       # Roster class — owns offense[]/defense[] Player arrays, construction, reset()
        ├── play_context/ # PlayContext class — offensePlay, button sprites, snap(), draw(), reset()
        ├── contact/      # Contact namespace — tackle() probability calculation and resolution
        ├── behaviors/
        │   ├── behavior.h            # Base Behavior struct — virtual update(Player*, GameContext&)
        │   ├── offensive_behaviors/
        │   │   ├── ball_carrier      # BallCarrier — d-pad movement for any ball carrier
        │   │   ├── ball_carrier/
        │   │   │   ├── throwing_ball_carrier  # ThrowingBallCarrier : BallCarrier — throw logic
        │   │   │   └── running_ball_carrier   # RunningBallCarrier : BallCarrier — post-catch running; exposes runningBallCarrierBehavior singleton
        │   │   └── route_runner      # RouteRunner — runs route, catches ball, switches to RunningBallCarrier on catch
        │   └── defensive_behaviors/
        │       └── blitz             # Blitz — chase ball carrier
        └── players/
            ├── player_stats.h        # PlayerStats — width, height, acceleration, topSpeed, catchRadius, weight, breakTackle, tackle (all default 0)
            └── player.h/cpp          # Player — pos, velocity, isOffense, position, stats (PlayerStats), behavior*, accelerate(), decelerate(), move(), goTo(), runAI()
```

The ARM7 CPU uses the pre-built `ds7_maine.elf` stub from devkitPro/calico (same as cfb-old), so there's no arm7/ source to maintain.

The ARM9 Makefile uses `find source -type d` to collect all source subdirectories automatically — no Makefile changes needed when adding new directories.

## Architecture

- **Field** — inherits `StatusMixin`. `Field::Status`: `PRESNAP` (bit 0), `IN_PLAY` (bit 1). Game state coordinator: owns `drawPosition`, `lineOfScrimmage`, `firstDown`, `Football*`, `Player* ballCarrier`, `Roster*`, and `PlayContext*`. `update()` resolves `ballCarrier`, runs input/AI, detects end-play conditions. `draw()` calls Renderer, iterates roster players, draws football and sidelines, delegates button HUD to `playContext->draw()`. `endPlay()` updates down/distance then delegates to `playContext->reset()`.
- **Roster** — owns `offense[PLAYER_COUNT]` and `defense[PLAYER_COUNT]` Player pointer arrays. Constructs all players with position-appropriate `PlayerStats` in its constructor. `endPlay(lineOfScrimmage, octx, dctx)` repositions players and resets `BALL_CARRIER` status between plays.
- **PlayContext** — owns `OffensivePlayContext offensePlay`, `DefensivePlayContext defensePlay`, and `u16* buttonGfxPtrs[5]`. Constructor takes `Roster&`, builds pass catcher assignments, allocates OAM gfx, and loads button tile/palette data. `snap(Roster&)` assigns static behavior instances at snap (QB → `ThrowingBallCarrier`, others → `RouteRunner`, defense → `Blitz`). `draw(scrollOffset)` renders button label sprites via OAM. `reset(Roster&, lineOfScrimmage)` delegates to `roster.endPlay()`.
- **field_geometry.h** — global constants: `TOP`, `BOTTOM`, `DRAW_WIDTH`, `DRAW_HEIGHT`, `END_ZONE_PX`, `PLAYER_SCREEN_X`. Included by `field.h`, `roster.h`, and any other subsystem that needs field dimensions without depending on `Field`.
- **Renderer** — owns all colors as macros (`OFFENSE_COLOR`, `DEFENSE_COLOR`, field/line colors); all color macros include `| BIT(15)` for the alpha bit required by `BgType_Bmp16`. Has a static `backbuffer[VIEWPORT_WIDTH * VIEWPORT_HEIGHT]` and a `bgGfxPtr` set via `Renderer::init(u16*)`. All `drawRect` calls write to the backbuffer; `flush()` copies it to `bgGfxPtr` via `dmaCopy` at the end of each frame. `drawField` clears the backbuffer with `dmaFillHalfWords`. Sidelines are drawn by `Field::draw()` after `drawField()`, before `flush()`. Football is drawn directly in `Field::draw()` using `football->color` and `football->drawSize`.
- **StatusMixin** — base class in `status_mixin.h` providing `uint16_t statusFlags` and templated `setStatus`/`clearStatus`/`hasStatus`/`resetStatus`. Inherited by `Player`, `Football`, and `Field`. Each class defines its own `Status` enum class; bits are logically independent per class.
- **Behavior** — base struct in `behaviors/behavior.h` with a single pure virtual `update(Player* self, const GameContext& ctx)`. Concrete behaviors live under `behaviors/offensive_behaviors/` and `behaviors/defensive_behaviors/`. Stateless behaviors are exposed as singletons (e.g. `runningBallCarrierBehavior`). `PlayContext` assigns behaviors to players at snap; behaviors can reassign `self->behavior` mid-play (e.g. `RouteRunner` switches to `RunningBallCarrier` on catch).
- **Player** — inherits `StatusMixin`. Has `Vector2 pos`, `Vector2 velocity`, `bool isOffense`, `Position position`, `PlayerStats stats`, `Behavior* behavior`, `int stumbleFrames`. Movement: `accelerate(direction)` steps velocity toward `topSpeed` in the given direction by `acceleration` per frame; `decelerate()` steps velocity toward zero; `move()` applies velocity to pos (`pos += velocity`). `goTo(Vector2)` uses braking distance (`v²/2a`) to decide whether to accelerate or decelerate, snapping when within `acceleration` of target. `runAI()` checks `STUMBLED` first (decrements `stumbleFrames`, clears status at 0, early return), then handles fumble pickup, then delegates to `behavior->update(this, ctx)`, then calls `move()`. `detectCollision(colliderPos, width, height)` does AABB overlap between self's box and the collider's box. `Player::Status`: `BALL_CARRIER` (bit 0), `STUMBLED` (bit 1). No subclasses — all players are instantiated directly as `Player` with the appropriate `isOffense`, `Position`, and `PlayerStats`. `Roster` holds separate `Player* offense[]` and `Player* defense[]` arrays; `PassCatcher::player` is also `Player*`.
- **PlayerStats** — plain struct in `player_stats.h` holding `width`, `height` (pixels), `acceleration`, `topSpeed`, `catchRadius`, `weight`, `breakTackle`, `tackle` (all default 0). Passed by value to the `Player` constructor. All players carry all stats; position-appropriate defaults are set at construction in `Roster`.
- **Contact** — namespace in `contact/contact.h/cpp`. `tackle(tackler, tacklee)` computes tackle success probability from stat differentials (`tackle` vs `breakTackle`, weight delta, relative velocity) and rolls `rand()` against it. Called by `Field::update()` on AABB collision; on failure the tackler gets `STUMBLED` for 60 frames. Constants `breakTackleWeight`, `weightDeltaWeight`, `speedWeight` are `constexpr float` in the header.
- **Football** — inherits `StatusMixin`. `Football::Status`: `HIDDEN` (bit 0), `FLYING` (bit 1), `FUMBLED` (bit 2) — mutually exclusive, set one at a time. Uses `Vector2 pos`, `start`, and `destination`. FLYING animates a parabolic arc based on travel distance. `update()` computes `drawSize` each frame (used by `Field::draw()` for visual arc effect) but does not draw directly.
- **Scrolling** — `drawPosition` is derived from the ball carrier's field-space X, anchored so the player appears at `PLAYER_SCREEN_X` (1/4 screen width = 64px), clamped at field edges.
- **Vector2** — `struct Vector2 { float x, y; }` defined in `utils.h` alongside `angleTo(a, b)`, `distanceTo(a, b)`, `magnitude(v)`, and `dotProduct(a, b)` free functions. Used for all player and football positions.

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

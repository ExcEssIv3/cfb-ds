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
        └── main.c        # entry point — start here
```

The ARM7 CPU uses the pre-built `ds7_maine.elf` stub from devkitPro/calico (same as cfb-old), so there's no arm7/ source to maintain.

## Game Design

- **Perspective**: top-down view of the field
- **Control**: player directly controls the ball carrier using the d-pad/buttons
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

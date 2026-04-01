# CFB DS

A college football game for the Nintendo DS, built with devkitPro/libnds.

Top-down, player-controlled gameplay — you directly control the ball carrier, not a play-selection system.

## Requirements

- [devkitPro](https://devkitpro.org/) with devkitARM and libnds

Make sure `DEVKITARM` is set in your environment:
```sh
export DEVKITARM=/opt/devkitpro/devkitARM
```

## Build

```sh
make        # produces cfb.nds
make clean  # removes build artifacts
```

Run `cfb.nds` in a DS emulator (e.g. [melonDS](https://melonds.kuribo64.net/)) or on real hardware via a flashcart.

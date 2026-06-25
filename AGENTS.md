# AGENTS.md — Super Mario War

## Project overview

Super Mario War is a multiplayer Mario-battle fan game (C++, GPLv2). Version 1.8 beta, originally by Florian Hufsky, continued by Two52.

- **Entry points**: _src/main.cpp (game), _src/leveleditor.cpp, _src/worldeditor.cpp
- **Central header**: _src/global.h — included by nearly every source file. Contains all constants, game state (`gv game_values`), global sprite declarations, and most other header includes.
- **Docs**: README.html (player manual), _src/src_how the thing works.txt (code architecture overview)

## Build

### Unix (Linux / macOS / MinGW)
```sh
./configure          # generates 'configuration' file included by Makefile
make                 # builds smw, leveledit, worldedit
```
- `./configure --debug` adds -D_DEBUG and debug symbols.
- `./configure --no-png-save` disables PNG screenshot support.
- Output binaries: smw, leveledit, worldedit.
- Object files land in build/ (must exist — configure creates it).

### Windows (Visual Studio)
- Solution: Projects/PC/SuperMarioWar.sln (VS 2005/2008 format, .vcproj files).
- Projects: SuperMarioWar, Leveleditor, Worldeditor, SuperMarioWar_console.

### macOS (Xcode)
- Project: Projects/MacOSX/Super Mario War.xcodeproj/.

### Dependencies
- SDL 1.2, SDL_image, SDL_mixer, SDL_net, libpng.

### Release packaging
- PrepareForRelease.bat strips dev-only dirs (_src/, Projects/, Scripts/, debian/, macosx/, zzz_test/) for distribution.

## Architecture warnings

### DON'T reorder PGFX_* constants in _src/global.h
```cpp
#define PGFX_STANDING_R 0   // must be even
#define PGFX_STANDING_L 1   // must be odd
#define PGFX_RUNNING_R  2
#define PGFX_RUNNING_L  3
// ... alternating right/left ...
```
The game checks `sprite_idx & 0x1` to determine the last-facing direction. Reordering breaks direction tracking.

### One .c file in a .cpp project
_src/SFont.c compiles with `$(CC)`, not `$(CXX)`. If adding a build system, keep this distinction.

### Heavy global state
Nearly all game state lives in the global `gv game_values` struct (declared in global.h). There is no dependency injection, service locator, or module boundaries. Changing headers triggers large-scale recompilation — global.h touches ~30 other headers.

### Map format is binary
Map files (.map) have a fixed binary layout. Map loading/saving in _src/map.cpp. Don't hand-edit these files.

## Runtime resources

Asset directories the game reads at runtime:
- gfx/ — sprites, tilesets, backgrounds, UI graphics
- maps/ — map files, plus maps/cache/ and maps/screenshots/
- worlds/ — world definitions
- music/ — background music tracks
- sfx/ — sound effects
- tours/ — tour configurations
- filters/ — map filter definitions

Settings are persisted in options.bin (binary format, recreated on next run if deleted).

## No test infrastructure

There are no automated tests, no CI, no linters, no formatters. The only way to verify changes is to compile and run the game. The Scripts/ directory contains menu navigation scripts for automated smoke-testing, not unit tests.

## Code conventions

- Physics constants are in global.h (velocities, gravity, tile sizes, etc.).
- TILESIZE = 32 and TILESIZE is often replaced by << 5 for performance.
- Naming: classes prefixed with C (e.g. CPlayer, CMap, CGameMode).
- Two collision-detection passes per frame: player-vs-map then object-vs-object.
- The eye-candy system (non-collidable visual effects) is documented in _src/src_how the thing works.txt.
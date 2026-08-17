# SMW 2.0 — Session state (2026-07-05)

## Faza 1.1-1.2 — CMake + SDL2 migration: DONE 

### Build system
- CMakeLists.txt, 3 targets: smw, leveledit, worldedit
- MinGW64 (MSYS2) toolchain, GCC 16.1.0
- Linker fix: imported targets (SDL2::SDL2, SDL2_image::SDL2_image, ...) + SDL2::SDL2main on Windows

### SDL 1.2 → 2.0 API migration (completed)
17 files modified: window, surface, rendering pipeline.
Keycode types (short → SDL_Keycode), Keynames array removed,
SDL2 API fixes (const Uint8* keystate, IMG_SavePNG_RW, etc.)

### Compilation: VERIFIED 
build/smw.exe, build/leveledit.exe, build/worldedit.exe — all build and run.

## Remaining tasks

### Faza 1.3 — Case-insensitive file extensions: DONE  (2026-06-30)
Fix: _src/dirlist.cpp endsWith() — replaced case-sensitive substr == q
with tolower() char-by-char loop. Added #include <cctype>, const string& params.

### Faza 1.4 — Empty music/ crash: DONE ✅ (2026-06-30)
4 files: sfx.cpp (+NULL guard w play() + Mix_HookMusicFinished unhook), FileList.cpp (+empty guards + exit(0)→warning), FileList.h (+GetCount + guards), main.cpp (+isready checks + auto-disable).
7 steps, build OK.

### Faza 1.5 — Skin memory crash (0xbaadfood): DONE ✅ (2026-07-05)
gfx.cpp — 3 null guards in gfxSprite::draw/drawStretch

### Faza 1.6 — Invincibility music resume after pause: DONE ✅ (2026-07-05)
sfx.h/cpp + main.cpp — pause()/resume() methods, invincible music save/restore

### Faza 1.7 — Goomba sprite flipping: DONE ✅ (2026-07-05)
objectgame.cpp — MO_Goomba::draw() uses velx direction for frozen sprite frame

### Faza 1.8 — Remove hardcoded splash/loading graphics from code: DONE ✅ (2026-07-05)
The game printed loading messages for embedded splash screens (splash_72dpi.png, splash_contest_winners.png, etc.).
All debug-only printfs wrapped in `#ifdef _DEBUG` — 14 total across 3 files:
- **gfx.cpp** (7 couts) — splash/loading texture prints
- **sfx.cpp** (2 couts) — audio init/debug prints
- **map.cpp** (5 couts) — map loading filename prints
Release builds now produce zero console noise during startup.

### Low-priority cleanup: DONE ✅ (2026-07-05) — stale comment removed, info_ptr leak fixed, 3 short→int narrowing in leveleditor/worldeditor. Xbox #ifdef _XBOX deferred.

## Full plan document
See the comprehensive plan produced by the plan agent (5 phases, ~17-25 weeks total).

## Build instructions
```powershell
$env:Path = "C:\msys64\mingw64\bin;C:\msys64\usr\bin;$env:Path"
$env:CMAKE_PREFIX_PATH = "C:\msys64\mingw64"
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM="C:/msys64/mingw64/bin/mingw32-make.exe"
cmake --build build
```

## Run
```powershell
.\run_smw.bat
```
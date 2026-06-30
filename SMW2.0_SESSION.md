# SMW 2.0 — Session state (2026-06-30)

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

### Faza 1.5 — Skin memory crash (0xbaadfood)
File: gfx.cpp (gfxSprite::draw — add if(!m_picture) return false;)

### Faza 1.6 — Invincibility music resume after pause
Files: sfx.cpp, main.cpp (pause logic)
Track invincible music playing state, resume on unpause.

### Faza 1.7 — Goomba sprite flipping
File: objectgame.cpp (MO_Goomba::draw, add fFacingRight with SDL_RendererFlip)

### Low-priority cleanup
- _src/global.cpp:68 — stale comment about SDL_GetKeyName() (we now use it)
- _src/savepng.cpp:147 — pre-existing info_ptr leak in png_destroy_write_struct
- _src/leveleditor.cpp, worldeditor.cpp — remaining short→SDL_Keycode narrowing
- Xbox #ifdef _XBOX blocks in gfx.cpp — broken for SDL2, deferred

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
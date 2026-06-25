# SMW 2.0 — Session state (2026-06-25)

## What was done

### Faza 1.1 — CMake build system
- Created CMakeLists.txt (SDL2-based, 3 targets: smw, leveledit, worldedit)
- SFont.c set as C source, linfunc.cpp conditionally on Unix, SDLMain.m for macOS
- Replaces old Makefile + configure + .vcproj

### Faza 1.2 — SDL 1.2 → SDL 2 migration
17 files modified with these API replacements:
- SDL_SetVideoMode → SDL_CreateWindow + SDL_GetWindowSurface
- SDL_Flip → SDL_UpdateWindowSurface (38 sites)
- SDL_WM_SetCaption → SDL_SetWindowTitle
- SDL_FULLSCREEN → SDL_WINDOW_FULLSCREEN_DESKTOP
- SDL_SoftStretch → SDL_BlitScaled
- SDL_DisplayFormat → SDL_ConvertSurfaceFormat
- SDL_DisplayFormatAlpha → same with ARGB8888
- SDL_SetColorKey: removed SDL_SRCCOLORKEY|SDL_RLEACCEL flags
- SDL_SetAlpha → SDL_SetSurfaceAlphaMod + SDL_SetSurfaceBlendMode
- SDLKey → SDL_Keycode
- SDL_GetKeyState → SDL_GetKeyboardState (with scancode indices)
- SDL_JoystickName → SDL_JoystickNameForIndex
- SDL_EnableKeyRepeat removed (SDL2 default)
- All #pragma comment(lib, SDL_*) removed

Added globals:
- SDL_Window *g_window (gfx.h/gfx.cpp)
- gfx_close() function

Windows Xbox #ifdef _XBOX blocks left untouched.

CMakeLists.txt also updated: SDL → SDL2 find_package, variables, removed MinGW -Dmain=SDL_main.

### NOT compiled — no SDL2 dev libs in this environment
Next step: install SDL2, SDL2_image, SDL2_mixer, SDL2_net dev packages and run:
  cmake -S . -B build
  cmake --build build

## Remaining tasks

### Faza 1.3 — Case-insensitive file extensions
Files: global.cpp (convertPath), gfx.cpp (IMG_Load helpers)

### Faza 1.4 — Empty music/ crash
Files: FileList.cpp, main.cpp (musicfinished, PlayNextMusicTrack)
Guard with isready() checks, auto-disable music if no files.

### Faza 1.5 — Skin memory crash (0xbaadfood)
File: gfx.cpp (gfxSprite::draw — add if(!m_picture) return false;)

### Faza 1.6 — Invincibility music resume after pause
Files: sfx.cpp, main.cpp (pause logic)
Track invincible music playing state, resume on unpause.

### Faza 1.7 — Goomba sprite flipping
File: objectgame.cpp (MO_Goomba::draw, add fFacingRight with SDL_RendererFlip)

## Full plan document
See the comprehensive plan produced by the plan agent (5 phases, ~17-25 weeks total).

## Architecture notes (from AGENTS.md)
- DON'T reorder PGFX_* constants in global.h (alternating R/L, checked with sprite_idx & 0x1)
- SFont.c is the only C file — must compile with C compiler
- Heavy global state in gv game_values struct
- Map format is binary (.map), don't hand-edit
- No tests — verify by compiling and running
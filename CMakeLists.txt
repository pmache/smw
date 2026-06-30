# CMakeLists.txt for Super Mario War
#
# Builds three executables:
#   smw        - the game
#   leveledit  - the map editor
#   worldedit  - the world editor
#
# This replaces the old Makefile + configure / VS .vcproj / Xcode setups.
# It targets SDL 2: CMake's FindSDL2* modules locate SDL 2 and its add-on
# libraries (SDL2_image, SDL2_mixer, SDL2_net).
#
# Only the essential build is replicated; the finer details of the old
# configure script (debug toggles, sdkroot, universal binaries, install
# rules, .app bundling) are intentionally omitted.

cmake_minimum_required(VERSION 3.16)

project(SuperMarioWar LANGUAGES C CXX)

# OBJC is only needed on macOS for macosx/SDLMain.m (the Cocoa entry point).
if(APPLE)
    enable_language(OBJC)
endif()

#------------------------------------------------------------------------------
# C++ standard
#
# The codebase is old C++98; it also compiles under C++11.  C++98 is the
# faithful target.  CMAKE_CXX_EXTENSIONS is left at its default (ON) so that
# gnu++98 is used on GCC/Clang, which is more permissive for legacy code.
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#------------------------------------------------------------------------------
# Dependencies
#------------------------------------------------------------------------------
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_net REQUIRED)
find_package(PNG REQUIRED)

#------------------------------------------------------------------------------
# Include directories
#
# The sources use SDL 1.2 style includes, e.g. #include "SDL.h",
# "SDL_image.h", "SDL_mixer.h", "SDL_net.h", "SDL_endian.h".
# The SDL2 include directories are added below so those same header names
# resolve to the SDL2 headers.
#------------------------------------------------------------------------------
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/_src
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${SDL2_NET_INCLUDE_DIR}
    ${PNG_INCLUDE_DIRS}
)

#------------------------------------------------------------------------------
# Compile definitions
#
# PNG_SAVE_FORMAT is enabled by default in the original configure script and
# is required for _src/savepng.cpp to emit any code (the whole file is
# wrapped in #ifdef PNG_SAVE_FORMAT).
#------------------------------------------------------------------------------
add_compile_definitions(PNG_SAVE_FORMAT)

if(APPLE)
    # macOS: -D__MACOSX__ -DLINUXFUNC -DPREFIXPATH=""  (empty data root)
    # The backslash-escaped quotes preserve a string-literal value through
    # CMake's argument lexer (a bare " would be stripped).
    add_compile_definitions(__MACOSX__ LINUXFUNC PREFIXPATH=\"\")
elseif(UNIX AND NOT APPLE)
    # Linux: -DLINUXFUNC -DPREFIXPATH="/usr/share/games/smw"
    add_compile_definitions(LINUXFUNC PREFIXPATH=\"/usr/share/games/smw\")
endif()

#------------------------------------------------------------------------------
# Common sources (compiled once, linked into all three executables).
# Mirrors COMMON_OBJS in the original Makefile.
#
# NOTE: _src/net.cpp is intentionally NOT built.  It is real, uncommented
# networking code but does not include net.h / SDL_net.h (and global.h has
# "#include net.h" commented out), so it does not compile as-is.  The
# original Makefile likewise excludes it from every target.  SDL_net is
# still linked per the task requirements.
#------------------------------------------------------------------------------
set(SMW_COMMON_SOURCES
    _src/FileIO.cpp
    _src/FileList.cpp
    _src/MapList.cpp
    _src/SFont.c
    _src/TilesetManager.cpp
    _src/dirlist.cpp
    _src/eyecandy.cpp
    _src/gfx.cpp
    _src/global.cpp
    _src/input.cpp
    _src/map.cpp
    _src/movingplatform.cpp
    _src/path.cpp
    _src/savepng.cpp
    _src/sfx.cpp
)

# SFont.c is the only C file in this C++ project -- compile it with the C
# compiler.  Its declarations are wrapped in extern "C" in SFont.h.
set_source_files_properties(_src/SFont.c PROPERTIES LANGUAGE C)

# linfunc.cpp is built only when LINUXFUNC is defined (Linux + macOS).
if(UNIX AND NOT WIN32)
    list(APPEND SMW_COMMON_SOURCES _src/linfunc.cpp)
endif()

# macOS: SDLMain.m provides the Cocoa application entry point.
if(APPLE)
    list(APPEND SMW_COMMON_SOURCES macosx/SDLMain.m)
    set_source_files_properties(macosx/SDLMain.m PROPERTIES LANGUAGE OBJC)
endif()

# Object library: compiled once, its objects are pulled into each executable.
add_library(smw_common OBJECT ${SMW_COMMON_SOURCES})

#------------------------------------------------------------------------------
# Shared link line for every executable
#------------------------------------------------------------------------------
set(SMW_LINK_LIBS
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_net::SDL2_net
    ${PNG_LIBRARIES}
)

# On Windows, SDL2main provides the WinMain entry point that calls our main().
# It must link BEFORE SDL2::SDL2: SDL2main is a static archive that calls SDL
# functions (SDL_strlen, SDL_SetMainReady, ...), and MinGW ld is single-pass
# left-to-right, so a library can only satisfy references from libraries listed
# after it.
if(WIN32)
    list(INSERT SMW_LINK_LIBS 0 SDL2::SDL2main)
endif()

#------------------------------------------------------------------------------
# smw -- the game
# Mirrors SMW_OBJS in the original Makefile.
#------------------------------------------------------------------------------
set(SMW_GAME_SOURCES
    _src/HashTable.cpp
    _src/ai.cpp
    _src/gamemodes.cpp
    _src/main.cpp
    _src/menu.cpp
    _src/modeoptionsmenu.cpp
    _src/object.cpp
    _src/objectgame.cpp
    _src/objecthazard.cpp
    _src/player.cpp
    _src/splash.cpp
    _src/uicontrol.cpp
    _src/uicustomcontrol.cpp
    _src/uimenu.cpp
    _src/world.cpp
)

add_executable(smw ${SMW_GAME_SOURCES} $<TARGET_OBJECTS:smw_common>)
target_link_libraries(smw ${SMW_LINK_LIBS})

#------------------------------------------------------------------------------
# leveledit -- the map editor
# Mirrors LEVELEDIT_OBJS in the original Makefile (just leveleditor.cpp).
#------------------------------------------------------------------------------
add_executable(leveledit _src/leveleditor.cpp $<TARGET_OBJECTS:smw_common>)
target_link_libraries(leveledit ${SMW_LINK_LIBS})

#------------------------------------------------------------------------------
# worldedit -- the world editor
# Mirrors WORLDEDIT_OBJS in the original Makefile.
#------------------------------------------------------------------------------
set(SMW_WORLDEDIT_SOURCES
    _src/modeoptionsmenu.cpp
    _src/object.cpp
    _src/objecthazard.cpp
    _src/uicontrol.cpp
    _src/uimenu.cpp
    _src/world.cpp
    _src/worldeditor.cpp
)

add_executable(worldedit ${SMW_WORLDEDIT_SOURCES} $<TARGET_OBJECTS:smw_common>)
target_link_libraries(worldedit ${SMW_LINK_LIBS})

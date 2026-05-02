@echo off
:: ============================================================
::  Build script for Pac-Man on Windows
::  Requires: raylib installed at C:\raylib
::  and MinGW (g++) in PATH
::
::  Now builds multiple .cpp files instead of one big file.
:: ============================================================

set RAYLIB=C:\raylib
set OUT=pacman.exe
set FLAGS=-std=c++17 -O2 -Wall

echo Building Pac-Man (multi-file)...

g++ %FLAGS% -o %OUT% ^
    Maze.cpp ^
    Entity.cpp ^
    Pathfinding.cpp ^
    Game.cpp ^
    Renderer.cpp ^
    main.cpp ^
    -I%RAYLIB%\include ^
    -L%RAYLIB%\lib ^
    -lraylib -lopengl32 -lgdi32 -lwinmm

if %ERRORLEVEL% == 0 (
    echo.
    echo  Build successful!  Run:  pacman.exe
) else (
    echo.
    echo  Build FAILED. Check that raylib is at %RAYLIB%
    echo  and that g++ is installed and in your PATH.
)
pause

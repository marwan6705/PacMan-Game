#pragma once
//  Central Game state struct and lifecycle functions.
// Data Structures Used: Stack (Move History), Queue (BFS AI), List (Leaderboard).

#include "raylib.h"
#include "Constants.h"
#include "Entity.h"
#include "Stack.h"
#include "List.h"

// UI Screen States
enum Screen {
    SCR_MENU,      // Main menu
    SCR_GAME,      // Active gameplay
    SCR_GAMEOVER,  // Lost all lives
    SCR_WIN        // Cleared all dots
};

// Main Game State
struct Game {
    // Maze Details
    int  maze[ROWS][COLS];  // Working copy
    int  totalDots;
    int  dotsEaten;

    // Player (Pac-Man)
    Entity    pac;
    Direction pacWant;      // Buffered input direction

    // Enemies
    Ghost ghosts[4];

    // Stats
    int  lives;
    int  score;
    int  level;

    // Power-up
    bool powerActive;       // Scared mode flag
    int  powerTimer;        // Frames remaining

    // Rendering & Timing
    Screen screen;
    bool   paused;
    int    frame;           // Animation frame counter

    // Animation
    float  mouth;           // Mouth angle
    bool   mouthOpening;

    // Data Structures
    Stack  moveHistory;     // Tracks visited cells
    List   leaderboard;     // Sorted scores

    // Player Info
    char   playerName[20];
    int    nameLen;
};

// Core Functions

// Initialize entire game state
void initGame(Game& G);

// Reset map. fullReset=true -> wipes score/lives for new game
void resetLevel(Game& G, bool fullReset);

// Main game loop logic (movement, collisions, scoring)
void updateGame(Game& G);
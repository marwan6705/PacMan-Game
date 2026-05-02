#pragma once
// ============================================================
//  Game.h
//  The central Game state struct and the functions that
//  initialise and update it every frame.
//
//  Data Structures summary:
//    Stack (Lec 2 & 3) – moveHistory: records every cell
//                         Pac-Man steps on (a trail of breadcrumbs)
//    Queue (Lec 4)     – used inside BFS (see Pathfinding.cpp)
//    List  (Lec 5)     – leaderboard: top-10 scores kept sorted
// ============================================================

#include "raylib.h"
#include "Constants.h"
#include "Entity.h"
#include "Stack.h"   // Lecture 2 & 3 – move history
#include "List.h"    // Lecture 5     – leaderboard

// ── Which screen are we showing? ────────────────────────────
enum Screen {
    SCR_MENU,      // main menu / name entry
    SCR_GAME,      // active gameplay
    SCR_GAMEOVER,  // player lost all lives
    SCR_WIN        // player cleared all dots (unused path in current code)
};

// ── The whole game state in one struct ──────────────────────
struct Game {
    // ── Maze ────────────────────────────────────────────────
    int  maze[ROWS][COLS];  // working copy (dots get eaten here)
    int  totalDots;         // how many dots existed at level start
    int  dotsEaten;         // how many dots have been collected so far

    // ── Pac-Man ─────────────────────────────────────────────
    Entity    pac;          // position + movement state
    Direction pacWant;      // the direction the player WANTS to go next
                            //  (buffered so you can press a key just before a corner)

    // ── Ghosts ──────────────────────────────────────────────
    Ghost ghosts[4];        // four ghosts: Red, Pink, Cyan, Orange

    // ── Game progress ────────────────────────────────────────
    int  lives;
    int  score;
    int  level;

    // ── Power-up state ───────────────────────────────────────
    bool powerActive;       // true while ghosts are scared
    int  powerTimer;        // frames remaining

    // ── Screen / frame ───────────────────────────────────────
    Screen screen;
    bool   paused;
    int    frame;           // increments every update – used for animations

    // ── Pac-Man mouth animation ──────────────────────────────
    float  mouth;           // current opening angle in degrees (each side)
    bool   mouthOpening;    // true = opening, false = closing

    // ── Data structures (graded) ─────────────────────────────
    Stack  moveHistory;     // Stack  – cell trail (Lec 2 & 3)
    List   leaderboard;     // List   – sorted top scores (Lec 5)

    // ── Player name ──────────────────────────────────────────
    char   playerName[20];
    int    nameLen;
};

// ── Game lifecycle functions ─────────────────────────────────

// Full first-time setup: zeroes everything, inits data structures.
void initGame(Game& G);

// Reset to the start of a level.
// fullReset=true  → also reset lives/score/level (used on new game)
// fullReset=false → keep score/lives/level    (used when clearing a level)
void resetLevel(Game& G, bool fullReset);

// Called once per frame while SCR_GAME and not paused.
// Moves Pac-Man, moves ghosts, checks collisions, handles scoring.
void updateGame(Game& G);

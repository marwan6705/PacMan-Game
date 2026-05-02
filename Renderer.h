#pragma once
// ============================================================
//  Renderer.h
//  All drawing / rendering functions.
//  Each function takes only what it needs to draw – no globals.
// ============================================================

#include "Game.h"

// Draw every wall, dot, and power-dot in the maze.
void DrawMaze(const Game& G);

// Draw Pac-Man at his current pixel position with mouth animation.
void DrawPacman(const Game& G);

// Draw a single ghost at its current pixel position.
// 'frame' is used to flash the ghost white when scared-time is low.
void DrawGhost(const Ghost& gh, int frame);

// Draw the right-side info panel (score, lives, leaderboard, DS status).
void DrawSidePanel(const Game& G);

// Draw the thin bottom bar showing score / level / lives / PAUSED text.
void DrawBottomBar(const Game& G);

// Draw the main menu screen (title, name entry, leaderboard).
void DrawMenu(Game& G);

// Draw the semi-transparent end-of-game overlay.
// won=true → "YOU WIN!"   won=false → "GAME OVER"
void DrawEndScreen(const Game& G, bool won);

// Draw the semi-transparent pause overlay.
void DrawPauseScreen(const Game& G);

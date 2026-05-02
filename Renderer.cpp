// ============================================================
//  
//  All drawing / rendering function implementations.
//  Only raylib draw calls live here – no game logic.
// ============================================================

#include "Renderer.h"
#include <cstdio>   // snprintf
#include <cmath>    // sinf, cosf, fmod

// ─────────────────────────────────────────────────────────────
//  DrawMaze
//  Loops over every cell and draws: walls (blue), dots (cream
//  circles), or power dots (pulsing yellow circles).
// ─────────────────────────────────────────────────────────────
void DrawMaze(const Game& G) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int px = c * CELL;   // top-left pixel x of this cell
            int py = r * CELL;   // top-left pixel y of this cell
            int  v = G.maze[r][c];

            if (v == TILE_WALL) {
                // Solid blue rectangle with a lighter inner outline
                DrawRectangle(px+1, py+1, CELL-2, CELL-2,
                              (Color){20, 60, 180, 255});
                DrawRectangleLines(px+2, py+2, CELL-4, CELL-4,
                                   (Color){60, 130, 255, 140});
            }
            else if (v == TILE_DOT) {
                // Small cream-coloured pellet
                DrawCircle(px + CELL/2, py + CELL/2, 3,
                           (Color){255, 215, 170, 255});
            }
            else if (v == TILE_POWER_DOT) {
                // Large pulsing yellow power pellet
                float pulse = 5.f + 2.f * sinf(G.frame * 0.18f);
                DrawCircle(px + CELL/2, py + CELL/2, (int)pulse, YELLOW);
                DrawCircleLines(px + CELL/2, py + CELL/2, (int)pulse + 2,
                                (Color){255, 255, 100, 90});
            }
            // TILE_EATEN and TILE_GHOST_HOUSE are drawn as empty (black floor)
        }
    }
}

// ─────────────────────────────────────────────────────────────
//  DrawPacman
//  Draws a circle-sector (pie slice) that opens and closes.
//  The flat side of the pie faces the direction of travel.
// ─────────────────────────────────────────────────────────────
void DrawPacman(const Game& G) {
    float cx = (float)G.pac.px;   // pixel center x
    float cy = (float)G.pac.py;   // pixel center y
    float r  = CELL / 2.f - 2.f; // radius (slightly smaller than a cell)

    // Rotate the "starting angle" of the sector so the opening
    // faces the direction Pac-Man is moving.
    float startDeg = 0.f;
    if      (G.pac.dir == DIR_RIGHT || G.pac.dir == DIR_NONE) startDeg =   0.f;
    else if (G.pac.dir == DIR_DOWN)                             startDeg =  90.f;
    else if (G.pac.dir == DIR_LEFT)                             startDeg = 180.f;
    else if (G.pac.dir == DIR_UP)                               startDeg = 270.f;

    float m = G.mouth;  // current opening angle
    DrawCircleSector({cx, cy}, r,
                     startDeg + m, startDeg + 360.f - m, 40, YELLOW);
    DrawCircleSectorLines({cx, cy}, r,
                          startDeg + m, startDeg + 360.f - m, 40,
                          (Color){180, 140, 0, 100});

    // Small black eye (positioned slightly toward the top of the head)
    float eyeAng = (startDeg + 65.f) * DEG2RAD;
    DrawCircle((int)(cx + r * 0.48f * cosf(eyeAng)),
               (int)(cy + r * 0.48f * sinf(eyeAng)), 2, BLACK);
}

// ─────────────────────────────────────────────────────────────
//  DrawGhost
//  Draws a ghost shape: semicircle head, rectangular body, wavy
//  bottom, and eyes.  Colour changes when SCARED.
// ─────────────────────────────────────────────────────────────
void DrawGhost(const Ghost& gh, int frame) {
    float cx = (float)gh.e.px;
    float cy = (float)gh.e.py;
    float r  = CELL / 2.f - 2.f;

    // Body colour: normal, scared (dark blue), or scared-flashing (white)
    Color body = gh.normalColor;
    if (gh.mode == SCARED) {
        bool flashing = (gh.scaredTimer < 80 && (frame / 7) % 2 == 0);
        body = flashing ? WHITE : DARKBLUE;
    }

    // Head (semicircle) + rectangular body
    DrawCircle   ((int)cx,           (int)cy, (int)r, body);
    DrawRectangle((int)(cx - r),     (int)cy, (int)(2 * r), (int)r, body);

    // Wavy bottom edge: three small circles
    float bw = (2.f * r) / 3.f;
    for (int i = 0; i < 3; i++) {
        float bx = cx - r + i * bw + bw / 2.f;
        DrawCircle((int)bx, (int)(cy + r), (int)(bw / 2.f) + 1, BLACK);
    }

    // Eyes and pupils (only when not scared)
    if (gh.mode != SCARED) {
        // White sclera
        DrawCircle((int)(cx - r * 0.35f), (int)(cy - r * 0.25f), 4, WHITE);
        DrawCircle((int)(cx + r * 0.35f), (int)(cy - r * 0.25f), 4, WHITE);

        // Blue pupils that shift slightly in the direction the ghost moves
        int pdx = 0, pdy = 0;
        if (gh.e.dir == DIR_LEFT)  pdx = -2;
        if (gh.e.dir == DIR_RIGHT) pdx =  2;
        if (gh.e.dir == DIR_UP)    pdy = -2;
        if (gh.e.dir == DIR_DOWN)  pdy =  2;

        DrawCircle((int)(cx - r * 0.35f + pdx), (int)(cy - r * 0.25f + pdy), 2, BLUE);
        DrawCircle((int)(cx + r * 0.35f + pdx), (int)(cy - r * 0.25f + pdy), 2, BLUE);
    } else {
        // Scared face: a zigzag "worried" mouth
        DrawLine((int)(cx - r*.45f), (int)(cy + r*.1f),
                 (int)(cx - r*.15f), (int)(cy - r*.1f), WHITE);
        DrawLine((int)(cx - r*.15f), (int)(cy - r*.1f),
                 (int)(cx + r*.15f), (int)(cy + r*.1f), WHITE);
        DrawLine((int)(cx + r*.15f), (int)(cy + r*.1f),
                 (int)(cx + r*.45f), (int)(cy - r*.1f), WHITE);
    }
}

// ─────────────────────────────────────────────────────────────
//  DrawSidePanel
//  Right-hand panel: player info, lives, score, leaderboard,
//  data-structure status, and controls reference.
// ─────────────────────────────────────────────────────────────
void DrawSidePanel(const Game& G) {
    // Dark background
    DrawRectangle(MAZE_W, 0, PANEL_W, WIN_H, (Color){8, 8, 25, 255});

    int px = MAZE_W + 10;  // left edge of panel content
    int py = 12;           // current vertical drawing position
    char buf[64];

    // ── Title ────────────────────────────────────────────────
    DrawText("PAC-MAN", px, py, 22, YELLOW);
    py += 36;

    // ── Player name ──────────────────────────────────────────
    snprintf(buf, sizeof(buf), "Player: %s", G.playerName);
    DrawText(buf, px, py, 13, LIGHTGRAY);
    py += 22;

    // ── Lives (drawn as small Pac-Man icons) ─────────────────
    DrawText("LIVES:", px, py, 14, WHITE);
    for (int i = 0; i < G.lives; i++) {
        DrawCircleSector({(float)(px + 68 + i * 20), (float)(py + 6)},
                         7.f, 30.f, 330.f, 16, YELLOW);
    }
    py += 26;

    // ── Score / Level / Dots remaining ───────────────────────
    snprintf(buf, sizeof(buf), "SCORE:  %d", G.score);
    DrawText(buf, px, py, 14, LIGHTGRAY);  py += 20;

    snprintf(buf, sizeof(buf), "LEVEL:  %d", G.level);
    DrawText(buf, px, py, 14, LIGHTGRAY);  py += 20;

    snprintf(buf, sizeof(buf), "DOTS:   %d", G.totalDots - G.dotsEaten);
    DrawText(buf, px, py, 14, (Color){255, 215, 170, 255});  py += 28;

    // ── Power-up bar ─────────────────────────────────────────
    if (G.powerActive) {
        DrawText("POWER!", px, py, 14, YELLOW);  py += 18;

        int barW     = PANEL_W - 22;
        int fillW    = (G.powerTimer * barW) / POWER_DURATION;
        DrawRectangle(px, py, barW,  10, DARKGRAY);
        DrawRectangle(px, py, fillW, 10, YELLOW);
        DrawRectangleLines(px, py, barW, 10, GRAY);
        py += 22;
    }
    py += 6;

    // ── Leaderboard (top 5 from the List ADT) ────────────────
    DrawLine(px, py, px + PANEL_W - 18, py, (Color){50, 50, 75, 255});
    py += 8;
    DrawText("TOP SCORES:", px, py, 14, WHITE);
    py += 18;

    for (int i = 0; i < G.leaderboard.length && i < 5; i++) {
        const ScoreEntry& se = G.leaderboard.items[i];
        snprintf(buf, sizeof(buf), "%d. %-8s %d", i+1, se.name, se.score);
        Color c = (i == 0) ? GOLD : (i == 1) ? LIGHTGRAY : GRAY;
        DrawText(buf, px, py, 12, c);
        py += 16;
    }
    py += 10;

    // ── Data structure live status ────────────────────────────
    DrawLine(px, py, px + PANEL_W - 18, py, (Color){50, 50, 75, 255});
    py += 8;
    DrawText("DS STATUS:", px, py, 12, (Color){150, 150, 150, 255});
    py += 16;

    snprintf(buf, sizeof(buf), "Stack: %d cells", Stack_Size(&G.moveHistory));
    DrawText(buf, px, py, 12, GREEN);    py += 14;
    DrawText("Queue: BFS paths", px, py, 12, SKYBLUE); py += 14;
    snprintf(buf, sizeof(buf), "List:  %d scores", G.leaderboard.length);
    DrawText(buf, px, py, 12, ORANGE);   py += 18;

    // ── Controls reference ────────────────────────────────────
    DrawLine(px, py, px + PANEL_W - 18, py, (Color){50, 50, 75, 255});
    py += 8;
    DrawText("CONTROLS:",         px, py, 12, WHITE);  py += 15;
    DrawText("Arrows/WASD  Move", px, py, 11, GRAY);   py += 13;
    DrawText("P         Pause",   px, py, 11, GRAY);   py += 13;
    DrawText("R         Restart", px, py, 11, GRAY);   py += 13;
    DrawText("ESC       Menu",    px, py, 11, GRAY);
}

// ─────────────────────────────────────────────────────────────
//  DrawBottomBar
//  Thin bar at the bottom of the maze area: score/level/lives
//  summary, and a PAUSED banner when the game is paused.
// ─────────────────────────────────────────────────────────────
void DrawBottomBar(const Game& G) {
    DrawRectangle(0, MAZE_H, MAZE_W, 48, (Color){4, 4, 14, 255});

    char buf[64];
    snprintf(buf, sizeof(buf), "Score: %d     Level: %d     Lives: %d",
             G.score, G.level, G.lives);
    DrawText(buf, 10, MAZE_H + 15, 16, WHITE);

    if (G.paused) {
        // pause is now shown as a full overlay – nothing extra needed here
    }
}

// ─────────────────────────────────────────────────────────────
//  DrawMenu
//  Animated main menu: floating dots background, title,
//  player name entry box, and the leaderboard if non-empty.
// ─────────────────────────────────────────────────────────────
void DrawMenu(Game& G) {
    ClearBackground(BLACK);

    double t  = GetTime();
    int    cx = WIN_W / 2;

    // Animated background: 80 slowly drifting pellet dots
    for (int i = 0; i < 80; i++) {
        int bx = (int)fmod(i * 71 + t * 25.0, WIN_W);
        int by = (int)fmod(i * 43 + t * 18.0, WIN_H);
        DrawCircle(bx, by, 2, (Color){255, 215, 170, 40});
    }

    // Title
    DrawText("PAC-MAN",
             cx - MeasureText("PAC-MAN", 64) / 2, 50, 64, YELLOW);

    // Subtitle
    DrawText("CMP G218 - Data Structures Project",
             cx - MeasureText("CMP G218 - Data Structures Project", 16) / 2,
             130, 16, LIGHTGRAY);
    DrawText("Cairo University  |  Spring 2026",
             cx - MeasureText("Cairo University  |  Spring 2026", 14) / 2,
             150, 14, GRAY);

    // ── Name entry box ───────────────────────────────────────
    DrawText("Enter your name:", cx - 130, 205, 18, WHITE);
    DrawRectangleRounded      ({(float)(cx-130), 230, 260, 38}, 0.3f, 8,
                               (Color){25, 25, 55, 255});
    DrawRectangleRoundedLines ({(float)(cx-130), 230, 260, 38}, 0.3f, 8, YELLOW);
    DrawText(G.playerName, cx - 118, 240, 20, YELLOW);

    // Blinking cursor
    if ((int)(t * 2) % 2 == 0) {
        DrawText("|", cx - 118 + MeasureText(G.playerName, 20), 240, 20, YELLOW);
    }

    // Start prompt
    const char* prompt = "PRESS  ENTER  TO  PLAY";
    DrawText(prompt,
             cx - MeasureText(prompt, 20) / 2, 295, 20,
             (Color){255, 255, 80, 200});

    // ── Leaderboard (only shown once at least one game played) ─
    if (!List_IsEmpty(&G.leaderboard)) {
        DrawText("LEADERBOARD:",
                 cx - MeasureText("LEADERBOARD:", 18) / 2, 355, 18, WHITE);

        for (int i = 0; i < G.leaderboard.length && i < 5; i++) {
            char buf[64];
            snprintf(buf, sizeof(buf), "%d.  %-10s  %d",
                     i + 1,
                     G.leaderboard.items[i].name,
                     G.leaderboard.items[i].score);
            Color c = (i == 0) ? GOLD : (i == 1) ? LIGHTGRAY : GRAY;
            DrawText(buf, cx - 100, 378 + i * 20, 16, c);
        }
    }

    // Data structures credit line
    DrawText("DS Used:  Stack | Queue | List",
             cx - MeasureText("DS Used:  Stack | Queue | List", 13) / 2,
             WIN_H - 32, 13, (Color){70, 70, 70, 255});
}

// ─────────────────────────────────────────────────────────────
//  DrawEndScreen
//  Semi-transparent overlay shown over the maze when the game
//  ends (either game-over or win).
// ─────────────────────────────────────────────────────────────
void DrawEndScreen(const Game& G, bool won) {
    // Darken the whole window
    DrawRectangle(0, 0, WIN_W, WIN_H, (Color){0, 0, 0, 165});

    int cx = WIN_W / 2;
    int cy = WIN_H / 2;

    // Big title
    const char* title = won ? "YOU  WIN!" : "GAME  OVER";
    Color       tc    = won ? YELLOW      : RED;
    DrawText(title, cx - MeasureText(title, 52) / 2, cy - 90, 52, tc);

    // Score summary
    char buf[64];
    snprintf(buf, sizeof(buf), "Final Score: %d", G.score);
    DrawText(buf, cx - MeasureText(buf, 22) / 2, cy - 20, 22, WHITE);

    snprintf(buf, sizeof(buf), "Level reached: %d", G.level);
    DrawText(buf, cx - MeasureText(buf, 18) / 2, cy + 10, 18, LIGHTGRAY);

    // Prompt
    DrawText("Press  R  to play again",
             cx - MeasureText("Press  R  to play again", 17) / 2,
             cy + 55, 17, GRAY);
    DrawText("Press  ESC  to quit",
             cx - MeasureText("Press  ESC  to quit", 15) / 2,
             cy + 80, 15, DARKGRAY);
}

// ─────────────────────────────────────────────────────────────
//  DrawPauseScreen
//  Semi-transparent overlay drawn on top of the frozen game,
//  same visual style as DrawEndScreen.
// ─────────────────────────────────────────────────────────────
void DrawPauseScreen(const Game& G) {
    // Darken the whole window
    DrawRectangle(0, 0, WIN_W, WIN_H, (Color){0, 0, 0, 165});

    int cx = WIN_W / 2;
    int cy = WIN_H / 2;

    // Rounded panel background
    DrawRectangleRounded(
        {(float)(cx - 160), (float)(cy - 120), 320, 240},
        0.12f, 10, (Color){10, 10, 35, 230}
    );
    DrawRectangleRoundedLines(
        {(float)(cx - 160), (float)(cy - 120), 320, 240},
        0.12f, 10, (Color){255, 200, 0, 200}
    );

    // Title
    const char* title = "PAUSED";
    DrawText(title, cx - MeasureText(title, 52) / 2, cy - 100, 52, YELLOW);

    // Divider line
    DrawLine(cx - 120, cy - 30, cx + 120, cy - 30, (Color){255, 200, 0, 100});

    // Current stats
    char buf[64];
    snprintf(buf, sizeof(buf), "Score:  %d", G.score);
    DrawText(buf, cx - MeasureText(buf, 20) / 2, cy - 15, 20, WHITE);

    snprintf(buf, sizeof(buf), "Level:  %d", G.level);
    DrawText(buf, cx - MeasureText(buf, 18) / 2, cy + 12, 18, LIGHTGRAY);

    snprintf(buf, sizeof(buf), "Lives:  %d", G.lives);
    DrawText(buf, cx - MeasureText(buf, 18) / 2, cy + 36, 18, LIGHTGRAY);

    // Divider line
    DrawLine(cx - 120, cy + 62, cx + 120, cy + 62, (Color){255, 200, 0, 100});

    // Prompts
    DrawText("Press  P  to resume",
             cx - MeasureText("Press  P  to resume", 17) / 2,
             cy + 74, 17, GRAY);
    DrawText("Press  ESC  for menu",
             cx - MeasureText("Press  ESC  for menu", 15) / 2,
             cy + 98, 15, DARKGRAY);
}

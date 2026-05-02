
#include "raylib.h"
#include "Game.h"
#include "Renderer.h"

int main(void) {
    // ── Create the window ────────────────────────────────────
    InitWindow(WIN_W, WIN_H, "Pac-Man  -  CMP G218 Data Structures");//Graphics display
    SetExitKey(KEY_NULL);   // disable raylib's default ESC=close so we can use ESC ourselves
    SetTargetFPS(60);   // lock to 60 frames per second

    // ── Create and initialise the game state ─────────────────
    Game G;
    initGame(G);

    // ── Main game loop ───────────────────────────────────────
    // WindowShouldClose() returns true when the user presses the
    // X button on the window, or presses the OS close shortcut.
    while (!WindowShouldClose()) {

        // ── Global hotkeys (work on any screen) ─────────────//raylib function(iskeypressed)
        if (IsKeyPressed(KEY_R)) {
            // Restart – keep the leaderboard between runs
            List backup  = G.leaderboard;//copying the existing leaderboard
            initGame(G);
            G.leaderboard = backup;
            G.screen      = SCR_GAME;
        }

        // P toggles pause while in-game
        if (IsKeyPressed(KEY_P) && G.screen == SCR_GAME)
            G.paused = !G.paused;//if paused resumes, if not then it is paused

        // ESC always goes back to the main menu (from any screen)
        if (IsKeyPressed(KEY_ESCAPE) && G.screen != SCR_MENU) {
            G.screen = SCR_MENU;
            G.paused = false;   // clear pause so the next game starts clean
        }

        // ── Menu: handle name typing ─────────────────────────
        if (G.screen == SCR_MENU) {
            // GetCharPressed() returns the Unicode codepoint of
            // any printable key pressed this frame (0 if none).
            int key = GetCharPressed();
            while (key > 0) {
                // Accept printable ASCII and limit length to 12
                if (key >= 32 && key <= 122 && G.nameLen < 12) {
                    G.playerName[G.nameLen++] = (char)key;//adding key then incrementing
                    G.playerName[G.nameLen]   = '\0';
                }
                key = GetCharPressed();   // check for more keys this frame
            }

            // Backspace deletes the last character
            if (IsKeyPressed(KEY_BACKSPACE) && G.nameLen > 0)
                G.playerName[--G.nameLen] = '\0';// Decrement then adding null

            // Enter starts the game
            if (IsKeyPressed(KEY_ENTER)) {
                if (G.nameLen == 0) {
                    // Default name if field left empty
                    strcpy(G.playerName, "Player");
                    G.nameLen = 6;
                }
                resetLevel(G, true);
                G.screen = SCR_GAME;
            }
        }

        // ── Update game logic (only when playing and not paused) ─
        if (G.screen == SCR_GAME && !G.paused)
            updateGame(G);

        // ── Draw ─────────────────────────────────────────────
        BeginDrawing();
        ClearBackground(BLACK);

        if (G.screen == SCR_MENU) {
            DrawMenu(G);                        // full-screen menu
        } else {
            // In-game: draw maze + entities + UI panels
            DrawMaze(G);
            DrawPacman(G);
            for (int i = 0; i < 4; i++)// drawing the ghosts
                DrawGhost(G.ghosts[i], G.frame);
            DrawSidePanel(G);
            DrawBottomBar(G);

            // Overlay end-of-game screen if applicable
            if (G.screen == SCR_GAMEOVER) DrawEndScreen(G, false);
            if (G.screen == SCR_WIN)      DrawEndScreen(G, true);
            if (G.paused)                 DrawPauseScreen(G);
        }

        EndDrawing();//End drawing for ray lib
    }

    // if i get outside the loop the the game is closed
    CloseWindow();
    return 0;
}

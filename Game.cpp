//  Core game logic

#include "Game.h"
#include "Pathfinding.h"   // BFS Queue
#include "Maze.h"
#include <cstring>

// Rebuilds maze, resets entities. fullReset wipes score/lives.
void resetLevel(Game& G, bool fullReset) {

    // Reset maze array
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            G.maze[r][c] = MAZE_TEMPLATE[r][c];

    G.totalDots  = countDots(G.maze);
    G.dotsEaten  = 0;
    G.powerActive= false;
    G.powerTimer = 0;
    G.frame      = 0;

    // Init mouth animation
    G.mouth        = 30.f;
    G.mouthOpening = false;
    G.pacWant      = DIR_NONE;

    // Spawn Pac-Man
    entityPlace(G.pac, 10, 16);

    // Spawn ghosts – distinct cells, alternating directions, staggered exit delays
    const int       gsx[4]   = {  9, 11,  9, 11 };
    const int       gsy[4]   = {  9,  9, 10, 10 };
    const Direction gdirs[4] = { DIR_UP, DIR_UP, DIR_DOWN, DIR_DOWN };
    const Color     gcols[4] = { RED, PINK, SKYBLUE, ORANGE };

    for (int i = 0; i < 4; i++) {
        entityPlace(G.ghosts[i].e, gsx[i], gsy[i]);
        G.ghosts[i].e.dir       = gdirs[i];
        G.ghosts[i].wantDir     = gdirs[i];
        G.ghosts[i].mode        = CHASE;
        G.ghosts[i].scaredTimer = 0;
        G.ghosts[i].normalColor = gcols[i];
        G.ghosts[i].exitDelay   = i * 20;  // ghosts leave house 60 frames apart
    }

    // New game stats reset
    if (fullReset) {
        G.lives = 3;
        G.score = 0;
        G.level = 1;
        Stack_Init(&G.moveHistory);   // Stack history
    }
}

// Initial setup at program start
void initGame(Game& G) {
    memset(&G, 0, sizeof(G));

    List_Init(&G.leaderboard);    // List leaderboard

    strcpy(G.playerName, "Player");
    G.nameLen = 6;
    G.screen  = SCR_MENU;
    G.paused  = false;

    resetLevel(G, true);
}

// Per-frame game loop
void updateGame(Game& G) {
    G.frame++;

    // 1. Mouth animation
    const float MOUTH_SPEED = 3.5f;
    if (G.mouthOpening) {
        G.mouth += MOUTH_SPEED;
        if (G.mouth >= 42.f) G.mouthOpening = false;
    } else {
        G.mouth -= MOUTH_SPEED;
        if (G.mouth <= 2.f)  G.mouthOpening = true;
    }

    // 2. Player input (buffered)
    if      (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) G.pacWant = DIR_UP;
    else if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) G.pacWant = DIR_DOWN;
    else if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) G.pacWant = DIR_LEFT;
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) G.pacWant = DIR_RIGHT;

    // 3. Move Pac-Man
    if (!G.pac.moving) {
        // Grid alignment check
        bool moved = (G.pacWant != DIR_NONE &&
                      entityStartMove(G.pac, G.maze, G.pacWant));
        if (moved)
            G.pac.dir = G.pacWant;          // turn succeeded
        else
            moved = entityStartMove(G.pac, G.maze, G.pac.dir); // keep going
        if (!moved)
            G.pac.dir = DIR_NONE;           // hit a wall, stop
    }

    // Pixel movement
    bool arrived = entitySlide(G.pac, PAC_SPEED);

    if (arrived) {
        // Push pos to Stack
        Stack_Push(&G.moveHistory, {G.pac.gx, G.pac.gy});

        // Pickups
        int& cell = G.maze[G.pac.gy][G.pac.gx];

        if (cell == TILE_DOT) {
            cell = TILE_EATEN;
            G.score += SCORE_DOT;
            G.dotsEaten++;
        }
        else if (cell == TILE_POWER_DOT) {
            cell = TILE_EATEN;
            G.score += SCORE_POWER_DOT;
            G.dotsEaten++;

            // Trigger Power-up
            G.powerActive = true;
            G.powerTimer  = POWER_DURATION;
            for (int i = 0; i < 4; i++) {
                G.ghosts[i].mode        = SCARED;
                G.ghosts[i].scaredTimer = POWER_DURATION;
            }
        }
    }

    // Power-up timer
    if (G.powerActive && --G.powerTimer <= 0) {
        G.powerActive = false;
        for (int i = 0; i < 4; i++)
            if (G.ghosts[i].mode == SCARED)
                G.ghosts[i].mode = CHASE;
    }

    // 4. Move ghosts (BFS)

    // Each ghost scatters to its own corner so they never share a BFS target.
    // Order: RED=top-right, PINK=top-left, CYAN=bottom-right, ORANGE=bottom-left
    const int scatterX[4] = { COLS - 2, 1,        COLS - 2, 1        };
    const int scatterY[4] = { 1,        1,         ROWS - 2, ROWS - 2 };

    // Collect where every ghost intends to move this frame so we can
    // stop two ghosts from reserving the exact same next cell.
    int reservedX[4], reservedY[4];
    for (int i = 0; i < 4; i++) {
        reservedX[i] = G.ghosts[i].e.tgx;
        reservedY[i] = G.ghosts[i].e.tgy;
    }

    for (int i = 0; i < 4; i++) {
        Ghost&  gh = G.ghosts[i];
        Entity& ge = gh.e;

        // Scared timer
        if (gh.mode == SCARED && --gh.scaredTimer <= 0)
            gh.mode = CHASE;

        if (!ge.moving) {
            // Tick down the exit delay while the ghost is still in the house
            if (gh.exitDelay > 0) {
                gh.exitDelay--;
                continue;   // don't move yet
            }

            // Choose next path
            bool inHouse = (ge.gy >= 9 && ge.gy <= 10 &&
                            ge.gx >= 8 && ge.gx <= 12);

            if (inHouse) {
                // Exit house – move up toward the opening
                gh.wantDir = DIR_UP;
            }
            else if (gh.mode == CHASE) {
                // BFS Chase toward Pac-Man
                gh.wantDir = BFS(G.maze, ge.gx, ge.gy,
                                 G.pac.gx, G.pac.gy);
            }
            else {
                // BFS Flee – each ghost runs to its own unique corner
                gh.wantDir = BFS(G.maze, ge.gx, ge.gy,
                                 scatterX[i], scatterY[i]);
            }

            // Apply movement – prefer wantDir, then current dir, then
            // try directions in a ghost-specific rotation order so that
            // two ghosts at the same intersection won't both pick DIR_UP.
            // The rotation order is offset by ghost index to spread them out.
            const Direction allDirs[4] = {DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT};

            bool moved = entityStartMove(ge, G.maze, gh.wantDir);

            if (!moved)
                moved = entityStartMove(ge, G.maze, ge.dir);

            if (!moved) {
                for (int d = 0; d < 4; d++) {
                    Direction tryDir = allDirs[(d + i) % 4];  // offset by ghost index
                    if (entityStartMove(ge, G.maze, tryDir)) break;
                }
            }

            // Update this ghost's reservation so later ghosts see it
            reservedX[i] = ge.tgx;
            reservedY[i] = ge.tgy;

            // If this ghost's target cell is already reserved by an earlier
            // ghost that is still moving into it, nudge it to a different
            // open neighbour so they don't perfectly overlap.
            for (int j = 0; j < i; j++) {
                if (reservedX[i] == reservedX[j] && reservedY[i] == reservedY[j] &&
                    G.ghosts[j].e.moving) {
                    // Try the rotated fallback directions (different offset from above)
                    ge.moving = false;  // cancel current intent
                    for (int d = 0; d < 4; d++) {
                        Direction tryDir = allDirs[(d + i + 2) % 4];
                        if (entityStartMove(ge, G.maze, tryDir)) {
                            reservedX[i] = ge.tgx;
                            reservedY[i] = ge.tgy;
                            break;
                        }
                    }
                    break;
                }
            }
        }

        entitySlide(ge, GHOST_SPEED);

        // 5. Collisions
        if (ge.gx == G.pac.gx && ge.gy == G.pac.gy) {

            if (gh.mode == SCARED) {
                // Pac-Man eats ghost
                G.score += SCORE_GHOST;
                entityPlace(ge, 10, 9);   // respawn ghost
                gh.mode = CHASE;
            }
            else {
                // Ghost catches Pac-Man
                G.lives--;
                if (G.lives <= 0) {
                    // Save score to List
                    ScoreEntry se;
                    strncpy(se.name, G.playerName, 19);
                    se.name[19] = '\0';
                    se.score = G.score;
                    se.level = G.level;
                    List_AddScore(&G.leaderboard, se);
                    G.screen = SCR_GAMEOVER;
                    return;
                }

                // Respawn entities
                entityPlace(G.pac, 10, 16);
                G.pacWant = DIR_NONE;

                const int       rsx[4]   = {  9, 11,  9, 11 };
                const int       rsy[4]   = {  9,  9, 10, 10 };
                const Direction rdirs[4] = { DIR_UP, DIR_UP, DIR_DOWN, DIR_DOWN };
                for (int j = 0; j < 4; j++) {
                    entityPlace(G.ghosts[j].e, rsx[j], rsy[j]);
                    G.ghosts[j].e.dir   = rdirs[j];
                    G.ghosts[j].wantDir = rdirs[j];
                    G.ghosts[j].mode    = CHASE;
                    G.ghosts[j].exitDelay = j * 60;
                }
                return;  // skip win check
            }
        }
    }

    // 6. Win condition
    if (G.dotsEaten >= G.totalDots) {
        G.score += SCORE_LEVEL_BONUS * G.level;
        G.level++;
        resetLevel(G, false);   // Next level
    }
}
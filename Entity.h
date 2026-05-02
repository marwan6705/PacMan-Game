#pragma once

#include "raylib.h"    
#include "Maze.h"      

// Ghost modes 
enum GhostMode {
    CHASE,   
    SCARED   
};

// Entity Shared by both Pac-Man and each ghost.
struct Entity {
    int       gx, gy;    
    int       px, py;    
    int       tgx, tgy;  
    Direction dir;        
    bool      moving;     
};

//  Ghost
// Wraps an Entity and adds ghost-specific fields.
struct Ghost {
    Entity    e;           
    Direction wantDir;     
    GhostMode mode;        
    int       scaredTimer; 
    Color     normalColor;
    int       exitDelay;   // frames to wait inside house before moving out
};

// Entity functions 

// Declaration : Place an entity  at the center of the grid with no movement.
void entityPlace(Entity& e, int gx, int gy);

// Declaration : check the condition for movement 
bool entityStartMove(Entity& e, const int maze[ROWS][COLS], Direction d);

// Declaration : order of movement without overshoot 
bool entitySlide(Entity& e, int speed);

# Pac-Man Arcade Game in C++

## Overview
A 2D arcade game inspired by the classic "Pac-Man," built entirely from scratch using C++ and the **Raylib** graphics library. This project was developed to bridge theoretical computer science concepts with practical software development, focusing heavily on Object-Oriented Programming (OOP) and the implementation of custom Abstract Data Types (ADTs).

## Core Features
* **Classic Gameplay:** Navigate the maze, consume dots, and avoid four roaming ghosts.
* **Power Dots & State Reversal:** Consuming a power dot temporarily switches ghosts to a vulnerable "Scared" state, allowing Pac-Man to consume them for bonus points.
* **Ghost AI Pathfinding:** Ghosts utilize an optimized Breadth-First Search (BFS) algorithm to calculate the shortest path to the player dynamically.
* **Horizontal Tunnel Wrapping:** Seamless screen transitioning from one side of the maze to the other.
* **Real-time Leaderboard:** In-game score tracking and high-score management.

## Technical Architecture & Data Structures
This project demonstrates the practical application of fundamental data structures in a real-time environment:
* **Queue (BFS Engine):** A custom circular Queue manages the node expansion for the BFS pathfinding algorithm, allowing ghosts to intelligently track Pac-Man in O(1) retrieval time via forward propagation.
* **Stack (Move History):** An array-based Stack records Pac-Man's movement history cell-by-cell (LIFO principle), visualized directly on the UI panel.
* **Linked List (Leaderboard):** A dynamic Linked List stores and manages the game over states, logging player names, levels, and final scores efficiently.

## Controls
* **Movement:** `W`, `A`, `S`, `D` or `Arrow Keys`
* **Pause:** `P`
* **Restart:** `R`
* **Menu/Exit:** `ESC`

## Development Team
This project was collaboratively engineered by:
* **Mowahed:** Entity management logic and the Linked List data structure.
* **Ashraf:** Entity management logic and the Queue data structure.
* **Haitham & Saed:** Core game engine, game loop, and state coordination.
* **Poga:** Main execution controller, input handling, and layered rendering pipeline.
* **Saeed:** Maze generation, grid validation, and tunnel boundary mechanisms.
* **Mazen:** Artificial Intelligence logic and BFS pathfinding optimization.
* **Khaled:** Stack data structure and the visual Renderer module.

## Dependencies
* C++ Compiler (e.g., GCC/MinGW)
* [Raylib](https://www.raylib.com/) (for graphics and UI rendering)

# ============================================================
#  Makefile – Linux / macOS
#  Requires: raylib installed system-wide (apt / brew / etc.)
# ============================================================

CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall
LIBS     = -lraylib -lGL -lm -lpthread -ldl

SRCS = Maze.cpp Entity.cpp Pathfinding.cpp Game.cpp Renderer.cpp main.cpp
OUT  = pacman

all: $(OUT)

$(OUT): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRCS) $(LIBS)

clean:
	rm -f $(OUT)

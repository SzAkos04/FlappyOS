#pragma once

#include "cstdbool.h"
#include "cstdint.h"

#define PLAYER_H 15
#define PLAYER_W 15
#define PIPE_W 25
#define PIPE_GAP 50

#define PIPE_N 3

typedef struct {
    int x, y;
    bool isAlive;
} Player;

typedef struct {
    int x, y;
} Pipe;

typedef struct {
    bool started;
    Player player;
    Pipe pipes[PIPE_N];
    int points;
    int max_points;
} State;

void state_init(State *state);

void flappy_render(State *);
void flappy_update(State *);

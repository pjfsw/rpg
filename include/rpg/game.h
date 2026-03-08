#pragma once

#include "rpg/input.h"
#include "rpg/screen.h"
#include "rpg/world.h"
#include "rpg/terminal.h"

typedef enum {
    GS_MAIN_MENU = 0,
    GS_IN_GAME
} GameState;

typedef enum {
    IGS_LOAD_NEXT_ACTION=0,
    IGS_PLAYER_INPUT
} InGameState;

typedef struct {
    char directions[4][256];
    int count;
} Directions;

typedef struct {
    InGameState inGameState;
    GameState gameState;
    int x;
    World world;
    int roomId;
    Directions directions;
    Terminal terminal;
} Game;

void gameInit(Game *game);

void gameUpdate(Game *game, Input *input);

void gameRender(Game *game, Screen *screen);
#pragma once

#include "rpg/input.h"
#include "rpg/screen.h"
#include "rpg/world.h"

typedef enum {
    GS_MAIN_MENU = 0,
    GS_IN_GAME
} GameState;

typedef enum {
    IGS_FADE_IN_ROOM=0,
    IGS_PLAYER_INPUT,
    IGS_FADE_OUT_ROOM
} InGameState;

typedef struct {
    char *north;
    char *south;
    char *east;
    char *west;
} Directions;

typedef struct {
    uint32_t fade;
    InGameState inGameState;
    GameState gameState;
    int x;
    World world;
    int roomId;
    int nextRoomId;
    Directions directions;
} Game;

void gameInit(Game *game);

void gameUpdate(Game *game, Input *input);

void gameRender(Game *game, Screen *screen);
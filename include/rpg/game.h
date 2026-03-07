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
    IGS_FADE_IN_ROOM=0,
    IGS_PLAYER_INPUT,
    IGS_FADE_OUT_ROOM
} InGameState;

#define MAX_ACTION_LENGTH 256
typedef struct {
    char button[4];
    char action[MAX_ACTION_LENGTH];
} ActionMapping;

#define MAX_ACTIONS 16
typedef struct {
    ActionMapping actions[MAX_ACTIONS];
    int count;
} Actions;

typedef struct {
    uint32_t fade;
    InGameState inGameState;
    GameState gameState;
    int x;
    World world;
    int roomId;
    int nextRoomId;
    Actions actions;
    Terminal terminal;
} Game;

void gameInit(Game *game);

void gameUpdate(Game *game, Input *input);

void gameRender(Game *game, Screen *screen);
#pragma once

#include "rpg/input.h"
#include "rpg/screen.h"

typedef struct {
    int x;
} Game;

void game_update(Game *game, Input *input);

void game_render(Game *game, Screen *screen);
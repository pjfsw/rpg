#pragma once

#include <SDL3/SDL.h>
#include "rpg/input.h"

struct Input {
    Action action;    
};

void inputInit(Input *input);

void inputHandleEvent(Input *input, SDL_Event *e);
#pragma once

#include "rpg/screen.h"
#include "font.h"

#define MAX_SPRITE_DATA 256

struct Screen {    
    SDL_Renderer *renderer;
    SDL_Texture *target;
    SDL_Texture *spriteData[MAX_SPRITE_DATA];
    Font font;
};

void screenInit(Screen *screen, SDL_Renderer *renderer, SDL_Texture *target);

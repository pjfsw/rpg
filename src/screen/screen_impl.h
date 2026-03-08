#pragma once

#include "rpg/screen.h"
#include "font.h"

#define MAX_SPRITE_DATA 256
#define MAX_SPRITE_NAME 512

struct Screen {    
    SDL_Renderer *renderer;
    SDL_Texture *target;
    SDL_Texture *spriteData[MAX_SPRITE_DATA];
    char spriteName[MAX_SPRITE_NAME][MAX_SPRITE_DATA];    
    int spriteCount;
    Font font;
};

void screenInit(Screen *screen, SDL_Renderer *renderer, SDL_Texture *target);

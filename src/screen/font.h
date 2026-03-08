#pragma once

#include <SDL3/SDL.h>

typedef struct {
    SDL_Texture *font[256];
    SDL_Renderer *renderer;
} Font;

#define FONT_HEIGHT 16
#define FONT_SCALE 1
#define FONT_SCALED_HEIGHT (FONT_HEIGHT*FONT_SCALE)
#define FONT_WIDTH 8


void fontInit(Font *font, SDL_Renderer *renderer);

void fontDestroy(Font *font);

void fontWrite(Font *font, char *str, int x, int y, uint32_t color);
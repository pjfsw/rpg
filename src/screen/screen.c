#include <SDL3/SDL.h>
#include "screen_impl.h"
#include "rpg/layout.h"
#include <stdio.h>

static SDL_Texture *createTexture(SDL_Renderer *renderer, int w, int h) {
    return SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );
}

static SDL_Texture *createDummyTexture(SDL_Renderer *renderer, int w, int h)
{
    SDL_Texture *tex = createTexture(renderer, w, h);
    SDL_SetRenderTarget(renderer, tex);

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // magenta debug color
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, NULL);

    return tex;
}


void screenInit(Screen *screen, SDL_Renderer *renderer, SDL_Texture *target) {
    memset(screen, 0, sizeof(Screen));
    screen->renderer = renderer;
    screen->target = target;
    for (int i = 0; i < MAX_SPRITE_DATA; i++) {
        screen->spriteData[i] = createDummyTexture(renderer, 24,24);
    } 
    fontInit(&screen->font, renderer);
    printf("fooo\n");
}

static void drawTexture(Screen *screen, SDL_Texture *texture, int x, int y) {
    /* disable blending on the source texture */
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_NONE);

    SDL_FRect dst = { (float)x, (float)y, 0.0f, 0.0f };

    /* Query texture size so we can draw at native size */
    float w = 0.0f, h = 0.0f;
    SDL_GetTextureSize(texture, &w, &h);
    dst.w = w;
    dst.h = h;

    SDL_RenderTexture(screen->renderer, texture, NULL, &dst);    
}

void drawSprite(Screen *screen, int spriteIndex, int x, int y) {    /* draw onto the virtual target */
    if ((spriteIndex < 0) || (spriteIndex >= MAX_SPRITE_DATA) || (screen->spriteData[spriteIndex] == 0)) {
        return;
    }
    SDL_Texture *sprite = screen->spriteData[spriteIndex];
    drawTexture(screen, sprite, x, y);
}

void drawText(Screen *screen, char *text, int x, int y, uint32_t color) {
    fontWrite(&screen->font, text, x, y, color);
}

void drawRect(Screen *screen, int x, int y, int w, int h, uint32_t color) {
    SDL_FRect rect = {
        .x = x,
        .y = y,
        .w = w,
        .h = h
    };
    SDL_SetRenderDrawColor(
        screen->renderer, (color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);
    SDL_RenderRect(screen->renderer, &rect);
}
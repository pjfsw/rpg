#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "screen_impl.h"
#include "rpg/layout.h"
#include <stdio.h>
#include <string.h>
#include "fileutil.h"

static SDL_Texture *createTexture(SDL_Renderer *renderer, int w, int h) {
    return SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );
}

/**
 * Loads textures from a directory into a provided array.
 * Returns the number of textures successfully loaded.
 */
static int loadTexturesFromFolder(
    SDL_Renderer *renderer, const char *foldername, SDL_Texture **textureArray, char nameArray[MAX_SPRITE_NAME][MAX_SPRITE_DATA], int maxTextures) {
    if (!renderer || !foldername || !textureArray || maxTextures <= 0) {
        return 0;
    }

    int count = 0;

    // SDL_EnumerateDirectory takes a callback function. 
    // We can also use SDL_GetDirectoryFiles for a simpler array-based approach:
    int fileCount = 0;
    printf("Foldername %s\n", foldername);
    char **files = SDL_GlobDirectory(foldername, "*", SDL_GLOB_CASEINSENSITIVE, &fileCount);

    if (!files) {
        SDL_Log("Could not open directory: %s", SDL_GetError());
        return 0;
    }

    printf("Filecount %d\n", fileCount);
        

    for (int i = 0; (i < fileCount) && (count < maxTextures); i++) {
        const char *filename = files[i];
        printf("%s\n", filename);
        // Check extension (case-insensitive check for .png and .jpg/.jpeg)
        if (SDL_strcasestr(filename, ".png") || 
            SDL_strcasestr(filename, ".jpg") || 
            SDL_strcasestr(filename, ".jpeg")) {
                
            printf("Loading %s\n", filename);

            // Construct the full path
            char fullPath[1024];
            SDL_snprintf(fullPath, sizeof(fullPath), "%s/%s", foldername, filename);

            // Create the texture
            SDL_Texture *tex = IMG_LoadTexture(renderer, fullPath);
            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);            
            
            if (tex) {
                textureArray[count] = tex;
                strncpy(nameArray[count], filename, MAX_SPRITE_NAME);
                count++;
            } else {
                SDL_Log("Failed to load %s: %s", filename, SDL_GetError());
            }
        }
    }

    // SDL_GetDirectoryFiles requires us to free the list manually
    SDL_free(files);

    return count;
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

    screen->spriteCount =
        loadTexturesFromFolder(renderer, RPG_ASSETS_DIR, screen->spriteData, screen->spriteName, MAX_SPRITE_DATA);
    printf("Loaded %d textures\n", screen->spriteCount);
    for (int i = screen->spriteCount; i < MAX_SPRITE_DATA; i++) {
        screen->spriteData[i] = createDummyTexture(renderer, 24,24);
    }
    fontInit(&screen->font, renderer);
    printf("fooo\n");
}

int findSprite(Screen *screen, char *name) {
    for (int i = 0; i < screen->spriteCount; i++) {
        if (filenamesMatchIgnoreExtAndCase(name, screen->spriteName[i])) {
            return i;
        }
    }
    return -1;
}

static void drawTexture(Screen *screen, SDL_Texture *texture, int x, int y, uint8_t alpha) {
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture, alpha);
    
    SDL_FRect dst = { (float)x, (float)y, 0.0f, 0.0f };

    /* Query texture size so we can draw at native size */
    float w = 0.0f, h = 0.0f;
    SDL_GetTextureSize(texture, &w, &h);
    dst.w = w;
    dst.h = h;

    SDL_RenderTexture(screen->renderer, texture, NULL, &dst);    
}

int getSpriteHeight(Screen *screen, int spriteIndex) {
    if ((spriteIndex < 0) || (spriteIndex >= screen->spriteCount)) {
        return 0;
    }
    float w = 0.0f, h = 0.0f;
    SDL_GetTextureSize(screen->spriteData[spriteIndex] , &w, &h);
    return h;
}

void drawSprite(Screen *screen, int spriteIndex, int x, int y, uint8_t alpha) {    /* draw onto the virtual target */
    if ((spriteIndex < 0) || (spriteIndex >= MAX_SPRITE_DATA) || (screen->spriteData[spriteIndex] == 0)) {
        return;
    }
    SDL_Texture *sprite = screen->spriteData[spriteIndex];
    drawTexture(screen, sprite, x, y, alpha);
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

void resetClipRect(Screen *screen) {
    SDL_SetRenderClipRect(screen->renderer, NULL);    
}

void setClipRect(Screen *screen, int x, int y, int w, int h) {
    SDL_Rect clip_area = { .x = x, .y = y, .w = w, .h = h };
    SDL_SetRenderClipRect(screen->renderer, &clip_area);    
}

int getFontWidth() {
    return FONT_WIDTH * FONT_SCALE;
}

int getFontHeight() {
    return FONT_SCALED_HEIGHT;    
}
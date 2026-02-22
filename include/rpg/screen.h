#pragma once 

#include <stdint.h>

typedef enum {
    TB_ROOM=0,
    TB_INVENTORY
} TextboxType;

typedef struct Screen Screen;

void drawSprite(Screen *screen, int spriteIndex, int x, int y);

void drawText(Screen *screen, char *text, int x, int y, uint32_t color);

void drawRect(Screen *screen, int x, int y, int w, int h, uint32_t color);

#pragma once

#include "rpg/screen.h"

typedef enum {
    TT_TEXT,
    TT_HEADER,
    TT_BUTTON,
    TT_NEIGHBOUR,
    TT_ACTION,
    TT_ERROR,
} Texttype;

typedef struct {
    uint32_t fade;
    Screen *screen;
    int x;
    int y;
    int w;
    int xpos;
    int ypos;
} Textbox;

void tbInit(Textbox *textbox, Screen *screen, int fade);
// void textboxDrawSprite
void tbWrite(Textbox *textbox, char *text, Texttype texttype);
void tbWriteln(Textbox *textbox, char *text, Texttype texttype);
void tbDrawImage(Textbox *textbox, char *name);
void tbNewline(Textbox *textbox, int count);
#include "textbox.h"
#include "rpg/layout.h"
#include <string.h>

#define ERROR_COLOR  0xff222200
#define HEADER_COLOR 0xffffff00
#define TEXT_COLOR   0xeedd4400
#define BUTTON_COLOR 0xffffffff
#define NEIGHBOUR_COLOR 0x4488ff00
#define FRAME_COLOR 0x5F574FFF
#define ACTION_COLOR 0x773344ff

static const int imageHeight = 128;

void tbInit(Textbox *textbox, Screen *screen, int fade) {
    textbox->screen = screen;

    textbox->y = 0;
    textbox->x = 0;
    textbox->w = 80;

    if (fade < 0) {
        textbox->fade = 0;
    } else if (fade > 255) {
        textbox->fade = 255;
    } else {
        textbox->fade = fade;
    }    
    textbox->xpos = 0;
    textbox->ypos = 0;
    //drawRect(screen, 0,imageHeight,textbox->w*8, SCREEN_HEIGHT-imageHeight, FRAME_COLOR);
}

// void textboxDrawSprite

void tbWrite(Textbox *textbox, char *text, Texttype texttype) {
    if ((strlen(text) + textbox->xpos) >= textbox->w) {
        // TODO add wrapping
        tbNewline(textbox, 1);
    }
    uint32_t color = 0x330000ff;
    switch(texttype) {
        case TT_BUTTON:
            color = BUTTON_COLOR;
            break;
        case TT_ERROR:
            color = ERROR_COLOR | textbox->fade;
            break;
        case TT_HEADER:
            color = HEADER_COLOR | textbox->fade;
            break;
        case TT_NEIGHBOUR:
            color = NEIGHBOUR_COLOR | textbox->fade;
            break;
        case TT_TEXT:
            color = TEXT_COLOR | textbox->fade;
            break;
        case TT_ACTION:
            color = ACTION_COLOR;
            break;
        default:
    }
    drawText(textbox->screen, text, textbox->xpos*getFontWidth() + textbox->x, textbox->y+textbox->ypos*getFontHeight(), color);
    textbox->xpos += strlen(text)+1;
}

void tbWriteln(Textbox *textbox, char *text, Texttype texttype) {
    tbWrite(textbox, text, texttype);
    tbNewline(textbox, 1);
}

void tbNewline(Textbox *textbox, int count) {
    textbox->ypos+=count;
    textbox->xpos = 0;
}

void tbDrawImage(Textbox *textbox, char *name) {
    setClipRect(textbox->screen, 440, 0, textbox->w*getFontWidth()-440, SCREEN_HEIGHT);
    int id = findSprite(textbox->screen, name);
    if (id >= 0) {
        drawSprite(textbox->screen, id, 440, 0, 0xff);
    }
    resetClipRect(textbox->screen);
}


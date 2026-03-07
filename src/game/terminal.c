#include "rpg/terminal.h"
#include "rpg/layout.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

static const int textColor = 0xbfc4cfff;
static const int actionColor = 0x7bb3ffff;
static const int sectionColor = 0x222222ff;

void termInit(Terminal *term) {
    memset(term, 0, sizeof(Terminal));
}

static void addText(Terminal *term, const char *text, TermEntryType type) {
    TerminalEntry *entry = &term->entries[term->head];
    memset(entry, 0, sizeof(TerminalEntry));
    strncpy(entry->content, text, MAX_TERMINAL_ENTRY_LENGTH);
    entry->length = strlen(entry->content);
    entry->type = type;    
    if (term->count < MAX_TERMINAL_HISTORY) {
        term->count++;
    }
    term->head++;
}

void termAddText(Terminal *term, const char *text) {
    addText(term, text, TERM_TEXT);
}


void termAddSection(Terminal *term) {
    addText(term, "---", TERM_SECTION);
    addText(term, "", TERM_TEXT);
}

void termAddAction(Terminal *term, const char *button, const char *action) {
    char s[MAX_TERMINAL_ENTRY_LENGTH];
    strcpy(s, button);
    strcat(s, " ");
    strncat(s, action, MAX_TERMINAL_ENTRY_LENGTH-strlen(s));
    addText(term, s, TERM_ACTION);
}

void termAddImage(Terminal *term, const char *spriteName) {
    addText(term, spriteName, TERM_IMAGE);
}

void termUpdate(Terminal *term) {
    TerminalEntry *entry = &term->entries[term->revealPos];
    while ((term->revealPos != term->head) && term->entries[term->revealPos].type == TERM_IMAGE) {
        term->revealPos++;
    }
    if (entry->charsRevealed >= entry->length) {
        if (term->revealPos != term->head) {
            term->revealPos++;
        }
    }
    if ((entry->type != TERM_IMAGE) && term->revealPos != term->head) {
        if (entry->charsRevealed < entry->length) {
            entry->charsRevealed++;
            strncpy(entry->actual, entry->content, entry->charsRevealed);
        }
    }
}

void termRender(Terminal *term, Screen *screen) {
    if (term->count == 0) {
        return;
    }
    int h = SCREEN_HEIGHT;
    int rh = 0;
    uint8_t idx = term->head;
    int c = 0;
    bool nextSection = false;
    uint8_t alphaIndex = idx;
    while ((rh < h) && (c < term->count)) {
        idx--;
        TerminalEntry *e = &term->entries[idx];
        if (e->type == TERM_SECTION) {
            nextSection = true;            
        }        
        if (!nextSection) {
            alphaIndex = idx;
        }
        if (e->type == TERM_IMAGE) {
            int id = findSprite(screen, e->content);            
            if (id >= 0) {
                e->height = getSpriteHeight(screen, id); // TODO precalc!! 
            }
            e->spriteIndex = id;
        } else {
            e->height = getFontHeight();
        }
        rh += e->height;
        c++;
    }
    const int x = (SCREEN_WIDTH-78*getFontWidth())/2;
        
    int y = SCREEN_HEIGHT-rh;
    uint8_t alpha = 0x80;
    for (int i = 0; i < c; i++) {
        TerminalEntry *e = &term->entries[idx];
        if (idx == alphaIndex) {
            alpha = 0xff;
        }
        if (e->type == TERM_IMAGE) {
            if (e->spriteIndex >= 0) {            
                drawSprite(screen, e->spriteIndex, x, y, alpha);
            }
        } else {
            uint32_t color;
            switch (e->type) {
                case TERM_TEXT:
                    color = textColor;
                    break;
                case TERM_ACTION:
                    color = actionColor;
                    break;
                case TERM_SECTION:
                    color = sectionColor;
                    break;
            }
            drawText(screen, e->actual, x, y, (color & 0xFFFFFF00) | alpha);
        }
        y += e->height;
        idx++;
    }
}

void termScroll(Terminal *term, int amount) {
}

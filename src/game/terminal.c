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
    entry->currentAlpha = 0xff;
    entry->targetAlpha = 0xff;    
    if (term->count < MAX_TERMINAL_HISTORY) {
        term->count++;
    }
    term->terminalBusy = true;
    term->head++;
}

void termAddText(Terminal *term, const char *text) {
    addText(term, text, TERM_TEXT);
}

void termAddSection(Terminal *term) {
    addText(term, "---", TERM_SECTION);
    addText(term, "", TERM_TEXT);
    term->isTransitioning = true;    
}

bool termReady(Terminal *term) {
    return (term->revealPos == term->head) && (!term->terminalBusy);
}

void termAddAction(Terminal *term, const char *button, const char *action) {
    char s[MAX_TERMINAL_ENTRY_LENGTH];
    strcpy(s, button);
    strcat(s, " ");
    strncat(s, action, MAX_TERMINAL_ENTRY_LENGTH-strlen(s));
    addText(term, s, TERM_ACTION);
}

void termAddImage(Terminal *term, const char *spriteName) {
    printf("ADD IMAGE %s\n", spriteName);
    addText(term, spriteName, TERM_IMAGE);
}

static void startAlphaFade(Terminal *term) {
    term->isTransitioning = false;

    uint8_t idx = term->head;
    int sectionsSeen = 0;

    // Assign a dim target alpha to older sections
    for (int i = 0; i < term->count; i++) {
        idx--;
        TerminalEntry *e = &term->entries[idx];

        if (e->type == TERM_SECTION) {
            sectionsSeen++;
        }

        if (sectionsSeen > 0) {
            e->targetAlpha = 0x80;
        }
    }
}

static void updateAlpha(Terminal *term) {
    const int fadeSpeed = 4;
    bool noFade = true;
    for (int i = 0; i < term->count; i++) {
        TerminalEntry *e = &term->entries[i];

        if (e->currentAlpha > e->targetAlpha) {
            // Subtract 5 per frame for a smooth fade without underflowing
            if (e->currentAlpha - e->targetAlpha < fadeSpeed) {
                e->currentAlpha = e->targetAlpha;
                noFade = false;
            } else {
                e->currentAlpha -= fadeSpeed;
            }
        }
    }
    if (noFade) {
        term->terminalBusy = false;
    }
}

void termUpdate(Terminal *term) {
    if ((term->revealPos == term->head)) {
        if (term->isTransitioning) {
            startAlphaFade(term);
        } else {
            updateAlpha(term);
        }
        return;
    }
    TerminalEntry *entry = &term->entries[term->revealPos];

    if (entry->type == TERM_IMAGE) {
        if ((entry->targetHeight > 0) && (entry->height >= entry->targetHeight)) {
            term->revealPos++;
        }
    } else {
        if (entry->charsRevealed < entry->length) {
            entry->charsRevealed++;
            strncpy(entry->actual, entry->content, entry->charsRevealed);
        } else {
            term->revealPos++;
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
    while ((rh < h) && (c < term->count)) {
        idx--;
        TerminalEntry *e = &term->entries[idx];

        if (e->type == TERM_IMAGE) {
            int id = findSprite(screen, e->content);
            if (id >= 0) {
                e->targetHeight = getSpriteHeight(screen, id);

                if (e->height < e->targetHeight) {
                    // Increase by 4 pixels per frame (adjust for speed)
                    e->height += 8;

                    if (e->height > e->targetHeight) {
                        e->height = e->targetHeight;
                    }
                }
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
    for (int i = 0; i < c; i++) {
        TerminalEntry *e = &term->entries[idx];
        uint8_t alpha = e->currentAlpha;
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

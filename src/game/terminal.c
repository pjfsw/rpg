#include "rpg/terminal.h"
#include "rpg/layout.h"
#include <string.h>
#include <stdio.h>

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
}

void termUpdate(Terminal *term) {
    TerminalEntry *entry = &term->entries[term->revealPos];
    if (entry->charsRevealed >= entry->length) {
        if (term->revealPos != term->head) {
            term->revealPos++;
        }
    }
    if (term->revealPos != term->head) {
        if (entry->charsRevealed < entry->length) {
            entry->charsRevealed++;
            strncpy(entry->actual, entry->content, entry->charsRevealed);
        }
    }
}

void termRender(Terminal *term, Screen *screen) {
    int h = SCREEN_HEIGHT / 16;
    int n = term->count > h ? h : term->count;
    uint8_t a = term->head-n;
    uint8_t b = term->head;
    int y = 0;
    for (uint8_t i = a; i != b; i++) {
        TerminalEntry *e = &term->entries[i];
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
        drawText(screen, e->actual, 8, y*16, color);
        y++;
    }    
}
void termScroll(Terminal *term, int amount) {
}

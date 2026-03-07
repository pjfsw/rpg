#pragma once

#include "rpg/screen.h"

typedef enum {
    TERM_TEXT,
    TERM_SECTION,
    TERM_ACTION,
    TERM_IMAGE
} TermEntryType;

#define MAX_TERMINAL_ENTRY_LENGTH 256
typedef struct {
    TermEntryType type;
    char content[MAX_TERMINAL_ENTRY_LENGTH+1];
    char actual[MAX_TERMINAL_ENTRY_LENGTH+1];
    
    // For progressive rendering (typewriter effect)
    int charsRevealed; 
    int length;    
    int height;
    int spriteIndex;
} TerminalEntry;

#define MAX_TERMINAL_HISTORY 256

typedef struct {
TerminalEntry entries[MAX_TERMINAL_HISTORY];
    uint8_t head;          // The index where the next entry will be inserted
    int count;         // Current number of valid entries (up to MAX_TERMINAL_HISTORY)
    uint8_t revealPos;
} Terminal;

void termInit(Terminal *term);
void termAddSection(Terminal *term);
void termAddText(Terminal *term, const char *text);
void termAddAction(Terminal *term, const char *button, const char *action);
void termAddImage(Terminal *term, const char *spriteName);
void termUpdate(Terminal *term);
void termRender(Terminal *term, Screen *screen);
void termScroll(Terminal *term, int amount);
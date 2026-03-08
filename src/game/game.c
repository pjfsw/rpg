#include <stdio.h>
#include <string.h>
#include "rpg/game.h"
#include "textbox.h"

#define FADE_SPEED 32

static void drawDirection(Terminal *terminal, char direction[]) {
    if (strlen(direction) == 0) {
        return;
    }
    termAddDirection(terminal, direction);
}

static void updateTerminal(Game *game) {
    Room *room = worldGetRoom(game->roomId);
    int y = 0;
    if (room == NULL) {
        char s[200];
        snprintf(s, 200, "Unknown room id: %d", game->roomId);
        return;
    }
    if (strlen(room->background) > 0) {
        termAddImage(&game->terminal, room->background);
    }
    for (int i = 0; i < room->desc_count; i++) {  
        termAddText(&game->terminal, room->desc[i]); 
    }
    termAddText(&game->terminal, "");
    for (int i = 0; i < game->directions.count; i++) {
        drawDirection(&game->terminal, game->directions.directions[i]);
    }
}

static char *copyDirection(char *neighbor) {
    if (neighbor == NULL) {
        return NULL;
    }
    Room *room = worldFindRoom(neighbor);    
    if (room == NULL) {
        return NULL;
    }
    return room->name;
}

static void addDirection(Game *game, char *direction, char *item) {
    if ((direction == NULL) || (item == NULL)) {
        return;
    }
    strcpy(game->directions.directions[game->directions.count], direction);
    strcat(game->directions.directions[game->directions.count], item);
    game->directions.count++;    
}

static void updateDirections(Game *game) {
    memset(&game->directions, 0, sizeof(Directions));
    Room *room = worldGetRoom(game->roomId);
    if (room == NULL) {
        return;
    }
    
    addDirection(game, "North is ", copyDirection(room->north));    
    addDirection(game, "South is ", copyDirection(room->south));
    addDirection(game, "West is ", copyDirection(room->west));
    addDirection(game, "East is ", copyDirection(room->east));
}

static void newRoom(Game *game) {
    updateDirections(game);
    updateTerminal(game);
}

static void newGame(Game *game) {
    game->gameState = GS_IN_GAME;
    game->roomId = 0;
    newRoom(game);
}

void gameInit(Game *game) {
    termInit(&game->terminal);
    worldLoad();
    newGame(game);
}

static void handleMovement(Game *game, char *direction) {
    if (direction == NULL) {
        return;
    }
    Room *nextRoom = worldFindRoom(direction);
    if (nextRoom != NULL) {
        termAddSection(&game->terminal);
        game->roomId = nextRoom->id;
        newRoom(game);
        game->inGameState = IGS_LOAD_NEXT_ACTION;
    }
}

static void waitForPlayer(Game *game, Action action) {
    Room *currentRoom = worldGetRoom(game->roomId);
    switch (action) {
        case ACTION_UP:
            handleMovement(game, currentRoom->north);
            break;
        case ACTION_DOWN:
            handleMovement(game, currentRoom->south);
            break;
        case ACTION_LEFT:
            handleMovement(game, currentRoom->west);
            break;
        case ACTION_RIGHT:
            handleMovement(game, currentRoom->east);
            break;
        case ACTION_SELECT:
            break;
        case ACTION_START:
            break;
    }
}

static void waitForTransition(Game *game) {
    if (termReady(&game->terminal)) {
        game->inGameState = IGS_PLAYER_INPUT;
    }
}

static void inGameUpdate(Game *game, Input *input) {
    Action action = inputGetAction(input);
    if (action == ACTION_START) {
        // Go to menu
    } else {
        switch (game->inGameState) {
            case IGS_LOAD_NEXT_ACTION:
                waitForTransition(game);
                break;
            case IGS_PLAYER_INPUT:
                waitForPlayer(game, action);
                break;
        }
    }
}

void gameUpdate(Game *game, Input *input) {
    if (game->gameState == GS_IN_GAME) {
        inGameUpdate(game, input);
    }
}

void gameRender(Game *game, Screen *screen) {
    if (game->gameState == GS_IN_GAME) {
        termUpdate(&game->terminal);
        termUpdate(&game->terminal);    // 120 fps update
        termRender(&game->terminal, screen);
   } else if (game->gameState == GS_MAIN_MENU) {
    }
}

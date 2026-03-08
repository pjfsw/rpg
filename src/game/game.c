#include <stdio.h>
#include <string.h>
#include "rpg/game.h"
#include "textbox.h"

#define FADE_SPEED 32

static void drawActionMapping(Terminal *terminal, ActionMapping *mapping) {
    if ((mapping == NULL) || (mapping->action == NULL) || (mapping->button == NULL)) {
        return;
    }
    termAddAction(terminal, mapping->button, mapping->action);
}

static void updateTerminal(Game *game) {
    Room *room = worldGetRoom(game->roomId);
    int y = 0;
    if (room == NULL) {
        char s[200];
        snprintf(s, 200, "Unknown room id: %d", game->roomId);
        return;
    }
    if (room->background != NULL) {
        termAddImage(&game->terminal, room->background);
    }
    for (int i = 0; i < room->desc_count; i++) {  
        termAddText(&game->terminal, room->desc[i]); 
    }
    termAddText(&game->terminal, "");
    for (int i = 0; i < game->actions.count; i++) {
        drawActionMapping(&game->terminal, &game->actions.actions[i]);
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

static void addAction(Game *game, char *button, char *action) {
    if ((button == NULL) || (action == NULL)) {
        return;
    }
    strncpy(game->actions.actions[game->actions.count].button, button, 3);
    strncpy(game->actions.actions[game->actions.count].action, action, MAX_ACTION_LENGTH);
    game->actions.count++;    
}

static void updateDirections(Game *game) {
    Room *room = worldGetRoom(game->roomId);
    if (room == NULL) {
        return;
    }
    
    addAction(game, "U", copyDirection(room->north));    
    addAction(game, "D", copyDirection(room->south));
    addAction(game, "L", copyDirection(room->west));
    addAction(game, "R", copyDirection(room->east));
}

static void updateActions(Game *game) {
    memset(&game->actions, 0, sizeof(Actions));
    updateDirections(game);
}

static void newRoom(Game *game) {
    updateActions(game);
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
    Room *newRoom = worldFindRoom(direction);
    if (newRoom != NULL) {
        termAddSection(&game->terminal);
        game->nextRoomId = newRoom->id;
        game->inGameState = IGS_FADE_OUT_ROOM;
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

static void fadeInRoom(Game *game) {
    game->fade += FADE_SPEED;
    if (game->fade > 255) {
        game->fade = 255;
    }
    if (game->fade == 255) {
        game->inGameState = IGS_PLAYER_INPUT;
    }
}


static void fadeOutRoom(Game *game) {
    if (game->fade > 0) {
        game->fade -= FADE_SPEED;
    } 
    if (game->fade > 255) {
        game->fade = 0;
    }
    if (game->fade == 0) {
        game->roomId = game->nextRoomId;
        newRoom(game);
        game->inGameState = IGS_FADE_IN_ROOM;
    }
}

static void inGameUpdate(Game *game, Input *input) {
    Action action = inputGetAction(input);
    if (action == ACTION_START) {
        // Go to menu
    } else {
        switch (game->inGameState) {
            case IGS_FADE_IN_ROOM:
                fadeInRoom(game);
                break;
            case IGS_PLAYER_INPUT:
                waitForPlayer(game, action);
                break;
            case IGS_FADE_OUT_ROOM:
                fadeOutRoom(game);
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

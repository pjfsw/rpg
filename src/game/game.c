#include <stdio.h>
#include <string.h>
#include "rpg/game.h"
#include "textbox.h"
#define FADE_SPEED 32

static void newGame(Game *game) {
    game->gameState = GS_IN_GAME;
    game->roomId = 0;
}

void gameInit(Game *game) {
    worldLoad();
    newGame(game);
}

static void handleMovement(Game *game, char *direction) {
    if (direction == NULL) {
        return;
    }
    Room *newRoom = worldFindRoom(direction);
    if (newRoom != NULL) {
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

void fadeInRoom(Game *game) {
    game->fade += FADE_SPEED;
    if (game->fade > 255) {
        game->fade = 255;
    }
    if (game->fade == 255) {
        game->inGameState = IGS_PLAYER_INPUT;
    }
}

void fadeOutRoom(Game *game) {
    if (game->fade > 0) {
        game->fade -= FADE_SPEED;
    } 
    if (game->fade > 255) {
        game->fade = 0;
    }
    if (game->fade == 0) {
        game->roomId = game->nextRoomId;
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

typedef char (*GetRoomDirectionFunc)(Room *room);

inline char *getSouth(Room *room) {
    return room->south;
}

char *copyDirection(char *neighbor) {
    if (neighbor == NULL) {
        return NULL;
    }
    Room *room = worldFindRoom(neighbor);    
    if (room == NULL) {
        return NULL;
    }
    return room->name;
}

void updateDirections(Game *game) {
    memset(&game->directions, 0, sizeof(Directions));
    Room *room = worldGetRoom(game->roomId);
    if (room == NULL) {
        return;
    }
    game->directions.north = copyDirection(room->north);
    game->directions.south = copyDirection(room->south);
    game->directions.west = copyDirection(room->west);
    game->directions.east = copyDirection(room->east);
}


void gameUpdate(Game *game, Input *input) {
    if (game->gameState == GS_IN_GAME) {
        inGameUpdate(game, input);
    }
    updateDirections(game);

}

static void drawCompass(Screen *screen, char *button, char *navi, int x, int y, uint32_t fade) {
    int bx = x;// x + strlen(button) / 2;
    drawText(screen, button, bx*8, y*16, 0x3366ffff);
    if (navi != NULL) {
        drawText(screen, navi, (x+strlen(button)+1)*8, y*16, 0xffffff00 | fade);
        //int nx = bx - strlen(navi) / 2;
        //drawText(screen, navi, nx, y + 1, 0xffffffff);
    }
}

static void inGameRender(Game *game, Screen *screen) {
    Textbox tb;
    tbInit(&tb, screen, game->fade);

    Room *room = worldGetRoom(game->roomId);
    int y = 0;
    if (room == NULL) {
        char s[200];
        snprintf(s, 200, "Unknown room id: %d", game->roomId);
        tbWriteln(&tb, s, TT_ERROR);
        return;
    }

    tbWriteln(&tb,  room->name, TT_HEADER);
    tbNewline(&tb, 1);

    for (int i = 0; i < room->desc_count; i++) {    
        tbWriteln(&tb, room->desc[i], TT_TEXT);
    }

    int x = 51;
    drawCompass(screen, "[U]", game->directions.north, x, y++, game->fade);
    drawCompass(screen, "[L]", game->directions.west, x, y++, game->fade); 
    drawCompass(screen, "[R]", game->directions.east, x, y++, game->fade);
    drawCompass(screen, "[D]", game->directions.south, x, y++, game->fade);
}

void gameRender(Game *game, Screen *screen) {
    if (game->gameState == GS_IN_GAME) {
        inGameRender(game, screen);
    } else if (game->gameState == GS_MAIN_MENU) {
    }
}

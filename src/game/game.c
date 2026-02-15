#include "rpg/game.h"

void game_update(Game *game, Input *input) {
    game->x = (game->x + 1) % 320;
}

void game_render(Game *game, Screen *screen) {
    drawSprite(screen, 0, game->x, 10);
}

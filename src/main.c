#include <SDL3/SDL.h>
#include <stdbool.h>

#include "input/input_impl.h"
#include "screen/screen_impl.h"
#include "rpg/game.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *virtualTarget;
    bool running;
    Input input;
    Game game;
} App;

static const int virtualWidth = 320;
static const int virtualHeight = 180;

static SDL_FRect computeLetterbox(int winW, int winH) {
    float scaleX = (float)winW / (float)virtualWidth;
    float scaleY = (float)winH / (float)virtualHeight;
    float scale = scaleX < scaleY ? scaleX : scaleY;

    float w = virtualWidth * scale;
    float h = virtualHeight * scale;

    SDL_FRect r;
    r.w = w;
    r.h = h;
    r.x = (winW - w) * 0.5f;
    r.y = (winH - h) * 0.5f;
    return r;
}

static void toggleFullscreen(SDL_Window *window) {
    SDL_WindowFlags flags = SDL_GetWindowFlags(window);
    bool fullscreen = (flags & SDL_WINDOW_FULLSCREEN) != 0;

    SDL_SetWindowFullscreen(window, fullscreen ? false : true);
}

static void appInit(App *app) {
    inputInit(&app->input);

    SDL_Rect bounds;
    SDL_GetDisplayUsableBounds(SDL_GetPrimaryDisplay(), &bounds);

    int winW = bounds.w * 3 / 4;
    int winH = bounds.h * 3 / 4;

    app->window = SDL_CreateWindow("rpg", winW, winH, SDL_WINDOW_RESIZABLE);

    app->renderer = SDL_CreateRenderer(app->window, NULL);

    app->virtualTarget =
        SDL_CreateTexture(app->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, virtualWidth, virtualHeight);

    SDL_SetTextureScaleMode(app->virtualTarget, SDL_SCALEMODE_NEAREST);
    SDL_SetRenderVSync(app->renderer, 1);
    app->running = true;
}

static bool handleEvent(App *app, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        app->running = false;
        return true;
    }

    if (event->type == SDL_EVENT_KEY_DOWN) {
        bool alt = (event->key.mod & SDL_KMOD_ALT) != 0;

        if (alt && event->key.key == SDLK_Q) {
            app->running = false;
            return true;
        }

        if ((alt && event->key.key == SDLK_RETURN) || event->key.key == SDLK_F11) {
            toggleFullscreen(app->window);
            return true;
        }
    }
    return false;
}

static void appRun(App *app) {
    Screen screen;
    screenInit(&screen, app->renderer, app->virtualTarget);

    while (app->running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (!handleEvent(app, &event)) {
                inputHandleEvent(&app->input, &event);
            }
        }

        game_update(&app->game, &app->input);

        /* Render to virtual texture */
        SDL_SetRenderTarget(app->renderer, app->virtualTarget);
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);

        game_render(&app->game, &screen);

        /* Present to window */
        SDL_SetRenderTarget(app->renderer, NULL);

        int winW, winH;
        SDL_GetWindowSize(app->window, &winW, &winH);

        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);

        SDL_FRect dst = computeLetterbox(winW, winH);
        SDL_RenderTexture(app->renderer, app->virtualTarget, NULL, &dst);

        SDL_RenderPresent(app->renderer);

        //SDL_Delay(16);
    }
}

void appClose(App *app) {
    SDL_DestroyTexture(app->virtualTarget);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return 1;
    }

    App app;
    memset(&app, 0, sizeof(App));
    appInit(&app);
    appRun(&app);
    appClose(&app);    
    SDL_Quit();

    return 0;
}

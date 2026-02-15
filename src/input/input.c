#include "input_impl.h"
#include <SDL3/SDL.h>

void inputInit(Input *input) {
    memset(input, 0, sizeof(Input));
}

void inputHandleEvent(Input *input, SDL_Event *e) {
    (void*)e;
}
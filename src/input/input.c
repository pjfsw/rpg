#include <SDL3/SDL.h>

#include "input_impl.h"

void inputInit(Input *input) {
    if (input) {
        input->action = ACTION_NONE;
    }
}

void inputHandleEvent(Input *input, SDL_Event *e) {
    if (!input || !e) {
        return;
    }

    // Handle Keyboard Inputs
    if (e->type == SDL_EVENT_KEY_DOWN) {
        // Ignore OS-level key repeats if the user holds the key down
        if (e->key.repeat != 0) {
            return;
        }

        switch (e->key.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                input->action = ACTION_UP;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                input->action = ACTION_DOWN;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                input->action = ACTION_LEFT;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                input->action = ACTION_RIGHT;
                break;
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_SPACE:
                input->action = ACTION_SELECT;
                break;
            case SDL_SCANCODE_ESCAPE:
                input->action = ACTION_START;
                break;
            default:
                break;
        }
    }
    // Handle Gamepad Inputs
    else if (e->type == SDL_EVENT_GAMEPAD_BUTTON_DOWN) {
        switch (e->gbutton.button) {
            case SDL_GAMEPAD_BUTTON_DPAD_UP:
                input->action = ACTION_UP;
                break;
            case SDL_GAMEPAD_BUTTON_DPAD_DOWN:
                input->action = ACTION_DOWN;
                break;
            case SDL_GAMEPAD_BUTTON_DPAD_LEFT:
                input->action = ACTION_LEFT;
                break;
            case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:
                input->action = ACTION_RIGHT;
                break;
            case SDL_GAMEPAD_BUTTON_SOUTH:  // Usually 'A' on Xbox, 'Cross' on PlayStation
                input->action = ACTION_SELECT;
                break;
            case SDL_GAMEPAD_BUTTON_START:
                input->action = ACTION_START;
                break;
            default:
                break;
        }
    }
}

Action inputGetAction(Input *input) {
    if (!input)
        return ACTION_NONE;

    // "Consume" the action so it only fires once per physical button press.
    Action currentAction = input->action;
    input->action = ACTION_NONE;

    return currentAction;
}
#pragma once 

typedef enum {
    ACTION_NONE=0,
    ACTION_UP,
    ACTION_DOWN,
    ACTION_LEFT,
    ACTION_RIGHT,
    ACTION_SELECT,
    ACTION_START
} Action;

typedef struct Input Input;

Action inputGetAction(Input *input);

#include "dialogs.h"

const String hello_world_1 = {.str = "hello world", .len = 11};
const String hello_world_2 = {.str = "welcome...\nto the game!!", .len = 24};

const DialogSequence hello_world_sequence = {
    .texts = {&hello_world_1, &hello_world_2, (String *)0, (String *)0, (String *)0},
    .actions = {SEQ_USER_INPUT, SEQ_TIMER, SEQ_NONE},
};
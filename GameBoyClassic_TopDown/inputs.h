#ifndef INPUTS_INCLUDE__
#define INPUTS_INCLUDE__

#include <stdint.h>
#include <gb/gb.h>
#include "math.h"

#define J_A_IDX 0
#define J_B_IDX 1
#define J_SELECT_IDX 2
#define J_START_IDX 3
#define J_BITS_SHIFT 4 // the framework reserve the button to the second half of the fist byte (<1111> 0000 etc..)

typedef struct PlayerInputs
{
    i8 x_direction;
    i8 y_direction;
    u8 state;
    u8 state_down;
    u8 state_up;
    u8 state_pressed[4];
} PlayerInputs;

void joystick_poll(PlayerInputs *input);

BOOLEAN is_down_this_frame(PlayerInputs *inputs, u8 key);
BOOLEAN is_down(PlayerInputs *inputs, u8 key);
BOOLEAN is_up(PlayerInputs *inputs, u8 key);
BOOLEAN is_pressed(PlayerInputs *inputs, u8 duration, u8 key);

#endif
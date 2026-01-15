#include "inputs.h"

static void update_pressed_duration(PlayerInputs *inputs, u8 state, u8 prev, u8 j_key_idx)
{
    u8 key = 1 << (j_key_idx + J_BITS_SHIFT);
    BOOLEAN should_increment = (state & prev & key) != 0;
    inputs->state_pressed[j_key_idx] = should_increment
                                           ? increment(inputs->state_pressed[j_key_idx])
                                           : 0;
}

BOOLEAN is_down_this_frame(PlayerInputs *inputs, u8 key)
{
    return (key & inputs->state_down) != 0;
}

BOOLEAN is_down(PlayerInputs *inputs, u8 key)
{
    return (key & inputs->state) != 0;
}

BOOLEAN is_up(PlayerInputs *inputs, u8 key)
{
    return (key & inputs->state_up) != 0;
}

BOOLEAN is_pressed(PlayerInputs *inputs, u8 duration, u8 key_idx)
{
    return inputs->state_pressed[key_idx] >= duration;
}

void joystick_poll(PlayerInputs *input)
{
    input->x_direction = 0;
    input->y_direction = 0;

    uint8_t frame_state = joypad();
    uint8_t previous_state = input->state;

    input->state_down = (frame_state) & (~previous_state);
    input->state_up = (~frame_state) & previous_state;

    update_pressed_duration(input, frame_state, previous_state, J_START_IDX);
    update_pressed_duration(input, frame_state, previous_state, J_SELECT_IDX);
    update_pressed_duration(input, frame_state, previous_state, J_A_IDX);
    update_pressed_duration(input, frame_state, previous_state, J_B_IDX);

    if ((frame_state & J_RIGHT) != 0)
    {
        input->x_direction = 1;
    }
    else if ((frame_state & J_LEFT) != 0)
    {
        input->x_direction = -1;
    }

    if ((frame_state & J_UP) != 0)
    {
        input->y_direction = -1;
    }
    else if ((frame_state & J_DOWN) != 0)
    {
        input->y_direction = 1;
    }

    input->state = frame_state;
}
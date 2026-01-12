#include "helpers.h"

/// @brief wait the number of frame given in the timer_x argument.
void await(AwaiterParams params)
{
    BOOLEAN continue_awaits = TRUE;

    while (continue_awaits)
    {
        vsync();

        switch (params.type)
        {
        case U8:
            params.value.timer_8 -= 1;
            continue_awaits = params.value.timer_8 > 0;
            break;
        case U16:
            params.value.timer_16 -= 1;
            continue_awaits = params.value.timer_16 > 0;
            break;
        }
    }
}

u8 get_vram_char(char c)
{
    const char lookup_table[7] = {' ', '!', ':', '?', '\\', '=', '.'};
    const u8 vram_addr[] = {128, 165, 166, 167, 168, 169, 171};

    if (c >= 'a' && c <= 'z')
    {
        return UI_FONT_VRAM_OFFSET + c - 'a' + 1; // NOTE(JuH) +1 stands for the blank character at the begining of the font
    }

    for (u8 i = 0; i < 7; i++)
    {
        if (c == lookup_table[i])
        {
            return vram_addr[i];
        }
    }

    return vram_addr[0];
}

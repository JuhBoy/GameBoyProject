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
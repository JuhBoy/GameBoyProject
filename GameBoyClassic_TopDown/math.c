#include "math.h"

i8 sin(u8 a)
{
    // NOTE(Juh): access to generated LUT table
    return sine_lut[a];
}

i8 cos(u8 a)
{
    // NOTE(Juh): access to generated LUT table
    return sine_lut[(u8)((u8)(a) + 64u)];
}

/// @brief output a vector rotated by 'a' angle scaled to speed 'speed'
iVec2 rotate(iVec2 p, u8 a, u8 speed)
{
    i16 c = cos(a * speed);
    i16 s = sin(a * speed);

    i16 x = (i16)(p.x * c + p.y * -s);
    i16 y = (i16)(p.x * s + p.y * c);

    iVec2 out = {.x = x, .y = -y};

    return out;
}
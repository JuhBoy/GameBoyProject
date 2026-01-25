#ifndef __LUTGENERATOR_INCLUDE
#define __LUTGENERATOR_INCLUDE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define u16 unsigned short
#define i16 __int16
#define i8 __int8
#define u8 unsigned __int8

// COLORS
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define PI 3.141592653589793

typedef u8 (*write_callback)(FILE *file, void *data);

typedef struct
{
    write_callback callback;
    void *data;
} FileWriteCallback;

typedef struct
{
    i8* sine_wave;
    const char *additional_content;
} LutSourceData;

void write(const char *file_path, FileWriteCallback callback);
void gen_sine_lut(i16 remap_value, double deg_step);

#endif
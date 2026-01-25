#ifndef __MATH_INCLUDE
#define __MATH_INCLUDE

#include <stdint.h>
#include "res/gen_sine_lut.h"

#define FP_SHIFT_VALUE (uint16_t)4 // 2^4 (16) sub values
#define fp16_t uint16_t

// Numbers definitions
//
#define u8 uint8_t
#define u16 uint16_t

#define i8 int8_t
#define i16 int16_t

typedef struct Vec2
{
	uint16_t x, y;
} Vec2;

typedef struct iVec2 { 
	int16_t x, y;
} iVec2;

typedef struct FPVec2
{
	fp16_t x, y;
} FPVec2;

typedef struct PixelCoords
{
	uint16_t x;
	uint16_t y;
} PixelCoords;

static inline fp16_t new_fp16(uint16_t whole)
{
	uint16_t out = whole;
	return out << FP_SHIFT_VALUE;
}

static inline uint8_t fp16_to_whole8(fp16_t input)
{
	fp16_t r = input >> FP_SHIFT_VALUE;
	return r;
}

static inline uint16_t fp16_to_whole16(fp16_t input)
{
	return input >> FP_SHIFT_VALUE;
}

static inline fp16_t fp16_add(fp16_t l, fp16_t r)
{
	return l + r;
}

static inline fp16_t fp16_sub(fp16_t l, fp16_t r)
{
	return l - r;
}

static inline fp16_t fp16_mul(fp16_t l, fp16_t r)
{
	return (uint32_t)l * (uint32_t)r;
}

static inline fp16_t fp16_mul4(fp16_t a)
{
	return a << 2;
}

static inline fp16_t fp16_mul8(fp16_t a)
{
	return a << 3;
}

static inline fp16_t fp16_div2(fp16_t a)
{
	return (a >> 1);
}

static inline fp16_t fp16_div4(fp16_t a)
{
	return a >> 2;
}

static inline fp16_t fp16_div8(fp16_t a)
{
	return a >> 3;
}

static inline fp16_t fp16_mod32(fp16_t a)
{
	return a - (a >> 5) * 32;
}

static inline fp16_t fp16_mod128(fp16_t a)
{
	return a - (a >> 7) * 128;
}

/// @brief increments the value and prevents overflow
static inline u8 increment(u8 value)
{
	return value == 255 ? value : value + 1;
}

static inline u8 div_2(u8 value)
{
	return (value >> 1);
}

static inline u8 mod_2(u8 value)
{
	return (value - (value >> 1) * 2);
}

static inline u8 min(u8 a, u8 b)
{
	return a < b ? a : b;
}

static inline u8 max(u8 a, u8 b)
{
	return a > b ? a : b;
}

static inline i8 abs(i8 a)
{
	i8 shift = a >> 7;
	return (a ^ shift) - shift;
}

i8 sin(u8 a);
i8 cos(u8 a);
iVec2 rotate(iVec2 p, u8 a, u8 speed);

#endif
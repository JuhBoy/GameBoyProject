#ifndef __HELPERS__
#define __HELPERS__

#include "inputs.h"
#include <gb/gb.h>

/// CALLBACKS
/// ==============
typedef BOOLEAN (*input_fn)(PlayerInputs *data);

typedef struct
{
	input_fn action;
	PlayerInputs *data;
} InputCB;

#define RUN_CB(cb) \
	(cb)->action((cb)->data)

/// DEFINES TYPES
/// ==============
#define u8 uint8_t
#define u16 uint16_t
#define u8ptr uint8_t *
#define u16ptr uint8_t *

/// DEFINES VALUES
/// ==============
#define ONE_SECS (u8)60
#define TWO_SECS (u8)120

/// DEFINES ACTIONS
/// ==============
#define WAIT_ONE_SECOND()                                                \
	AwaiterParams params = {.type = U8, .value = {.timer_8 = ONE_SECS}}; \
	await(params);

#define WAIT_N_SECOND(n)                                                            \
	AwaiterParams params = {.type = U16, .value = {.timer_16 = (u16)n * ONE_SECS}}; \
	await(params);

#define CALL_2(f) \
	f();          \
	f();
#define CALL_3(f) \
	f();          \
	f();          \
	f();
#define CALL_4(f) \
	f();          \
	f();          \
	f();          \
	f();

// VRAM VALUES
//
#define UI_FONT_VRAM_OFFSET (u8)128
#define UI_BORDERS_VRAM_OFFSET (u8)173

/// AWAITERS
/// ==============
union AwaiterValues
{
	u8 timer_8;
	u16 timer_16;
};

typedef enum AwaiterType
{
	U8,
	U16,
} AwaiterType;

typedef struct AwaiterParams
{
	AwaiterType type;
	union AwaiterValues value;
} AwaiterParams;

void await(AwaiterParams params);

// TEXTS
u8 get_vram_char(char c);

#endif
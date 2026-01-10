#ifndef __HELPERS__
#define __HELPERS__

#include <gb/gb.h>

/// CALLBACKS
/// ==============
typedef BOOLEAN (*awaiter_action)(void);

/// DEFINES TYPES
/// ==============
#define u8 uint8_t
#define u16 uint16_t

/// DEFINES VALUES 
/// ==============
#define ONE_SECS (u8)60
#define TWO_SECS (u8)120

/// DEFINES ACTIONS 
/// ==============
#define WAIT_ONE_SECOND()                                                \
	AwaiterParams params = {.type = U8, .value = {.timer_8 = ONE_SECS}}; \
	await(params);

#define WAIT_N_SECOND(n)                                                \
	AwaiterParams params = {.type = U16, .value = {.timer_16 = (u16)n * ONE_SECS}}; \
	await(params);

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

#endif
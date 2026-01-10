#ifndef __entities_INCLUDE
#define __entities_INCLUDE

#include <stdint.h>
#include <gb/gb.h>
#include "math.c"
#include "helpers.h"

typedef enum EntityId {
	ETT_PLAYER = (u8)0,
	ETT_GOBLIN = (u8)1,
} EntityId;

typedef struct Velocity
{
	uint8_t x;
	uint8_t y;
} Velocity;

typedef struct EntitiesBank
{
	EntityId tile_ids[40];

	// positions
	fp16_t px[40];
	fp16_t py[40];

	// velocity
	uint8_t vx[40];
	uint8_t vy[40];

	uint8_t count;
} EntitiesBank;

void init_sprites(EntitiesBank *entities);
void update_velocity(EntitiesBank *entities, EntityId ett, Velocity velocity);
void draw_entities(EntitiesBank *entities);
void update_entity_relative(EntitiesBank *entities, uint8_t id, FPVec2 *entity_pos, PixelCoords *camera_pos);
void hide_entities(EntitiesBank* entities);

#endif
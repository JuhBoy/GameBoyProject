#include "entities.h"

const fp16_t START_POS = 8 << FP_SHIFT_VALUE;
const fp16_t END_X = 160 << FP_SHIFT_VALUE;
const fp16_t END_Y = 140 << FP_SHIFT_VALUE;

void init_sprites(EntitiesBank *entities)
{
	for (int i = 0; i < entities->count; i++)
	{
		uint8_t id = entities->tile_ids[i];
		set_sprite_tile(i, id);
	}
}

void update_velocity(EntitiesBank *entities, EntityId ett, Velocity velocity)
{
	if (entities->count <= ett)
	{
		return;
	}

	entities->vx[ett] = velocity.x;
	entities->vy[ett] = velocity.y;
}

void draw_entities(EntitiesBank *entities)
{
	for (uint8_t i = 0; i < entities->count; i++)
	{
		fp16_t posx = entities->px[i];
		fp16_t posy = entities->py[i];
		uint8_t velx = entities->vx[i];
		uint8_t vely = entities->vy[i];

		if (posx >= END_X)
		{
			posx = START_POS;
		}
		if (posy >= END_Y)
		{
			posy = START_POS;
		}

		posx += velx;
		posy += vely;

		move_sprite(i, fp16_to_whole8(posx), fp16_to_whole8(posy));

		entities->px[i] = posx;
		entities->py[i] = posy;
	}
}

void hide_entities(EntitiesBank *entities)
{
	for (uint8_t i = 0; i < entities->count; i++)
	{
		move_sprite(i, 0, 0);
	}
}

void update_entity_relative(EntitiesBank *entities, uint8_t id, FPVec2 *entity_pos, PixelCoords *camera_pos)
{
	entities->px[id] = entity_pos->x + new_fp16(8) - new_fp16(camera_pos->x);
	entities->py[id] = entity_pos->y + new_fp16(8) - new_fp16(camera_pos->x);
}

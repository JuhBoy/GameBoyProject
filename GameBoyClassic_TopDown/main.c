#include <gb/gb.h>
#include <stdint.h>

#include "map.h"
#include "entities.h"
#include "res/life_sprite.h"
#include "res/solider.h"
#include "res/Font.h"
#include "res/D01_Room_01_Sprites.h"
#include "res/D01_Room_01_Map.h"
#include "res/Descriptors/maps_data.h"
#include "game_state.h"

typedef struct PlayerInputs
{
	int8_t x_direction;
	int8_t y_direction;
	uint8_t state;
} PlayerInputs;

void joystick_poll(PlayerInputs *input)
{
	input->x_direction = 0;
	input->y_direction = 0;

	uint8_t input_state = joypad();
	input->state = input_state;

	if ((input_state & J_RIGHT) != 0)
	{
		input->x_direction = 1;
	}
	else if ((input_state & J_LEFT) != 0)
	{
		input->x_direction = -1;
	}

	if ((input_state & J_UP) != 0)
	{
		input->y_direction = -1;
	}
	else if ((input_state & J_DOWN) != 0)
	{
		input->y_direction = 1;
	}
}

// Entities ================
//
static PlayerInputs inputs;
static EntitiesBank entities;
static fp16_t player_velocity = 16;

// Locations ===============
//
static Vec2 player_pos = {.x = 0, .y = 0};
static MapDescriptor *des = NULL;
static MapCoords grid_coords = {.x = 0, .y = 0};
static PixelCoords cam_coords = {.x = 0, .y = 0};

// Game Systems ===========
//
void fn_update_playing(void)
{
	joystick_poll(&inputs);

	player_pos.x += player_velocity * inputs.x_direction;
	player_pos.y += player_velocity * inputs.y_direction;
	MapCoords grid_pos = snap_on_grid(fp16_to_whole16(player_pos.x), fp16_to_whole16(player_pos.y));

	entities.px[0] = player_pos.x + new_fp16(8) - (cam_coords.x << 4);
	entities.py[0] = player_pos.y + new_fp16(16) - (cam_coords.y << 4);

	draw_entities(&entities);

	if (grid_pos.x != grid_coords.x || grid_pos.y != grid_coords.y)
	{
		border_tilemap_draw(grid_coords, grid_pos, des);
		grid_coords = grid_pos;
	}
}

typedef BOOLEAN (*coroutine_action)(void);

// TODO(JUH): move this to an game_loop file
union AwaiterValues
{
	uint8_t timer_8;
	uint16_t timer_16;
};
typedef enum AwaiterType
{
	BIT_8,
	BIT_16,
} AwaiterType;
typedef struct AwaiterParams
{
	AwaiterType type;
	union AwaiterValues value;
} AwaiterParams;

void fn_start_awaiter(AwaiterParams params)
{
	BOOLEAN continue_coroutine = TRUE;

	while (continue_coroutine)
	{
		vsync();

		switch (params.type)
		{
		case BIT_8:
			params.value.timer_8 -= 1;
			continue_coroutine = params.value.timer_8 > 0;
			break;
		case BIT_16:
			params.value.timer_16 -= 1;
			continue_coroutine = params.value.timer_16 > 0;
			break;
		}
	}
}

void fn_update_transition(void)
{
	if (GAME_STATE.controllerState != LOCATION_TRANSITION)
	{
		return;
	}

	MapLocation *locations = NULL;
	uint8_t *links = NULL;
	gs_get_locations_data(GAME_STATE.map, &locations, &links);

	MapCoords prev_coords = {.x = grid_coords.x - MAP_VIEW_COLS_HALF + 2, .y = grid_coords.y - MAP_VIEW_ROWS_HALF + 2};
	transform_tile_view32(&prev_coords);

	MapLocation *new_location = &locations[GAME_STATE.location];
	uint8_t entrance_index = gs_get_entrance_index();
	grid_coords.x = new_location->entrance_x[entrance_index];
	grid_coords.y = new_location->entrance_y[entrance_index];

	player_pos.x = new_fp16(grid_coords.x * 8);
	player_pos.y = new_fp16(grid_coords.y * 8);
	cam_coords.x = player_pos.x;
	cam_coords.y = player_pos.y;

	// NOTE(JUH): hide player entity while loading next location (next -> all entities + special function to handle it)
	entities.px[0] = 0;
	entities.py[0] = 0;
	draw_entities(&entities);

	// NOTE(JUH): this is the first version of super naive awaiter 
	fill_bkg_rect(prev_coords.x, prev_coords.y, MAP_VIEW_COLS, MAP_VIEW_ROWS, 255);
	AwaiterParams awaiter_param = {
		.type = BIT_8,
		.value.timer_8 = 40, // 0.66seconds
	};
	fn_start_awaiter(awaiter_param);

	full_tilemap_draw(grid_coords, des);
	gs_set_player_state(PLAYING, TRUE);
}

void fn_update_backround(void)
{
	// player position is in subpixels coordinates, so we need to shift them to normal pixel coordinates
	cam_coords.x = fp16_to_whole16(player_pos.x);
	cam_coords.y = fp16_to_whole16(player_pos.y);
	cam_coords = move_bkg_with_coords(cam_coords);
}

// GAME LOOP =====================
//
int main(void)
{
	DISPLAY_ON;
	SHOW_SPRITES;
	SHOW_BKG;

	entities.tile_ids[0] = 0; // sprite id 0
	entities.tile_ids[1] = 1; // sprite id 1
	entities.count = 3;

	set_sprite_data(0, 1, life_sprite);
	set_sprite_data(1, 1, solider_sprite);
	init_sprites(&entities);

	Velocity vel_0 = {.x = 0, .y = 0};
	update_velocity(&entities, 0, vel_0);
	entities.px[0] = new_fp16(8);
	entities.py[0] = new_fp16(35);

	Velocity vel_1 = {.x = 0, .y = 0};
	update_velocity(&entities, 1, vel_1);
	entities.px[1] = new_fp16(8);
	entities.py[1] = new_fp16(40);

	// Initialize =========
	//
	gs_update_map(D01);
	des = gs_get_map_descriptor(GAME_STATE.map);
	MapCoords start = {.x = des->spawn_pos.x, .y = des->spawn_pos.y};

	player_pos.x = new_fp16(start.x * 8);
	player_pos.y = new_fp16(start.y * 8);

	grid_coords.x = start.x;
	grid_coords.y = start.y;
	cam_coords.x = player_pos.x;
	cam_coords.y = player_pos.y;
	full_tilemap_draw(grid_coords, des);
	cam_coords = move_bkg_with_coords(cam_coords);

	while (1)
	{
		switch (GAME_STATE.controllerState)
		{
		case PLAYING:
			fn_update_playing();
			vsync();
			break;
		case LOCATION_TRANSITION:
			fn_update_transition();
			break;
		case PAUSED:
			break;
		}

		// NOTE(JUH): this act after the game loop and is there to check if any collision happened, leading to a state change
		Vec2 grid_coors_vec = {.x = grid_coords.x, .y = grid_coords.y};
		uint8_t exit_index = gs_poll_exit_location(grid_coors_vec);
		if (exit_index > 0)
		{
			gs_update_location(exit_index);
		}

		fn_update_backround();
	}
}

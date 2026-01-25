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
#include "helpers.h"
#include "ui.h"
#include "inputs.h"
#include "res/dialogs.h"

#define HERO_DEFAULT_SPEED 16
#define HERO_HALF_SPEED 11

// Entities ================
//
static PlayerInputs inputs;
static EntitiesBank entities;
static fp16_t player_velocity = HERO_DEFAULT_SPEED;

// Locations ===============
//
static Vec2 player_pos = {.x = 0, .y = 0};
static MapDescriptor *map_desc = NULL;
static MapCoords grid_coords = {.x = 0, .y = 0};
static PixelCoords cam_coords = {.x = 0, .y = 0};

// Game Loop Systems ===========
//

// TODO(JUH): this doesn't work nice for now, it needs an implementation of get_key_down !!
void gl_state_paused(void)
{
	SHOW_WIN;
	HIDE_SPRITES;

	ui_show_pause();

	if (is_down_this_frame(&inputs, J_START))
	{
		gs_set_player_state(PLAYING, FALSE);
		SHOW_SPRITES;
		HIDE_WIN;
		return;
	}
}

void gl_state_playing(void)
{
	if (is_down_this_frame(&inputs, J_START))
	{
		gs_set_player_state(PAUSED, FALSE);
		return;
	}

	// TODO(JuH) this is for the example and need to be removed
	//
	if (is_down(&inputs, J_A))
	{
		player_velocity = 40;
	}
	else
	{
		player_velocity = HERO_DEFAULT_SPEED;
	}

	if (is_down_this_frame(&inputs, J_B))
	{
		player_pos.x += 400 * (int16_t)inputs.x_direction;
		player_pos.y += 400 * (int16_t)inputs.y_direction;
	}

	if (is_pressed(&inputs, TWO_SECS, J_SELECT_IDX))
	{
		const DialogSequence *example_seq = &hello_world_sequence;
		ui_start_dialog_seq(example_seq);

		gs_set_player_state(DIALOGUE, FALSE);
		return;
	}

	if (abs(inputs.x_direction) == 1 && abs(inputs.y_direction) == 1)
	{
		player_velocity = HERO_HALF_SPEED;
	}
	u16 new_x = player_pos.x + player_velocity * (u16)inputs.x_direction;
	u16 new_y = player_pos.y + player_velocity * (u16)inputs.y_direction;
	MapCoords grid_pos = snap_on_grid(fp16_to_whole16(new_x), fp16_to_whole16(new_y));

	entities.px[0] = player_pos.x + new_fp16(8) - new_fp16(cam_coords.x);
	entities.py[0] = player_pos.y + new_fp16(16) - new_fp16(cam_coords.y);

	player_pos.x = new_x;
	player_pos.y = new_y;

	draw_entities(&entities);

	if (grid_pos.x != grid_coords.x || grid_pos.y != grid_coords.y)
	{
		border_tilemap_draw(grid_coords, grid_pos, map_desc);
		grid_coords = grid_pos;
	}
}

void gl_state_transition(void)
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

	// NOTE(JUH): move the sprites to 0x, 0y coordinates - out of the screen for gameboy specs
	hide_entities(&entities);

	fill_bkg_rect(prev_coords.x, prev_coords.y, MAP_VIEW_COLS, MAP_VIEW_ROWS, 255);
	AwaiterParams awaiter_param = {
		.type = U8,
		.value.timer_8 = 40, // 0.66seconds
	};
	await(awaiter_param);

	full_tilemap_draw(grid_coords, map_desc);
	gs_set_player_state(PLAYING, TRUE);
}

void gl_state_dialogue(void)
{
	if (ui_wait_user_input() && !is_down_this_frame(&inputs, J_A))
	{
		return;
	}

	// NOTE(JuH): this is thread blocking for one sec, maybe remove it if I ever implements sounds
	if (ui_wait_timer())
	{
		WAIT_ONE_SECOND();
	}

	if (ui_running_seq())
	{
		ui_dialog_step();
	}

	if (UI_STATE.state == UI_NONE)
	{
		ui_end_dialog_seq();
		gs_set_player_state(PLAYING, FALSE);
	}

	vsync();
}

void gl_update_background(void)
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

	entities.tile_ids[0] = ETT_PLAYER; // sprite id 0
	entities.tile_ids[1] = ETT_GOBLIN; // sprite id 1
	entities.count = 2;

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
	ui_init();

	gs_update_map(D01);
	map_desc = gs_get_map_descriptor(GAME_STATE.map);
	MapCoords start = {.x = map_desc->spawn_pos.x, .y = map_desc->spawn_pos.y};

	player_pos.x = new_fp16(start.x * 8);
	player_pos.y = new_fp16(start.y * 8);

	grid_coords.x = start.x;
	grid_coords.y = start.y;
	cam_coords.x = player_pos.x;
	cam_coords.y = player_pos.y;
	full_tilemap_draw(grid_coords, map_desc);
	cam_coords = move_bkg_with_coords(cam_coords);

	while (1)
	{
		joystick_poll(&inputs);

		// TODO: TO REMOVE, this is for the exemple
		{
			static u8 angle = 0;

			iVec2 v = {.x = 1, .y = 0};
			iVec2 vr = rotate(v, angle, 2);

			// -128 - 127
			// remap 16px range
			vr.x = vr.x >> 2;
			vr.y = vr.y >> 2;

			entities.px[1] = new_fp16(MIDDLE_SCREEN_PXL_HOR + vr.x);
			entities.py[1] = new_fp16(MIDDLE_SCREEN_PXL_VER + 8 + vr.y);
			angle += 1;
		}

		switch (GAME_STATE.controllerState)
		{
		case PLAYING:
			gl_state_playing();
			vsync();
			break;
		case LOCATION_TRANSITION:
			gl_state_transition();
			break;
		case PAUSED:
			gl_state_paused();
			vsync();
			break;
		case DIALOGUE:
			gl_state_dialogue();
			break;
		}

		// NOTE(JUH): this act after the game loop and is there to check if any collision happened, leading to a state change
		Vec2 grid_coors_vec = {.x = grid_coords.x, .y = grid_coords.y};
		uint8_t exit_index = gs_poll_exit_location(grid_coors_vec);
		if (exit_index > 0)
		{
			gs_update_location(exit_index);
		}

		gl_update_background();
		ui_draw();
	}
}

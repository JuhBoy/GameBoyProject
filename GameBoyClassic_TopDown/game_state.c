#include "game_state.h"

// Global state
GameState GAME_STATE = {
	.map = NONE,
	.location = 0,
	.transitionState = 0,
	.controllerState = STARTING,
	.cameraTypeFollow = TRUE};

// Controllers ==========
void gs_set_player_state(ControllerState state, BOOLEAN reset_transition)
{
	GAME_STATE.controllerState = state;
	if (reset_transition)
	{
		GAME_STATE.transitionState = 0;
	}
}

ControllerState gs_get_player_state(void)
{
	return GAME_STATE.controllerState;
}

// Locations & Maps ==========

MapDescriptor *gs_get_map_descriptor(Maps map)
{
	switch (map)
	{
	case ENTRY:
		break;
	case D01:
		return &D01_MAP_DESC;
		break;
	}

	return NULL;
}

void gs_update_map(Maps map)
{
	Maps current = GAME_STATE.map;

	GAME_STATE.map = map;
	GAME_STATE.location = 0;
	GAME_STATE.transitionState = 0;
	GAME_STATE.controllerState = PLAYING;

	MapDescriptor *des = gs_get_map_descriptor(map);
	MapCoords spawn = {.x = des->spawn_pos.x, .y = des->spawn_pos.y};

	// NOTE(JUH): reserve one square black for transition at last index of VRAM bank
	set_bkg_data(0, des->tiles_count, des->map_tiles);
	set_bkg_data(255, 1, black_square_sprite);
}

void gs_update_location(uint8_t exit_index)
{
	MapLocation *locations = NULL;
	uint8_t *links = NULL;
	gs_get_locations_data(GAME_STATE.map, &locations, &links);

	// NOTE(JUH): make the index zero based for convinience, this leave the exit_index==0 be the default "NONE"
	uint8_t nextLocation = links[GAME_STATE.location * 6 + exit_index] - 1;

	Vec2 pos = locations[GAME_STATE.location].pos;
	Vec2 npos = locations[nextLocation].pos;

	uint8_t diff_x = pos.x > npos.x ? pos.x - npos.x : npos.x - pos.x;
	uint8_t diff_y = pos.y > npos.y ? pos.y - npos.y : npos.y - pos.y;

	GAME_STATE.location = nextLocation;

	// Trigger a transition if the Room transition imply a teleport
	if (diff_x > 9 || diff_y > 9)
	{
		GAME_STATE.controllerState = LOCATION_TRANSITION;
	}

	GAME_STATE.transitionState = 1 << exit_index;
}

void gs_get_locations_data(Maps map, const MapLocation **map_location, const uint8_t **links)
{
	switch (map)
	{
	// TODO(JUH): implements ENTRY when map changes will be implemented.
	case ENTRY:
	case D01:
		*map_location = D01_Locations;
		*links = D01_Location_links;
		return;
	}

	map_location = NULL;
	links = NULL;
}

/// @brief check if the position match one of the current location exit and return it
/// @return uin8_t zero based index of the exit matching the given position
uint8_t gs_poll_exit_location(Vec2 position)
{
	MapLocation *locations = NULL;
	uint8_t *links = NULL;
	gs_get_locations_data(GAME_STATE.map, &locations, &links);

	MapLocation *location = &locations[GAME_STATE.location];

	for (uint8_t i = 0; i < 6; i++)
	{
		uint8_t x = location->exits_x[i];
		uint8_t y = location->exits_y[i];
		uint8_t max_x = x + 1;
		uint8_t max_y = y + 1;

		if ((position.x >= x && position.x <= max_x) && (position.y >= y && position.y <= max_y))
		{
			return i;
		}
	}

	return 0;
}

uint8_t gs_get_entrance_index(void)
{
	switch (GAME_STATE.transitionState)
	{
	case (1 << 0):
		return 0;
	case (1 << 1):
		return 1;
	case (1 << 2):
		return 2;
	case (1 << 3):
		return 3;
	case (1 << 4):
		return 4;
	case (1 << 5):
		return 5;
	}

	return 0;
}
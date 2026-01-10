#ifndef __GAME_STATE
#define __GAME_STATE

#include "map.h"
#include "res/Descriptors/maps_data.h"
#include "res/black_square_sprite.h"

typedef enum ControllerState
{
	STARTING,
	LOADING,
	PLAYING,
	LOCATION_TRANSITION,
	DIALOGUE,
	PAUSED,
} ControllerState;

typedef enum Maps
{
	NONE,
	SPLASH_SCREEN,
	ENTRY,
	D01,
} Maps;

typedef struct GameState
{
	Maps map;
	uint8_t location;
	uint8_t transitionState; // 1 bit per 6 transitions possible (left,right,top,bot,up,down)
	ControllerState controllerState;
	BOOLEAN cameraTypeFollow;
} GameState;

extern GameState GAME_STATE;

// Maps ============
void gs_update_map(Maps map);
MapDescriptor *gs_get_map_descriptor(Maps map);

// Locations ======
void gs_get_locations_data(Maps map, MapLocation **map_locations, uint8_t **links);
void gs_update_location(uint8_t nextLocation);
uint8_t gs_poll_exit_location(Vec2 ett_pos);
uint8_t gs_get_entrance_index(void);

// Controller ======
void gs_set_player_state(ControllerState state, BOOLEAN reset_transition);
void gs_toggle_pause_state(ControllerState otherState);
ControllerState gs_get_player_state(void);

#endif

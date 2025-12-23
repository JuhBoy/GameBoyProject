#include "maps_data.h"

/// PLAN =================
//

/// D01 ==================
//
extern const MapLocation D01_Locations[] = {
		// First Room of the game
		{
			.pos = {.x = 40, .y = 18},
			.exits_x = {0, 0, 0, 0, 0, 56},
		 	.exits_y = {0, 0, 0, 0, 0, 30},
			.entrance_x = {0, 0, 0, 0, 55, 0},
			.entrance_y = {0, 0, 0, 0, 30, 0}},

		// TP map left bottom
		{
			.pos = {.x = 108, .y = 110},
			.exits_x = {0, 0, 0, 0, 114, 0},
		 	.exits_y = {0, 0, 0, 0, 112, 0},
			.entrance_x = {0, 0, 0, 0, 0, 114},
			.entrance_y = {0, 0, 0, 0, 0, 114}},
	};

extern const uint8_t D01_Location_links[] = {
	// First Room
	0, 0, 0, 0, 0, 2,
	// Tp map Left bottom
	0, 0, 0, 0, 1, 0,
};

extern const MapDescriptor D01_MAP_DESC = {
	.size = D01_Room_01_MapWidth,
	.tiles_count = 60,
	.map_tiles = D01_Room_01_Sprites,
	.map_data = D01_Room_01_Map,
	.camera_position = 0,
	.spawn_pos = {.x = 44, .y = 20},
};
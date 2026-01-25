#ifndef __map_INCLUDE
#define __map_INCLUDE

#include "gb/gb.h"
#include <stdint.h>
#include "math.h"

#define MAP_VIEW_ROWS (uint16_t)20			// 2 columns wider than screen view
#define MAP_VIEW_COLS (uint16_t)22			// 2 columns wider than screen view
#define MAP_VIEW_ROWS_HALF (uint16_t)10 // 2 columns wider than screen view
#define MAP_VIEW_COLS_HALF (uint16_t)11 // 2 columns wider than screen view
#define MAP_ROWS_COLS (MAP_VIEW_COLS * MAP_VIEW_ROWS)

#define MAP_MAX_Y_TILE (uint8_t)110
#define MAP_MIN_Y_TILE (uint8_t)0

#define MAP_MAX_X_TILE (uint8_t)108
#define MAP_MIN_X_TILE (uint8_t)0

typedef struct MapLocation
{
	Vec2 pos;
	uint8_t exits_x[6];
	uint8_t exits_y[6];
	uint8_t entrance_x[6];
	uint8_t entrance_y[6];
} MapLocation;

typedef struct MapIndex
{
	uint8_t row;
	uint8_t col;
} MapIndex;

typedef struct MapCoords
{
	fp16_t x;
	fp16_t y;
} MapCoords;

typedef struct MapDescriptor
{
	uint16_t size;
	uint8_t tiles_count;
	const unsigned char *map_data;
	const unsigned char *map_tiles;

	Vec2 spawn_pos;
	uint8_t camera_position;
} MapDescriptor;

static uint8_t view_layout[MAP_ROWS_COLS];

uint16_t to_array_index(const MapIndex mapIndex);
MapIndex to_map_index(uint16_t array_index);
MapCoords snap_on_grid(fp16_t x, fp16_t y);

// DRAWERS =======
void border_tilemap_draw(MapCoords previous, MapCoords next, MapDescriptor *map);
void full_tilemap_draw(MapCoords coords, MapDescriptor *map);
void clear_tilemap_view(MapCoords coords, uint8_t w, uint8_t h);

// HELPERS ======
PixelCoords move_bkg_with_coords(PixelCoords coords);
void tr_world_to_draw_space(MapCoords *coords);
void transform_tile_view32(MapCoords *coords);

#endif
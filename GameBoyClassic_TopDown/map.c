#include "map.h"

uint16_t to_array_index(const MapIndex mapIndex)
{
	uint16_t ouput = (uint16_t)mapIndex.row * MAP_VIEW_COLS;
	return ouput + mapIndex.col;
}

MapIndex to_map_index(const uint16_t array_index)
{
	uint16_t row = array_index / MAP_VIEW_COLS;
	uint16_t col = array_index % MAP_VIEW_COLS;

	MapIndex output = {
			.row = row,
			.col = col};

	return output;
}

MapCoords snap_on_grid(fp16_t x, fp16_t y)
{
	fp16_t xx = fp16_div8(x);
	fp16_t yy = fp16_div8(y);

	MapCoords coords = {
			.x = xx,
			.y = yy,
	};

	return coords;
}

void transform_tile_view32(MapCoords *coords)
{
	coords->x = fp16_mod32(coords->x);
	coords->y = fp16_mod32(coords->y);
}

void tr_world_to_draw_space(MapCoords *coords)
{
	if (coords->x > MAP_VIEW_COLS_HALF - 2)
	{
		coords->x = (coords->x - MAP_VIEW_COLS_HALF + 2);
	}
	else
	{
		coords->x = 0;
	}

	if (coords->y > MAP_VIEW_ROWS_HALF - 2)
	{
		coords->y = (coords->y - MAP_VIEW_ROWS_HALF + 2);
	}
	else
	{
		coords->y = 0;
	}
}

void tr_tile_to_pixels(MapCoords *coords)
{
	coords->x *= 8;
	coords->y *= 8;
}

void tr_to_map_boundaries(MapCoords *coords)
{
	coords->x = coords->x > MAP_MAX_X_TILE ? MAP_MAX_X_TILE : coords->x;
	coords->y = coords->y > MAP_MAX_Y_TILE ? MAP_MAX_Y_TILE : coords->y;
}

void clear_tilemap_view(MapCoords coords, uint8_t w, uint8_t h)
{
	transform_tile_view32(&coords);
	fill_bkg_rect(coords.x, coords.y, w, h, 0x00);
}

PixelCoords move_bkg_with_coords(PixelCoords coords)
{
	static const fp16_t x_ext = MAP_VIEW_COLS_HALF * 8 - 16;
	static const fp16_t y_ext = MAP_VIEW_ROWS_HALF * 8 - 16;
	static const fp16_t x_max = MAP_MAX_X_TILE * 8;
	static const fp16_t y_max = MAP_MAX_Y_TILE * 8;

	coords.x = (coords.x > x_ext) ? coords.x - x_ext : 0;
	coords.y = (coords.y > y_ext) ? coords.y - y_ext : 0;

	coords.x = coords.x > x_max ? x_max : coords.x;
	coords.y = coords.y > y_max ? y_max : coords.y;

	move_bkg(coords.x, coords.y);

	return coords;
}

void border_tilemap_draw(MapCoords previous, MapCoords next, MapDescriptor *map)
{
	int8_t horDir = (int16_t)next.x - (int16_t)previous.x;
	int8_t verDir = (int16_t)next.y - (int16_t)previous.y;

	tr_world_to_draw_space(&next);
	tr_to_map_boundaries(&next);

	if (horDir != 0)
	{
		if (horDir == 1)
		{
			set_bkg_submap(next.x + MAP_VIEW_COLS - 1, next.y, 2, MAP_VIEW_ROWS, map->map_data, map->size);
		}
		else
		{
			set_bkg_submap(next.x - 1, next.y, 2, MAP_VIEW_ROWS, map->map_data, map->size);
		}
	}
	if (verDir != 0)
	{
		if (verDir == 1)
		{
			set_bkg_submap(next.x, next.y + MAP_VIEW_ROWS - 1, MAP_VIEW_COLS, 2, map->map_data, map->size);
		}
		else
		{
			int16_t y = ((int16_t)next.y) - 1;
			set_bkg_submap(next.x, next.y - 1, MAP_VIEW_COLS, y < 0 ? 1 : 2, map->map_data, map->size);
		}
	}
}

void full_tilemap_draw(MapCoords coords, MapDescriptor *map)
{
	tr_world_to_draw_space(&coords);
	tr_to_map_boundaries(&coords);
	set_bkg_submap(coords.x, coords.y, MAP_VIEW_COLS, MAP_VIEW_ROWS, map->map_data, map->size);

	tr_tile_to_pixels(&coords);
	move_bkg(coords.x, coords.y);
}
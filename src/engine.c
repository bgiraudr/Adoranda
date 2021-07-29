#include <gint/display.h>
#include <gint/keyboard.h>
#include "engine.h"
#include "game.h"
#include "map.h"
#include "player.h"

#define TILESET_WIDTH 29

void engine_draw(struct game const *game) {
	dclear(C_WHITE);
	for(int y = 0; y < game->map->h; y++) {
		for(int x = 0; x < game->map->w; x++) {
			int tile = game->map->tiles[y * game->map->w + x] - 1;
			if(tile != -1) {
				int tile_x = tile % TILESET_WIDTH;
				int tile_y = tile / TILESET_WIDTH;
				//dprint(x * 55, y * 16, C_BLACK,"%d(%d:%d)",tile, tile_x, tile_y);

				dsubimage(x * 16, y * 16, game->map->tileset, tile_x * 16, tile_y * 16, 16, 16, DIMAGE_NONE);
			}
		}
	}
	engine_draw_player(game->player);
}

void engine_draw_player(struct player const *player) {
	extern bopti_image_t img_spritesheet;
	dsubimage(12 * 16, 7 * 16 - 5, &img_spritesheet, player->direction * 16, 0, 16, 21, DIMAGE_NONE);
}

void engine_move(struct player *player, int direction) {
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);

	player->direction = direction;
	player->x += dx;
	player->y += dy;
}
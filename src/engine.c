#include <gint/display.h>
#include <gint/keyboard.h>
#include "engine.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "animation.h"

#define TILESET_WIDTH 29
#define HEIGHT_VISIBLE 14
#define WIDTH_VISIBLE 24

/*void engine_draw(struct game const *game) {
	dclear(C_BLACK);
	for(int y = 0; y < HEIGHT_VISIBLE; y++) {
		for(int x = 0; x < WIDTH_VISIBLE; x++) {
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
}*/

void engine_draw(struct game const *game) {
	dclear(C_WHITE);
	int j = 0;
	int x = 0;

	int level_width = game->map->w;
	int taillemap = game->map->h*game->map->w;
	int player_view = 12;

	int coomap = game->player->x + game->player->y*level_width;
	int beginmap = coomap - player_view - level_width*player_view;

	int beginline = coomap - player_view * level_width - coomap%level_width; 

	if(beginmap < 0) {
		beginmap = game->player->x - player_view;
		if(beginmap < 0) beginmap = 0;
	}

	if((coomap - player_view)%level_width > coomap%level_width) {
		if(beginline > 0) {
			beginmap = beginline;
		} else {
			beginmap = 0;
		}
	}

	int next = game->player->x + player_view + beginmap/level_width * level_width;
	int endline = beginmap+level_width-beginmap%level_width-1;

	if(next > endline) next = endline;

	if(next < 0) {
		next = game->player->x + player_view;
	}

	int indypro = game->player->y + player_view;
	if(indypro > taillemap/level_width-1) {
		indypro = taillemap/level_width-1;
	}
	int endmap = next%level_width + level_width*indypro;

	int ecart = next - beginmap;
	int xcentre = 0, ycentre = 0;

	for(int i = beginmap; i <= endmap; i++) {
		x++;
		if(i == coomap) {xcentre = x-1; ycentre = j; break;}
		if(i == beginmap + ecart + j * level_width) {
			j++;
			x=0;
			i = beginmap + j*level_width-1;
		}
	}

	j=0;
	x=12-xcentre;
	for(int i = beginmap; i <= endmap; i++) {
		int y = j + (7-ycentre);
		
		for(int layer = 0; layer < game->map->nb_layer; layer++) {
			int tile;
			tile = layer == 0 ? game->map->tiles_layer1[i] - 1 : game->map->tiles_layer2[i] - 1;
			if(tile != -1) {
				int tile_x = tile % TILESET_WIDTH;
				int tile_y = tile / TILESET_WIDTH;
				//dprint(x * 55, y * 16, C_BLACK,"%d(%d:%d)",tile, tile_x, tile_y);

				dsubimage(x * 16, y * 16, game->map->tileset, tile_x * 16, tile_y * 16, 16, 16, DIMAGE_NONE);
			}
		}

		x++;
		if(i==beginmap+ecart+j*level_width) {
			j++;
			x=12-xcentre;
			i = beginmap + j*level_width-1;
		}
	}
	engine_draw_player(game->player);
}

void engine_draw_player(struct player const *player) {
	dframe(12 * 16, 7 * 16 - 5, player->anim.img);
	dprint(1,1,C_BLACK,"%d:%d",player->x, player->y);
}

void engine_move(struct game *game, int direction) {
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);

	if(game->player->direction == direction) {
		if(map_walkable(game->map, game->player->x + dx, game->player->y + dy)) {
			game->player->x += dx;
			game->player->y += dy;
		} else {
			game->player->idle = !anim_player_idle(&game->player->anim, 1);
		}
	} else {
		game->player->direction = direction;
		game->player->anim.dir = direction;
	}
}

void engine_tick(struct game *game, int dt)
{
    game->player->anim.duration -= dt;

    /* Call the animation function to generate the next frame */
    if(game->player->anim.duration <= 0) {
    	game->player->idle = !game->player->anim.function(&game->player->anim, 0);
	}
}

int map_walkable(struct map const *map, int x, int y)
{
    int tile = map->info_map[y * map->w + x];
    return (tile != TILE_SOLID);
}
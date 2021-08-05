#include <gint/display.h>
#include <gint/keyboard.h>
#include "engine.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "animation.h"
#include "define.h"

#define TILESET_WIDTH 29

void engine_draw(struct game const *game) {
	dclear(game->background);
	engine_draw_map_around_player(game);
	engine_draw_player(game->player);
}

void engine_draw_map_around_player(struct game const *game) {
	const int level_width = game->map->w;
	const int taillemap = game->map->w * game->map->h;
	const int posx = game->player->show_x;
	const int posy = game->player->show_y;
	
	const int coo_player_map = game->player->x + game->player->y*level_width; //the index of the player on the map
	int beginmap = coo_player_map - PLAYER_VIEW_X - level_width*PLAYER_VIEW_Y; //compute the theorical top left corner
	int beginline = coo_player_map - PLAYER_VIEW_Y*level_width - coo_player_map%level_width; //index of the tile wich begin the row

	//anti begin oob
	if(beginmap < 0) {
		beginmap = game->player->x - PLAYER_VIEW_X;
		if(beginmap < 0) beginmap = 0;
	}

	//anti horizontal oob
	if((coo_player_map - PLAYER_VIEW_X)%level_width > coo_player_map%level_width) {
		beginmap = beginline > 0 ? beginline : 0;
	}

	int next = game->player->x + PLAYER_VIEW_X + beginmap/level_width * level_width; //index of the top right corner
	int endline = beginmap+level_width-beginmap%level_width-1; //index of the tile wich end the row

	if(next > endline) next = endline;
	if(next < 0) next = game->player->x + PLAYER_VIEW_X;

	int indypro = game->player->y + PLAYER_VIEW_Y; //y value of the bottom row
	//anti bottom oob
	if(indypro > taillemap/level_width-1) indypro = taillemap/level_width-1;

	int endmap = next%level_width + level_width*indypro; //index of the bottom right corner
	int ecart = next - beginmap; //number of column to draw

	//player x, y on the compute map
	int xcentre = 0, ycentre = 0;

	//calculate the player position in the generated map
	for(int i = beginmap; i < coo_player_map; i++) {
		xcentre++;

		if(i == next + ycentre * level_width) {
			ycentre++;
			xcentre=0;
			i = beginmap + ycentre*level_width-1;
		}
	}

	int j = 0;
	int x = posx-xcentre;
	for(int i = beginmap; i <= endmap; i++) {
		int y = j + (posy-ycentre);
		
		for(int layer = 0; layer < game->map->nb_layer; layer++) {
			int tile;
			tile = layer == 0 ? game->map->tiles_layer1[i] - 1 : game->map->tiles_layer2[i] - 1;
			if(tile != -1) {
				int tile_x = tile % TILESET_WIDTH;
				int tile_y = tile / TILESET_WIDTH;
				dsubimage(x * 16 - game->player->anim.dx * 3, y * 16 - game->player->anim.dy * 3,
				 game->map->tileset, tile_x * 16, tile_y * 16, 16, 16, DIMAGE_NONE);
			}
		}

		x++;
		if(i==beginmap+ecart+j*level_width) {
			j++;
			x=posx-xcentre;
			i = beginmap + j*level_width-1;
		}
	}
}

void engine_draw_player(struct player const *player) {
	dframe(player->show_x * 16, player->show_y * 16 - 5, player->anim.img); //draw the player 5 pixel up
	dprint(1,1,C_BLACK,"%d:%d",player->x, player->y);
}

int engine_move(struct game *game, int direction) {
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);

	if(!game->player->idle) return 0;

	if(game->player->direction == direction) {
		if(map_walkable(game->map, game->player->x + dx, game->player->y + dy)) {
			game->player->x += dx;
			game->player->y += dy;
			game->player->idle = !anim_player_walking(&game->player->anim, 1);
		} else {
			game->player->idle = !anim_player_idle(&game->player->anim, 1);
		}
	} else {
		game->player->direction = direction;
		game->player->anim.dir = direction;
	}
	return 1;
}

void engine_tick(struct game *game, int dt) {
	game->player->anim.duration -= dt;

	if(game->player->anim.duration <= 0) {
		game->player->idle = !game->player->anim.function(&game->player->anim, 0);
	}
}

int map_walkable(struct map const *map, int x, int y) {
	int tile = map->info_map[x + map->w * y];
	if(x < 0 || x > map->w-1 || y < 0 || y > map->h) return 0;
	return (tile != TILE_SOLID);
}

void engine_set_background(struct game *game, int color) {
	game->background = color;
}
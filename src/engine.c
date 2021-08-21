#include <gint/display.h>
#include <gint/keyboard.h>
#include "engine.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "animation.h"
#include "define.h"
#include "character.h"
#include "camera.h"

/*draw the current state of the game*/
void engine_draw(struct Game const *game) {
	dclear(game->background);
	engine_draw_map(game);
	engine_draw_player(game);
	dprint(1,15,C_BLACK,"%d:%d",game->camera->x, game->camera->y);
	dprint(1,30,C_BLACK,"%d:%d",game->map->w, game->map->h);
}

void engine_draw_map(struct Game const *game) {
	int x_offset = (game->camera->x - DWIDTH/2);
	int y_offset = (game->camera->y - DHEIGHT/2);

	//currently -1 to avoid white during transition
	for (int layer = 0 ; layer < game->map->nb_layers; layer++) {
		for (int y = -1 ; y <= DHEIGHT / TILE_SIZE + 1; y++) {
			for (int x = -1 ; x <= DWIDTH / TILE_SIZE + 1; x++) {
				unsigned int tile_id = 0;
				//detect if the map is oob
				int indexY = (y + y_offset / TILE_SIZE);
				int indexX = (x + x_offset / TILE_SIZE);
				if(indexX >= 0 && indexX < game->map->w
					&& indexY >= 0 && indexY < game->map->h) 
					tile_id = game->map->layers[layer][indexX + indexY * game->map->w];
				//tile_id = game->map->layers[layer][(x + x_offset / TILE_SIZE) + (y + y_offset / TILE_SIZE) * game->map->w];
				
				if (tile_id != 0) {
					tile_id--;
					unsigned int tile_x = TILE_SIZE * (tile_id % TILESET_WIDTH);
					unsigned int tile_y = TILE_SIZE * (tile_id / TILESET_WIDTH);

					//provisoire le temps de trouver une manière propre
					dsubimage(x * TILE_SIZE - x_offset%TILE_SIZE - game->player->anim.dx * 3, 
						y * TILE_SIZE - y_offset%TILE_SIZE - game->player->anim.dy * 3, game->map->tileset, 
						tile_x, tile_y, TILE_SIZE, TILE_SIZE, DIMAGE_NONE);
				}
			}
		}
	}

}

/*draw the player*/
void engine_draw_player(struct Game const *game) {
	if(game->map->w > DWIDTH / TILE_SIZE + 1 &&
				game->map->h > DHEIGHT / TILE_SIZE + 1) {
		dframe(game->player->show_x * 16, game->player->show_y * 16 - 5, game->player->anim.img); //draw the player 5 pixel up
	} else {
		int offset_map_x = (DWIDTH / TILE_SIZE - game->map->w + 1)/2;
		int offset_map_y = (DHEIGHT / TILE_SIZE - game->map->h + 1)/2;
		dframe((game->player->x + offset_map_x) * 16, 
		(game->player->y + offset_map_y) * 16 - 5, game->player->anim.img); //draw the player 5 pixel up
	}
	dprint(1,1,C_BLACK,"%d:%d",game->player->x, game->player->y);
}

/*move the player to the direction*/
int engine_move(struct Game *game, int direction) {
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);

	if(!game->player->idle) return 0;

	if(game->player->direction == direction) {
		if(map_walkable(game->map, game->player->x + dx, game->player->y + dy)) {
			game->player->x += dx;
			game->player->y += dy;

			if(game->map->w > DWIDTH / TILE_SIZE + 1 &&
				game->map->h > DHEIGHT / TILE_SIZE + 1) {
				game->camera->x += dx*16;
				game->camera->y += dy*16;
			}

			game->player->idle = !anim_player_walking(&game->player->anim, 1);
			engine_check_position(game);
		} else {
			game->player->idle = !anim_player_idle(&game->player->anim, 1);
		}
	} else {
		game->player->direction = direction;
		game->player->anim.dir = direction;
	}
	return 1;
}

/*update the player animation*/
void engine_tick(struct Game *game, int dt) {
	game->player->anim.duration -= dt;
	if(game->player->anim.duration <= 0) {
		game->player->idle = !game->player->anim.function(&game->player->anim, 0);
	}
}

/*set the background color*/
void engine_set_background(struct Game *game, int color) {
	game->background = color;
}

/*make an interaction with something*/
void engine_action(struct Game const *game, int action) {
	if(action == ACTION_SHIFT) {
		if(player_facing(game) == TILE_CHARACTER) {
			int direction = game->player->direction;
			int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
			int dy = (direction == DIR_DOWN) - (direction == DIR_UP);
			draw_dialog(get_character_xy(game->characters, game->player->x + dx, game->player->y + dy));
		}
	}
}

/*check the current position of the player. To perform action depends of his location*/
void engine_check_position(struct Game *game) {
	int player_curr_tile = map_get_player_tile(game);
	if(player_curr_tile == TILE_DOOR) {
		engine_set_background(game, C_BLACK);
		generate_interior_map(game);
	} else {
		engine_set_background(game, C_WHITE);
	}
}
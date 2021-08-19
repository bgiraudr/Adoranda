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
	//engine_draw_map_around_player(game);
	engine_draw_map(game);
	engine_draw_player(game->player);
}

/*draw the map around the player*/
void engine_draw_map_around_player(struct Game const *game) {
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
		
		for(int layer = 0; layer < game->map->nb_layers; layer++) {
			int tile = game->map->layers[layer][i]-1;
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

void engine_draw_map(struct Game const *game) {
	int x_offset = (game->camera->x - DWIDTH/2);
	int y_offset = (game->camera->y - DHEIGHT/2);

	//currently -1 to avoid white during transition
	for (int layer = 0 ; layer < game->map->nb_layers; layer++) {
		for (int y = -1 ; y <= DHEIGHT / TILE_SIZE + 1; y++) {
			for (int x = -1 ; x <= DWIDTH / TILE_SIZE + 1; x++) {
				unsigned int tile_id = 0;
				if(game->map->w > x && game->map->h > y) {
					//detect if the map is oob
					int indexY = (y + y_offset / TILE_SIZE);
					int indexX = (x + x_offset / TILE_SIZE);
					if(indexX >= 0 && indexX < game->map->w
						&& indexY >= 0 && indexY < game->map->h) 
						tile_id = game->map->layers[layer][indexX + indexY * game->map->w];
				}
				
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
void engine_draw_player(struct Player const *player) {
	dframe(player->show_x * 16, player->show_y * 16 - 5, player->anim.img); //draw the player 5 pixel up
	dprint(1,1,C_BLACK,"%d:%d",player->x, player->y);
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

			game->camera->x += dx*16;
			game->camera->y += dy*16;

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
	vec_lerp(game->camera, game->player, 0.05);
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
	} else {
		engine_set_background(game, C_WHITE);
	}
}

void vec_lerp(struct Camera *from, struct Player const *to, float scale) {
	from->x = from->x * (1 - scale) + (to->x*TILE_SIZE + to->x_mid) * scale;
	from->y = from->y * (1 - scale) + (to->y*TILE_SIZE + to->y_mid) * scale;
}
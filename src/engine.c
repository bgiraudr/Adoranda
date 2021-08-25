#include <gint/display.h>
#include <gint/keyboard.h>
#include "engine.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "animation.h"
#include "define.h"
#include "talkable.h"
#include "camera.h"
#include "vec2.h"

/*draw the current state of the game*/
void engine_draw(struct Game const *game) {
	dclear(game->background);
	engine_draw_map(game);
	engine_draw_player(game);
	dprint(1,15,C_BLACK,"%d:%d",game->camera.offset.x, game->camera.offset.y);
	dprint(1,30,C_BLACK,"%d:%d",game->map->w, game->map->h);
}

void engine_draw_map(struct Game const *game) {
	int x_offset = (game->camera.offset.x - DWIDTH/2);
	int y_offset = (game->camera.offset.y - DHEIGHT/2);

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

					dsubimage(x * TILE_SIZE - x_offset%TILE_SIZE, 
						y * TILE_SIZE - y_offset%TILE_SIZE, game->map->tileset, 
						tile_x, tile_y, TILE_SIZE, TILE_SIZE, DIMAGE_NONE);
				}
			}
		}
	}

}

/*draw the player*/
void engine_draw_player(struct Game const *g) {
	struct Vec2f draw_offset = vec2f_sub(g->player->pos_visual, vec2_vec2f(g->camera.offset));
	draw_offset.x += g->player->anim.dx * 3;
	draw_offset.y += g->player->anim.dy * 3;

	dframe(g->player->show_x * TILE_SIZE + draw_offset.x + g->player->x_mid, 
	g->player->show_y * TILE_SIZE - 5 + draw_offset.y + g->player->y_mid, 
	g->player->anim.img); //draw the player 5 pixel up

	dprint(1,1,C_BLACK,"%d:%d",g->player->pos.x, g->player->pos.y);
}

/*move the player to the direction*/
int engine_move(struct Game *game, int direction) {
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);

	if(!game->player->idle) return 0;

	if(game->player->direction == direction) {
		if(map_walkable(game->map, game->player->pos.x + dx, game->player->pos.y + dy)) {
			game->player->pos.x += dx;
			game->player->pos.y += dy;

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

void engine_tick(struct Game *g, int dt) {
	/* update the player animation */
	g->player->anim.duration -= dt;
	if(g->player->anim.duration <= 0) {
		g->player->idle = !g->player->anim.function(&g->player->anim, 0);
	}
	/* update camera */
	g->player->pos_visual = vec2_vec2f(vec2_mul(g->player->pos, TILE_SIZE));
	camera_update(&g->camera);
}

/*set the background color*/
void engine_set_background(struct Game *game, int color) {
	game->background = color;
}

/*make an interaction with something*/
void engine_action(struct Game const *game, int action) {
	if(action == ACTION_SHIFT) {
		if(player_facing(game) == TILE_TALKABLE) {
			int direction = game->player->direction;
			int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
			int dy = (direction == DIR_DOWN) - (direction == DIR_UP);
			draw_dialog(get_dialog_xy(game->map, game->player->pos.x + dx, game->player->pos.y + dy));
		}
	}
}

/*check the current position of the player. To perform action depends of his location*/
void engine_check_position(struct Game *game) {
	int player_curr_tile = map_get_player_tile(game);
	if(player_curr_tile == TILE_DOOR_IN) {
		engine_set_background(game, C_BLACK);
		generate_interior_map(game);
	}
	if(player_curr_tile == TILE_DOOR_OUT) {
		engine_set_background(game, C_WHITE);
	}
}

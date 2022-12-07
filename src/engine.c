#include <gint/display.h>
#include <gint/keyboard.h>
#include <string.h>
#include <gint/std/stdlib.h>
#include <gint/rtc.h>

#include "engine.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "animation.h"
#include "define.h"
#include "talkable.h"
#include "camera.h"
#include "vec2.h"
#include "battle.h"
#include "capacite.h"
#include "util.h"
#include "inventory.h"
#include "item.h"
#include "event.h"
#include "type.h"

/*draw the current state of the game*/
void engine_draw(struct Game const *game) {
	dclear(game->background);
	engine_draw_map(game);
	engine_draw_player(game);
	/*drect(0,0,DWIDTH,20,C_BLACK);
	drect(0,DHEIGHT-20,DWIDTH,DHEIGHT,C_BLACK);
	drect(0,0,20,DHEIGHT,C_BLACK);
	drect(DWIDTH-20,0,DWIDTH,DHEIGHT,C_BLACK);*/
	
	dprint(1,1,C_WHITE,"%d:%d",game->player->pos.x, game->player->pos.y);
	dprint(1,20,C_WHITE,"%d",game->player->sprint);
	dprint(1,40,C_WHITE,"%d",get_nb_eventdialog(game->player));
	dprint(1,60,C_WHITE,"%d",game->player->eventListDialog[get_nb_eventdialog(game->player)-1 > 0 ? get_nb_eventdialog(game->player)-1 : 0]);
}

void engine_draw_map(struct Game const *game) {
	const int x_offset = (game->camera.offset.x - DWIDTH/2);
	const int y_offset = (game->camera.offset.y - DHEIGHT/2);
	const int tileset_size = game->map->tileset_size;

	for (int y = 0 ; y <= DHEIGHT / TILE_SIZE; y++) {
		for (int x = 0 ; x <= DWIDTH / TILE_SIZE+1; x++) {
			for (int layer = 0 ; layer < game->map->nb_layers; layer++) {
				unsigned int tile_id = 0;
				//detect if the map is oob
				const int indexY = (y + y_offset / TILE_SIZE);
				const int indexX = (x + x_offset / TILE_SIZE);
				if(indexX >= 0 && indexX < game->map->w
					&& indexY >= 0 && indexY < game->map->h)
					tile_id = game->map->layers[layer][indexX + indexY * game->map->w];

				if (tile_id != 0) {
					tile_id--;
					unsigned int tile_x = TILE_SIZE * (tile_id % tileset_size);
					unsigned int tile_y = TILE_SIZE * (tile_id / tileset_size);

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
	srand(rtc_ticks());
	int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
	int dy = (direction == DIR_DOWN) - (direction == DIR_UP);

	if(!game->player->idle) return 0;

	if(game->player->direction == direction) {
		if(map_walkable(game->map, game->player->pos.x + dx, game->player->pos.y + dy)) {
			game->player->pos.x += dx;
			game->player->pos.y += dy;

			if(game->player->sprint) {
				game->player->idle = !anim_player_sprinting(&game->player->anim, 1);
			} else {
				game->player->idle = !anim_player_walking(&game->player->anim, 1);
			}
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
void engine_action(struct Game *game, int action) {
	if(action == ACTION_SHIFT) {
		if(player_facing(game) == TILE_TALKABLE) {
			draw_dialog(game);
		}
	}
	if(action == ACTION_F1) {
		/*game->player->sprint = game->player->sprint ? 0 : 1;
		add_move(game->player, get_move_id(3));*/
		open_inventory(game, &game->player->inventory, "Consultation", true);
	}
	// if(action == ACTION_F2) {
	// 	//add_item_to_inventory(game, &game->player->inventory, get_item_id(1));
	// 	// srand(game->player->pos.x * game->player->pos.y);
	// 	drawTypeEffects(getTypeFromId(rand_range(1,5)));
	// }
	if(action == ACTION_OPTN) {
		draw_stats(game->player->stats);
		draw_player_moves(game->player);
		dupdate();
		wait_for_input(KEY_OPTN);
	}
}

/*check the current position of the player. To perform action depends of his location*/
void engine_check_position(struct Game *game) {
	static struct Map *old_map;
	static struct Vec2 old_pos;
	if(is_in_zone(game->player, game->map)) {
		struct Zone zone = get_zone(game->player, game->map);
		if(!check_eventzone(game->player, zone.id)) {
			if(strcmp(zone.event, "")) {
				if(handle_event(game, zone.event)) {
					addZoneToPlayer(game->player, zone);
				}
			}
		}
	}

	int player_curr_tile = map_get_player_tile(game);
	if(player_curr_tile == TILE_DOOR_IN) {
		old_map = game->map;
		old_pos = game->player->pos;
		generate_interior_map(game);
		engine_center_camera(game);
	}
	if(player_curr_tile == TILE_DOOR_OUT) {
		game->map = old_map;
		game->player->pos = old_pos;
		game->player->direction = DIR_DOWN;
		game->player->anim.dir = DIR_DOWN;
		engine_center_camera(game);
	}
	if(player_curr_tile == TILE_TELEPORTER) {
		struct Teleporter teleporter = get_teleporter_xy(game->map, game->player->pos);
		game->player->pos.x = teleporter.x2;
		game->player->pos.y = teleporter.y2;
		if(teleporter.idMap != -1) {
			set_map(game, teleporter.idMap);
		}
		engine_center_camera(game);
	}
	if(player_curr_tile == TILE_GRASS) {
		if(rand_range(0, PROBA_WILD) == 0) create_battle(game);
	}
	if(player_curr_tile == TILE_ICE) {
		int direction = game->player->direction;
		int dx = (direction == DIR_RIGHT) - (direction == DIR_LEFT);
		int dy = (direction == DIR_DOWN) - (direction == DIR_UP);

		if(player_facing(game) == TILE_ICE || map_walkable(game->map, game->player->pos.x+dx, game->player->pos.y+dy)) {
			game->player->pos.x += dx;
			game->player->pos.y += dy;
			engine_draw(game);
			engine_tick(game, ENGINE_TICK);
			dupdate();
			engine_check_position(game);
		}
	}
}

void engine_center_camera(struct Game *game) {
	game->camera.pos.x = game->player->pos.x * TILE_SIZE + game->player->x_mid;
	game->camera.pos.y = game->player->pos.y * TILE_SIZE + game->player->y_mid;
}
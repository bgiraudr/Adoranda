#include <string.h>
#include <stdlib.h>
#include <gint/rtc.h>

#include "monster.h"
#include "stats.h"
#include "capacite.h"
#include "player.h"
#include "map.h"
#include "util.h"

#include <gint/display.h>
#include <gint/keyboard.h>

extern struct Monsters monsters;

struct Monster *get_monster_id(int id) {
	for(int i = 0; i < monsters.nbMonsters; i++) {
		if(monsters.monster[i]->id == id) return monsters.monster[i];
	}
	return monsters.monster[0];
}

struct Monster *generate_monster(struct Game *game) {

	srand(rtc_ticks());

	int level_zone;

	//set the monster depends of the current zone
	struct Monster *monster;
	if(is_in_zone(game->player, game->map)) {
		struct Zone zone = get_zone(game->player, game->map);
		if(zone.nbMonsters != 0) {
			monster = copyMonster(get_monster_id(zone.monsters[rand_range(0,zone.nbMonsters)]));
		} else {
			monster = copyMonster(get_monster_id(0));
		}
		level_zone = zone.level != -1 ? zone.level : game->player->stats.level;
	} else {
		monster = copyMonster(get_monster_id(0));
		level_zone = game->player->stats.level;
	}

	int signe = rand_range(0,2);
	signe = signe ? -1 : 1;

	int variation = signe*rand_range(0,6);
	int level = level_zone + variation;
	if(level > 100) level = 100;
	if(level < 1) level = 1;

	monster->stats->level = level;
	set_stats_level(monster->stats);

	monster->stats->pv = monster->stats->max_pv;
	return monster;
}

struct Move *monster_select(struct Player *player, struct Monster *monster) {
	struct Move *stoMove = get_move_id_pointer(monster->moves[0]);
	int stoDamage = 0;

	for(int i = 0; i < monster->nbMoves; i++) {
		struct Move *move = get_move_id_pointer(monster->moves[i]);
		int damage = calc_damage(monster->stats, &player->stats, move);
		// if(damage >= player->stats.pv) {
		// 	stoMove = move;
		// 	if(move->precision > 80) return move;
		// }
		if(damage > stoDamage) {
			stoMove = move;
			stoDamage = damage;
		}
	}
	return stoMove;
}

struct Monster *copyMonster(struct Monster *source) {
	struct Monster *dest = malloc(sizeof(struct Monster));

	dest->name = source->name;
	dest->nbMoves = source->nbMoves;
	dest->sprite = source->sprite;
	dest->sprite_x = source->sprite_x;
	dest->sprite_y = source->sprite_y;
	
	dest->id = source->id;
	
	dest->moves = malloc(dest->nbMoves * sizeof *source->moves);
	memcpy(dest->moves, source->moves, dest->nbMoves * sizeof *source->moves);

	dest->stats = malloc(sizeof *source->stats);
	memcpy(dest->stats, source->stats, sizeof *source->stats);

	return dest;
}

void free_monster(struct Monster *monster) {
	free(monster->moves);
	free(monster->stats);
	free(monster);
}
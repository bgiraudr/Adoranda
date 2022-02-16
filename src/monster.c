#include <string.h>
#include <stdlib.h>

#include "monster.h"
#include "stats.h"
#include "capacite.h"
#include "player.h"
#include "map.h"

struct Monster *generate_monster(struct Game *game) {

	extern struct Monster monster_test;

	struct Monster *monster = copyMonster(&monster_test);

	int level_zone = get_level_zone(game->player, game->map);
	//En cas d'erreur / aucune zone trouvée
	if(level_zone == -1) {
		level_zone = game->player->stats.level;
	}

	monster->stats->level = level_zone;
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
		if(damage >= player->stats.pv) {
			return move;
		}
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
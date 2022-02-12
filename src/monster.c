#include "monster.h"
#include "stats.h"
#include "capacite.h"
#include "player.h"
#include <string.h>
#include <stdlib.h>

struct Monster *generate_monster(struct Game *game) {

	extern struct Monster monster_test;

	struct Monster *monster = copyMonster(&monster_test);
	// TODO formule pour niveau du monstre adverse
	monster->stats->level = game->player->stats.level;
	return monster;
}

struct Move monster_select(struct Player *player, struct Monster *monster) {
	struct Move stoMove = get_move_id(monster->moves[0]);
	for(int i = 0; i < monster->nbMoves; i++) {
		struct Move move = get_move_id(monster->moves[i]);
		if(move.atk >= player->stats.pv) {
			return move;
		}
		if(move.atk > stoMove.atk) {
			stoMove = move;
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
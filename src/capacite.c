#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/image.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <gint/rtc.h>

#include "capacite.h"
#include "util.h"
#include "type.h"
#include "draw_util.h"

extern struct Capacities capacities;

struct Move default_move() {
	return *capacities.moves[0];
}

struct Move get_move_id(int id) {
	for(int i = 0; i < capacities.nbCapacities; i++) {
		if(capacities.moves[i]->id == id) return *capacities.moves[i];
	}
	return *capacities.moves[0];
}

struct Move *get_move_id_pointer(int id) {
	for(int i = 0; i < capacities.nbCapacities; i++) {
		if(capacities.moves[i]->id == id) return capacities.moves[i];
	}
	return capacities.moves[id];
}

struct Move *copy_move(struct Move move) {
	struct Move *copyMove = malloc(sizeof(struct Move));
	copyMove->name = move.name;
	copyMove->type = move.type;
	copyMove->init_pp = move.init_pp;
	copyMove->id = move.id;
	copyMove->categorie = move.categorie;
	
	copyMove->pp = move.pp;
	copyMove->atk = move.atk;
	copyMove->precision = move.precision;
	copyMove->boost_atk = move.boost_atk;
	copyMove->boost_def = move.boost_def;
    copyMove->boost_spe_atk = move.boost_spe_atk;
    copyMove->boost_spe_def = move.boost_spe_def;
	copyMove->boost_hp = move.boost_hp;

	return copyMove;
}

void draw_move(int x, int y, int x2, int y2, struct Move *move, int selected) {
	extern bopti_image_t img_capacite;
	extern bopti_image_t img_categories;
	extern bopti_image_t img_contours;

	const int font_size = 8;
	draw_change_one_color(x, y, &img_capacite, 0xE6D6, getTypeFromName(move->type).color);
	if(selected) dimage(x, y, &img_contours);
	dsubimage(x+96, y+7, &img_categories, 20*move->categorie, 0, 20, 10, DIMAGE_NONE);

	int color = move->pp > 0 ? C_BLACK : C_RED;
	dprint(x+15, y+5, color, "PP : %d", move->pp);
	dprint((int)((x+x2)/2)-(int)(strlen(move->name)/2*font_size), 
		(int)((y+y2)/2)-font_size/2, 
		C_BLACK, "%s", move->name);

	if(move->categorie == STATUT) {
		if(move->boost_atk > 0) dprint(x+10, y2-17, C_BLACK, "A+%d%%", move->boost_atk);
		if(move->boost_hp > 0) dprint(x+47, y2-17, C_BLACK, "H+%d%%", move->boost_hp);
		if(move->boost_def > 0) dprint(x+85, y2-17, C_BLACK, "D+%d%%", move->boost_def);
	} else {
		dprint(x+15, y2-17, C_BLACK, "ATK : %d", move->atk);
		dprint(x+70, y2-17, C_BLACK, "PRE : %d", move->precision);
	}
}

void draw_special_move(int x, int y, struct Move *move, int selected) {
	draw_move(x, y, x+125, y+60, move, selected);
}

void draw_classic_move(int x, int y, struct Move *move) {
	draw_move(x, y, x+125, y+60, move, 0);
}

int execute_move(struct Stats *player_stats, struct Stats *monster_stats, struct Move *move, int ismonster) {
	srand(rtc_ticks());
	if(move->categorie == PHYSICAL || move->categorie == SPECIAL) {
		if(is_miss(move)) {
			move->pp--;
			return MISS;
		}

		float typeEffect = ismonster ?
                           getTypeEffect(getTypeFromName(move->type), getTypeFromName(player_stats->type)) :
                           getTypeEffect(getTypeFromName(move->type), getTypeFromName(monster_stats->type));

		if(ismonster) {
			player_stats->pv-=calc_damage(monster_stats, player_stats, move);
		} else {
			move->pp--;
			monster_stats->pv-=calc_damage(player_stats, monster_stats, move);
		}

		if(typeEffect == 2)	return SUPER_EFFECTIVE;
		if(typeEffect == 0.5) return LESS_EFFECTIVE;
		if(typeEffect == 0)	return NOT_EFFECTIVE;
	} else {
        return EFFECT;
		/*if(ismonster) {
			return self_effect(monster_stats, move);
		} else {
			move->pp--;
			return self_effect(player_stats, move);	
		}
		return HEAL;*/
	}
	return SUCCESS;
}

int calc_damage(struct Stats *attacker, struct Stats *target, struct Move *move) {
	int atk =1, def = 1;
	if(move->categorie == PHYSICAL) {
		atk = attacker->atk;
		def = target->def;
	} else if(move->categorie == SPECIAL) {
		atk = attacker->spe_atk;
		def = target->spe_def;
	}

	if(getTypeEffect(getTypeFromName(move->type), getTypeFromName(target->type)) == 0) return 0;
	return floor((floor(((2*attacker->level / 5 + 2) * atk * 
		move->atk
			*getTypeEffect(getTypeFromName(move->type), getTypeFromName(target->type))
			*stab(attacker->type, move->type) 
			/ def) 
		/ 50) + 2)*crit(attacker));
}

float stab(char *type, char *move) {
	if(!strcmp(type, move)) return 1.5f;
	return 1.0f;
}

int is_crit() {
	//une chance sur 16 d'avoir un coup critique
	const int proba_crit = PROBA_CRIT;
	return rand_range(0,proba_crit)==0;
}

float crit(struct Stats *attacker) {
	float taux = 1.0f;
	if(is_crit()) {
		taux = (float)(2 * attacker->level + 5)/(attacker->level+5);
	}
	return taux;
}

/*1 if miss, else 0*/
int is_miss(struct Move *move) {
	return rand_range(0, 101) > move->precision;
}

void self_effect(struct Stats *stats, struct Move *move) {
	stats->pv += stats->max_pv * move->boost_hp/100;
	stats->atk *= (float)(100+move->boost_atk)/100;
	stats->def *= (float)(100+move->boost_def)/100;
    stats->spe_atk *= (float)(100+move->boost_spe_atk)/100;
    stats->spe_def *= (float)(100+move->boost_spe_def)/100;

	if(stats->pv > stats->max_pv) stats->pv = stats->max_pv;
}
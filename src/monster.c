#include "monster.h"
#include "stats.h"
#include "capacite.h"

struct Monster generate_monster(struct Game *game) {

	extern bopti_image_t img_test;

	struct Stats s = {
		.atk=10,
		.def=10,
		.pv=20,
		.level=1,
		.max_pv=20,
	};

	struct Monster m = {
		.name="pas content",
		.stats = s,
		.sprite = &img_test,
	};

	m.moves[0] = get_move_id(0);
	m.moves[1] = get_move_id(1);
	m.moves[2] = get_move_id(2);

	return m;
}
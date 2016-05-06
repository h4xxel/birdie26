#include "healthbar.h"
#include "main.h"


static void _calculate_one(MOVABLE_ENTRY *entry, DARNIT_TILE *tile, int x_pos, int id) {
	float arne;
	int w;

	arne = ((float) entry->hp) / entry->hp_max;
	w = arne * 128;
	d_render_tile_set(tile, 0, id * 2);
	d_render_tile_set(tile, 1, id * 2 + 1);
	d_render_tile_move(tile, 0, x_pos, 0);
	d_render_tile_move(tile, 1, x_pos + w, 0);
	d_render_tile_size_set(tile, 0, w, 16);
	d_render_tile_size_set(tile, 1, 128 - w, 16);
	d_render_tile_tilesheet_coord_set(tile, 0, 0, id * 16, w, 16);
	d_render_tile_tilesheet_coord_set(tile, 1, 128 + w, 16 * id, w, 16);
}


void healthbar_calculate() {
	/* This will be retardedly slow */
	int i, id;

	for (i = 0; i < s->movable.movables; i++) {
		id = _get_player_id(&s->movable.movable[i]);
		if (id < 0)
			continue;
		_calculate_one(&s->movable.movable[i], s->healthbar.bar[id], 160 * id, id);
		s->healthbar.hp[id] = s->movable.movable[i].hp;
	}
}


void healthbar_init() {
	s->healthbar.ts = d_render_tilesheet_load("res/healthbars.png", 128, 16, DARNIT_PFORMAT_RGB5A1);
	s->healthbar.bar[0] = d_render_tile_new(2, s->healthbar.ts);
	s->healthbar.bar[1] = d_render_tile_new(2, s->healthbar.ts);
	s->healthbar.bar[2] = d_render_tile_new(2, s->healthbar.ts);
	s->healthbar.bar[3] = d_render_tile_new(2, s->healthbar.ts);
}


void healthbar_draw() {
	int i;

	healthbar_calculate();
	d_render_offset(0, 0);

	for (i = 0; i < 4; i++)
		if (s->healthbar.hp[i])
			d_render_tile_draw(s->healthbar.bar[i], 2);
}

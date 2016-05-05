#include "ailib.h"
#include "movable.h"
#include "main.h"
#include <string.h>



void ai_player(void *dummy, void *entry, MOVABLE_MSG msg) {
	MOVABLE_ENTRY *self = entry;
	
	switch (msg) {
		case MOVABLE_MSG_INIT:
			self->hp = self->hp_max = 400;
			self->gravity_effect = 1;
			s->camera.follow = self->id;
			break;
		case MOVABLE_MSG_LOOP:
			if (d_keys_get().left)
				self->x_velocity = -400;
			else if (d_keys_get().right)
				self->x_velocity = 400;
			else
				self->x_velocity = 0;
			if (d_keys_get().up) {
				DARNIT_KEYS keys;

				keys =	d_keys_zero();
				keys.up = 1;
				d_keys_set(keys);
				
				if (!self->y_velocity)
					self->y_velocity = -600;
			}
			break;
		default:
			break;

	}
}

static struct AILibEntry ailib[] = {
	{ "player", ai_player },
	{ NULL, NULL }
};

void *ailib_get(const char *str) {
	int i;

	for (i = 0; ailib[i].symbol; i++) {
		if (!strcmp(ailib[i].symbol, str))
			return (void *) ailib[i].func;
	}

	return NULL;
}

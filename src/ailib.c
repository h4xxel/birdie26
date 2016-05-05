#include "ailib.h"
#include "movable.h"
#include "ingame.h"
#include "main.h"
#include <string.h>



void ai_player(void *dummy, void *entry, MOVABLE_MSG msg) {
	MOVABLE_ENTRY *self = entry;
	int player_id;
	char *playerid_str;
	
	/* On a scale of 1 to italy, how inefficient is this? */
	if (!(playerid_str = d_map_prop(s->active_level->object[self->id].ref, "player_id")))
		return self->hp = 0, (void) 0;
	player_id = atoi(playerid_str);
	
	switch (msg) {
		case MOVABLE_MSG_INIT:
			self->hp = self->hp_max = 400;
			self->gravity_effect = 1;
			if (player_id >= PLAYER_CAP)	// TODO: replace PLAYER_CAP with actual number of connected players //
				self->hp = 0;
			break;
		case MOVABLE_MSG_LOOP:
			if (ingame_keystate[player_id].left)
				self->x_velocity = -400;
			else if (ingame_keystate[player_id].right)
				self->x_velocity = 400;
			else
				self->x_velocity = 0;
			if (ingame_keystate[player_id].jump) {
				DARNIT_KEYS keys;
				
				ingame_keystate[player_id].jump = 0;
				
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

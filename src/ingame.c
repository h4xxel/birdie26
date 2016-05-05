#include "main.h"
#include "ingame.h"
#include "camera.h"
#include "util.h"
#include <stdbool.h>

struct InGameKeyStateEntry ingame_keystate[PLAYER_CAP];

void ingame_init() {
	int i;
	const char *playerid_str;
	/* Leak *all* the memory */
	s->active_level = d_map_load(util_binrel_path("res/arne.ldmz"));
	s->camera.follow = -1;
	s->camera.x = s->camera.y = 0;

	movableInit();
	bulletInit();
	movableLoad();

	for (i = 0; i < s->movable.movables; i++) {
		if (!(playerid_str = d_map_prop(s->active_level->object[i].ref, "player_id")))
			continue;
		if (atoi(playerid_str) == 1) {	// TODO: Replace with clients player ID */
			s->camera.follow = i;
			break;
		}
	}
}


void ingame_loop() {
	int i;

	d_render_tint(255, 255, 255, 255);
	movableLoop();
	camera_work();
	d_map_camera_move(s->active_level, s->camera.x, s->camera.y);
	for (i = 0; i < s->active_level->layers; i++) {
		d_tilemap_draw(s->active_level->layer[i].tilemap);
		d_render_offset(s->camera.x, s->camera.y);
		movableLoopRender(i);
	}
}


void ingame_client_keyboard() {
	struct InGameKeyStateEntry oldstate;
	struct InGameKeyStateEntry newstate;
	
	newstate.left = d_keys_get().left;
	newstate.right = d_keys_get().right;
	newstate.jump = d_keys_get().up;
	
	if (newstate.left ^ oldstate.left) {
		// change happened
	}

	if (newstate.right ^ oldstate.right) {
		// change happened
	}
	
	if (newstate.jump ^ oldstate.jump) {
		// change happened
	}

	/* ... */
	/* TODO: Need to send this to the serb */

	oldstate = newstate;
}

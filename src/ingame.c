#include "main.h"
#include "ingame.h"
#include "camera.h"
#include "util.h"
#include "network/protocol.h"
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
		if (atoi(playerid_str) == s->player_id + 1) {
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
	ingame_client_keyboard();
}


void ingame_client_keyboard() {
	static struct InGameKeyStateEntry oldstate;
	struct InGameKeyStateEntry newstate, pressevent, releaseevent;
	
	newstate.left = d_keys_get().left;
	newstate.right = d_keys_get().right;
	newstate.jump = d_keys_get().up;

	
	
	if (newstate.left ^ oldstate.left) {
		if (newstate.left)
			pressevent.left = true, releaseevent.left = false;
		else
			releaseevent.left = true, pressevent.left = false;
	}

	if (newstate.right ^ oldstate.right) {
		if (newstate.right)
			pressevent.right = true, releaseevent.right = false;
		else
			releaseevent.right = true, pressevent.right = false;
	}
	
	if (newstate.jump ^ oldstate.jump) {
		if (newstate.jump)
			pressevent.jump = true, releaseevent.jump = false;
		else
			releaseevent.jump = true, pressevent.jump = false;
	}

	PacketKeypress kp;

	kp.size = sizeof(kp);
	kp.type = PACKET_TYPE_KEYPRESS;
	kp.keypress = pressevent;
	kp.keyrelease = releaseevent;

	protocol_send_packet(server_sock, (void *) &kp);

	oldstate = newstate;
}

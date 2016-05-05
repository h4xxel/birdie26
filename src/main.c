#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <signal.h>
#include <stdbool.h>
#include <darnit/darnit.h>
#include "ui/ui.h"
#include "menu.h"
#include "gameroom.h"
#include "lobby.h"
#include "server/server.h"
#include "network/network.h"
#include "network/protocol.h"
#include "main.h"
#include "camera.h"
#include "ingame.h"

Gfx gfx;
GameState gamestate;
struct GameStateStruct *s;
char player_name[NAME_LEN_MAX];
int server_sock;

void (*state_render[GAME_STATES])()={
	[GAME_STATE_MENU] = menu_render,
	[GAME_STATE_GAME] = ingame_loop,
	[GAME_STATE_SELECT_NAME] = NULL,
//	[GAME_STATE_GAME_OVER] = game_over_render,
};

void (*state_network_handler[GAME_STATES])()={
	[GAME_STATE_GAMEROOM] = gameroom_network_handler,
	[GAME_STATE_LOBBY] = lobby_network_handler,
};

struct UI_PANE_LIST *gamestate_pane[GAME_STATES];

void restart_to_menu(const char *name) {
	// This is ugly :D
	char buf[4096];
	sprintf(buf, "%s", d_fs_exec_path());
	sprintf(buf, "%s", basename(buf));
	if(name)
		execl(d_fs_exec_path(), buf, name, NULL);
	else
		execl(d_fs_exec_path(), buf, NULL);
}

int join_game(unsigned long sip) {
	PacketJoin join;
	
	if((server_sock = network_connect_tcp(sip, PORT + 1)) < 0) {
		fprintf(stderr, "failed to join %lu\n", sip);
		return -1;
	}

	join.type = PACKET_TYPE_JOIN;
	join.size = sizeof(PacketJoin);
	join.id = 0;
	memcpy(join.name, player_name, NAME_LEN_MAX);
	join.name[NAME_LEN_MAX - 1] = 0;

	protocol_send_packet(server_sock, (void *) &join);
	return 0;
}

void game_state(GameState state) {
	//Game state destructors
	switch(gamestate) {
		case GAME_STATE_GAME:
			//pthread_cancel(game.thread);
			break;
		case GAME_STATE_MENU:
			//ui_event_global_remove(menu_buttons, UI_EVENT_TYPE_BUTTONS);
		case GAME_STATE_SELECT_NAME:
		case GAME_STATE_HOST:
		case GAME_STATE_LOBBY:
		case GAME_STATE_ENTER_IP:
			break;
		case GAME_STATE_GAMEROOM:
			break;
		case GAME_STATE_GAME_OVER:
		case GAME_STATE_QUIT:
		
		case GAME_STATES:
			break;
	}
	//Game state constructors
	switch(state) {
		case GAME_STATE_GAME:
			ingame_init();
			//init game shit
			//pthread_create(&game.thread, NULL, object_thread, NULL);
			#ifndef __DEBUG__
			//d_input_grab();
			#endif
			break;
		case GAME_STATE_MENU:
			//ui_event_global_add(menu_buttons, UI_EVENT_TYPE_BUTTONS);
		case GAME_STATE_SELECT_NAME:
			ui_selected_widget = select_name.entry;
			break;
		case GAME_STATE_LOBBY:
			//gameroom.button.start->enabled = false;
			//we_are_hosting_a_game = false;
			//ui_listbox_clear(lobby.list);
			break;
		case GAME_STATE_ENTER_IP:
			ui_selected_widget = enter_ip.entry;
			break;
		case GAME_STATE_HOST:
			//we_are_hosting_a_game = true;
			server_start();
			gameroom.button.start->enabled = true;
			
			join_game(network_local_ip());
			
			state = GAME_STATE_GAMEROOM;
		case GAME_STATE_GAMEROOM:
			ui_listbox_clear(gameroom.list);
		case GAME_STATE_GAME_OVER:
		case GAME_STATE_QUIT:
			d_input_release();
		
		case GAME_STATES:
			break;
	}
	
	gamestate=state;
}

int main(int argc, char  **argv) {
	char font_path[512];
	
	d_init_custom("birdie26", DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, "birdie26", NULL);
	
	sprintf(font_path, "%s", d_fs_exec_path());
	sprintf(font_path, "%s/res/font.ttf", dirname(font_path));
	gfx.font.large = d_font_load(font_path, 40, 256, 256);
	gfx.font.small = d_font_load(font_path, 16, 256, 256);
	s = malloc(sizeof(*s));

	ui_init(4);
	menu_init();
	gameroom_init();
	lobby_init();
	
	gamestate_pane[GAME_STATE_MENU] = &menu.pane;
	gamestate_pane[GAME_STATE_SELECT_NAME] = &select_name.pane;
	gamestate_pane[GAME_STATE_LOBBY] = &lobby.pane;
	gamestate_pane[GAME_STATE_ENTER_IP] = &enter_ip.pane;
	gamestate_pane[GAME_STATE_GAMEROOM] = &gameroom.pane;
	//gamestate_pane[GAME_STATE_GAME_OVER] = &game_over.pane;
	
	
	signal(SIGINT, d_quit); //lol
	network_init(PORT);
	
	d_cursor_show(1);
	
	if(argc > 1) {
		snprintf(player_name, NAME_LEN_MAX, "%s", argv[1]);
		game_state(GAME_STATE_MENU);
	} else
		game_state(GAME_STATE_SELECT_NAME);
	while(gamestate!=GAME_STATE_QUIT) {
		if(state_network_handler[gamestate])
			state_network_handler[gamestate]();
		
		d_render_begin();
		d_render_blend_enable();
		if(state_render[gamestate])
			state_render[gamestate]();
		
		d_render_tint(0, 255, 0, 255);
		if(gamestate_pane[gamestate])
			ui_events(gamestate_pane[gamestate], 1);

		d_render_end();
		d_loop();
	}

	d_quit();
	return 0;
}

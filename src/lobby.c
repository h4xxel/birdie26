#include <string.h>
#include <darnit/darnit.h>
#include "ui/ui.h"
#include "network/protocol.h"
#include "network/network.h"
#include "main.h"
#include "lobby.h"

Lobby lobby;

static void button_callback(UI_WIDGET *widget, unsigned int type, UI_EVENT *e) {
	UI_PROPERTY_VALUE v;
	
	if(widget == lobby.button.back) {
		restart_to_menu(player_name);
	} else if(widget == lobby.button.join) {
		v = lobby.list->get_prop(lobby.list, UI_LISTBOX_PROP_SELECTED);
		if(v.i < 0)
			return;
		
		unsigned long sip = strtoul(ui_listbox_get(lobby.list, v.i), NULL, 10);
		join_game(sip);
		game_state(GAME_STATE_GAMEROOM);
	}
}

void lobby_init() {
	lobby.pane.pane = ui_pane_create(10, 10, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 20, lobby.vbox = ui_widget_create_vbox());
	lobby.pane.next = NULL;

	ui_vbox_add_child(lobby.vbox, lobby.label = ui_widget_create_label(gfx.font.large, "Join game"), 0);
	ui_vbox_add_child(lobby.vbox, lobby.list = ui_widget_create_listbox(gfx.font.small), 1);
	
	lobby.hbox = ui_widget_create_hbox();
	ui_vbox_add_child(lobby.hbox, lobby.button.back = ui_widget_create_button_text(gfx.font.small, "Back"), 0);
	ui_vbox_add_child(lobby.hbox, lobby.button.join = ui_widget_create_button_text(gfx.font.small, "Join"), 0);
	ui_vbox_add_child(lobby.vbox, lobby.hbox, 0);
	
	lobby.button.back->event_handler->add(lobby.button.back, button_callback, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
	lobby.button.join->event_handler->add(lobby.button.join, button_callback, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
}

void lobby_network_handler() {
	UI_PROPERTY_VALUE v;
	Packet pack;
	char name[256], *s;
	unsigned long ip;
	int i;
	
	if(!network_poll_udp())
		return;
	ip = network_recv_udp(&pack, sizeof(PacketLobby));
	if(pack.type != PACKET_TYPE_LOBBY)
		return;
	
	v = lobby.list->get_prop(lobby.list, UI_LISTBOX_PROP_SIZE);
	for(i = 0; i < v.i; i++) {
		s = ui_listbox_get(lobby.list, i);
		if(strtoul(s, NULL, 10) == ip) {
			if(strstr(s, "Unknown")) {
				sprintf(name, "%lu: %s's game", ip, pack.lobby.name);
				ui_listbox_set(lobby.list, i, name);
				return;
			} else
				return;
		}
	}
	sprintf(name, "%lu: %s's game", ip, pack.lobby.name);
	ui_listbox_add(lobby.list, name);
}

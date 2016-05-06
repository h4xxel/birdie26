#include <stdbool.h>
#include <darnit/darnit.h>
#include "ui/ui.h"
#include "characters.h"
#include "main.h"

CharacterRoom character_room;

static void button_callback(UI_WIDGET *widget, unsigned int type, UI_EVENT *e) {
	if(widget == character_room.button.back) {
		game_state(GAME_STATE_MENU);
	}
}

void character_room_init() {
	character_room.pane.pane = ui_pane_create(10, 10, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 20, character_room.vbox = ui_widget_create_vbox());
	character_room.pane.next = NULL;

	character_room.pane.pane->background_color.r = 0x7f;
	character_room.pane.pane->background_color.g = 0x7f;
	character_room.pane.pane->background_color.b = 0x7f;

	ui_vbox_add_child(character_room.vbox, character_room.label = ui_widget_create_label(gfx.font.large, "Character Showcase"), 0);
	
	UI_WIDGET *tmp;
	
	character_room.hbox1 = ui_widget_create_hbox();
	
	tmp = ui_widget_create_vbox();
	
	ui_vbox_add_child(tmp, ui_widget_create_label(gfx.font.large, "Rump"), 0);
	ui_vbox_add_child(tmp, ui_widget_create_label(gfx.font.small, "\"Make America hate again!\""), 0);
	
	ui_hbox_add_child(character_room.hbox1, tmp, 1);
	tmp = ui_widget_create_vbox();
	
	ui_vbox_add_child(tmp, ui_widget_create_label(gfx.font.large, "Dim Young 'Un"), 0);
	ui_vbox_add_child(tmp, ui_widget_create_label(gfx.font.small, "\"Everyone be like: Dear Leader, your onion breath is so fresh!\""), 0);
	
	ui_hbox_add_child(character_room.hbox1, tmp, 1);
	
	character_room.hbox2 = ui_widget_create_hbox();
	
	tmp = ui_widget_create_vbox();
	
	ui_vbox_add_child(tmp, ui_widget_create_label(gfx.font.large, "Shitler"), 0);
	ui_vbox_add_child(tmp, ui_widget_create_label(gfx.font.small, "\"NEIN NEIN NEIN NEIN\""), 0);
	
	ui_hbox_add_child(character_room.hbox2, tmp, 1);
	tmp = ui_widget_create_vbox();
	
	ui_vbox_add_child(tmp, ui_widget_create_label(gfx.font.large, "Ståhlin"), 0);
	ui_vbox_add_child(tmp, ui_widget_create_label(gfx.font.small, "\"My gulag brings all the boys to the yard\""), 0);
	
	ui_hbox_add_child(character_room.hbox2, tmp, 1);
	
	character_room.hbox_buttons = ui_widget_create_hbox();
	ui_vbox_add_child(character_room.hbox_buttons, character_room.button.back = ui_widget_create_button_text(gfx.font.small, "Back"), 0);
	
	ui_vbox_add_child(character_room.vbox, character_room.hbox1, 1);
	ui_vbox_add_child(character_room.vbox, character_room.hbox2, 1);
	ui_vbox_add_child(character_room.vbox, character_room.hbox_buttons, 0);
	
	character_room.button.back->event_handler->add(character_room.button.back, button_callback, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
	
	character_room.sprite[0] = d_sprite_load("res/player.spr", 2, DARNIT_PFORMAT_RGBA8);
	character_room.sprite[1] = d_sprite_load("res/player2.spr", 2, DARNIT_PFORMAT_RGBA8);
	character_room.sprite[2] = d_sprite_load("res/player3.spr", 2, DARNIT_PFORMAT_RGBA8);
	character_room.sprite[3] = d_sprite_load("res/player4.spr", 2, DARNIT_PFORMAT_RGBA8);
	d_sprite_move(character_room.sprite[0], 160, 160);
	d_sprite_move(character_room.sprite[1], DISPLAY_WIDTH - 180, 160);
	d_sprite_move(character_room.sprite[2], 160, 360);
	d_sprite_move(character_room.sprite[3], DISPLAY_WIDTH - 180, 360);
	
	int i;
	for(i = 0; i < 4; i++) {
		d_sprite_activate(character_room.sprite[i], 2);
		d_sprite_animate_start(character_room.sprite[i]);
		
	}
}

void character_room_render() {
	int i;
	
	d_render_blend_enable();
	for(i = 0; i < 4; i++)
		d_sprite_draw(character_room.sprite[i]);
}

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

	ui_vbox_add_child(character_room.vbox, character_room.label = ui_widget_create_label(gfx.font.large, "Character Showcase"), 0);
	
	character_room.hbox1 = ui_widget_create_hbox();
	ui_vbox_add_child(character_room.hbox1, ui_widget_create_label(gfx.font.small, "Rump"), 1);
	ui_vbox_add_child(character_room.hbox1, ui_widget_create_label(gfx.font.small, "Dim Young 'Un"), 1);
	
	character_room.hbox2 = ui_widget_create_hbox();
	ui_vbox_add_child(character_room.hbox2, ui_widget_create_label(gfx.font.small, "Shitler"), 1);
	ui_vbox_add_child(character_room.hbox2, ui_widget_create_label(gfx.font.small, "Ståhlin"), 1);
	
	character_room.hbox_buttons = ui_widget_create_hbox();
	ui_vbox_add_child(character_room.hbox_buttons, character_room.button.back = ui_widget_create_button_text(gfx.font.small, "Back"), 0);
	
	ui_vbox_add_child(character_room.vbox, character_room.hbox1, 1);
	ui_vbox_add_child(character_room.vbox, character_room.hbox2, 1);
	ui_vbox_add_child(character_room.vbox, character_room.hbox_buttons, 0);
	
	character_room.button.back->event_handler->add(character_room.button.back, button_callback, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
}

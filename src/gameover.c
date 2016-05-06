#include <stdbool.h>
#include <darnit/darnit.h>
#include "ui/ui.h"
#include "gameover.h"
#include "main.h"

GameOver game_over;

static const char *character_names[] = {
	"Rump",
	"Dim Young 'Un",
	"Shitler",
	"Ståhlin",
};

static void button_callback(UI_WIDGET *widget, unsigned int type, UI_EVENT *e) {
	if(widget == game_over.button.menu) {
		restart_to_menu(player_name);
	}
}

void game_over_init() {
	game_over.pane.pane = ui_pane_create(10, 10, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 20, game_over.vbox = ui_widget_create_vbox());
	game_over.pane.next = NULL;

	game_over.pane.pane->background_color.r = PANE_R;
	game_over.pane.pane->background_color.g = PANE_G;
	game_over.pane.pane->background_color.b = PANE_B;

	ui_vbox_add_child(game_over.vbox, game_over.label = ui_widget_create_label(gfx.font.large, "Game Over"), 0);
	ui_vbox_add_child(game_over.vbox, game_over.whowon = ui_widget_create_label(gfx.font.large, "Name won!"), 1);
	
	ui_vbox_add_child(game_over.vbox, game_over.button.menu = ui_widget_create_button_text(gfx.font.small, "Main menu"), 0);
	
	game_over.button.menu->event_handler->add(game_over.button.menu, button_callback, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
}

void game_over_set_player(int player_id, const char *name) {
	UI_PROPERTY_VALUE v;
	
	v.p = malloc(512);
	sprintf(v.p, "%s (%s) has won!", name, player_id < 0 ? " - " : character_names[player_id]);
	game_over.whowon->set_prop(game_over.whowon, UI_LABEL_PROP_TEXT, v);
}

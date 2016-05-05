#include <darnit/darnit.h>
#include "ui/ui.h"
#include "main.h"
#include "menu.h"

Menu menu;
SelectName select_name;
MenuHelp menu_help;

static void button_callback_name(UI_WIDGET *widget, unsigned int type, UI_EVENT *e) {
	UI_PROPERTY_VALUE v;
	if(widget == select_name.button.quit) {
		d_quit();
	}
	v = select_name.entry->get_prop(select_name.entry, UI_ENTRY_PROP_TEXT);
	snprintf(player_name, NAME_LEN_MAX, "%s", (char *) v.p);
	game_state(GAME_STATE_MENU);
}

static void button_callback_menu(UI_WIDGET *widget, unsigned int type, UI_EVENT *e) {
	/*UI_PROPERTY_VALUE v;
	v.p = "arne";
	label->set_prop(label, UI_LABEL_PROP_TEXT, v);*/
	if(widget == menu.button.host) {
		game_state(GAME_STATE_HOST);
	} else if(widget == menu.button.join) {
		game_state(GAME_STATE_LOBBY);
	} else if(widget == menu.button.quit)
		d_quit();
}

void menu_init() {
	UI_PROPERTY_VALUE v;
	menu.pane.pane = ui_pane_create(10, 10, DISPLAY_WIDTH - 20, 200, menu.vbox = ui_widget_create_vbox());
	menu.pane.next = &menu_help.pane;
	
	menu_help.pane.pane = ui_pane_create(10, 200 + 30, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 200 - 20 - 20, menu_help.vbox = ui_widget_create_vbox());
	menu_help.pane.next = NULL;
	
	ui_vbox_add_child(menu_help.vbox, menu_help.label.title = ui_widget_create_label(gfx.font.large, "How to play"), 0);
	ui_vbox_add_child(menu_help.vbox, ui_widget_create_spacer_size(0, 10), 0);
	ui_vbox_add_child(menu_help.vbox, menu_help.label.title = ui_widget_create_label(gfx.font.small, 
		"arne"
	), 0);
	ui_vbox_add_child(menu_help.vbox, ui_widget_create_spacer(), 1);
	ui_vbox_add_child(menu_help.vbox, menu_help.label.title = ui_widget_create_label(gfx.font.small, "2016 party hack for Birdie Gamedev compo\nh4xxel (http://h4xxel.org) & slaesjag (http://rdw.se) of //actung fulkod"), 0);
	
	ui_vbox_add_child(menu.vbox, menu.label.title = ui_widget_create_label(gfx.font.large, "birdie26"), 1);
	ui_vbox_add_child(menu.vbox, menu.label.author = ui_widget_create_label(gfx.font.small, "//actung fulkod"), 1);
	//ui_vbox_add_child(menu.vbox, ui_widget_create_entry(font), 0);
	ui_vbox_add_child(menu.vbox, menu.button.host = ui_widget_create_button_text(gfx.font.small, "Host game"), 0);
	ui_vbox_add_child(menu.vbox, menu.button.join = ui_widget_create_button_text(gfx.font.small, "Join game"), 0);
	ui_vbox_add_child(menu.vbox, menu.button.quit = ui_widget_create_button_text(gfx.font.small, "Quit game"), 0);

	menu.button.host->event_handler->add(menu.button.host, button_callback_menu, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
	menu.button.join->event_handler->add(menu.button.join, button_callback_menu, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
	menu.button.quit->event_handler->add(menu.button.quit, button_callback_menu, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
        menu.button.host->event_handler->add(menu.button.host, button_callback_menu, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
        
        
	select_name.pane.pane = ui_pane_create(DISPLAY_WIDTH/2 - 200, DISPLAY_HEIGHT/2 - 150/2, 400, 150, select_name.vbox = ui_widget_create_vbox());
	select_name.pane.next = NULL;

	ui_vbox_add_child(select_name.vbox, select_name.label = ui_widget_create_label(gfx.font.large, "Enter a name"), 1);
	ui_vbox_add_child(select_name.vbox, select_name.entry = ui_widget_create_entry(gfx.font.small), 0);
	select_name.hbox = ui_widget_create_hbox();
	ui_vbox_add_child(select_name.hbox, select_name.button.ok = ui_widget_create_button_text(gfx.font.small, "OK"), 0);
	ui_vbox_add_child(select_name.hbox, select_name.button.quit = ui_widget_create_button_text(gfx.font.small, "Quit"), 0);
	ui_vbox_add_child(select_name.vbox, select_name.hbox, 0);
	
	#ifdef _WIN32
	v.p = getenv("USERNAME");
	#else
	v.p = getenv("LOGNAME");
	#endif
	select_name.entry->set_prop(select_name.entry, UI_ENTRY_PROP_TEXT, v);
	
	select_name.button.ok->event_handler->add(select_name.button.ok, button_callback_name, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
	select_name.button.quit->event_handler->add(select_name.button.quit, button_callback_name, UI_EVENT_TYPE_UI_WIDGET_ACTIVATE);
}

void menu_render() {
	
}

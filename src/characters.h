#ifndef __CHARACTERS_H__
#define __CHARACTERS_H__
#include "ui/ui.h"

typedef struct CharacterRoom CharacterRoom;
struct CharacterRoom {
	struct UI_PANE_LIST pane;
	UI_WIDGET *label;
	UI_WIDGET *list;
	UI_WIDGET *vbox;
	UI_WIDGET *hbox1;
	UI_WIDGET *hbox2;
	UI_WIDGET *hbox_buttons;
	struct {
		UI_WIDGET *start;
		UI_WIDGET *back;
	} button;
};

extern CharacterRoom character_room;
void character_room_network_handler();
void character_room_init();

#endif

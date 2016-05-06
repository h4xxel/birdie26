#ifndef __GAMEOVER_H__
#define __GAMEOVER_H__
#include "ui/ui.h"

typedef struct GameOver GameOver;
struct GameOver {
	struct UI_PANE_LIST pane;
	UI_WIDGET *vbox;
	UI_WIDGET *label;
	UI_WIDGET *whowon;
	struct {
		UI_WIDGET *menu;
	} button;
};

extern GameOver game_over;
void game_over_init();

#endif

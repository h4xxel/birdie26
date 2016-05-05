#ifndef _INGAME_H__
#define	_INGAME_H__

#include "main.h"
#include <stdbool.h>

void ingame_init();
void ingame_loop();


struct InGameKeyStateEntry {
	bool			left;
	bool			right;
	bool			jump;
};


extern struct InGameKeyStateEntry ingame_keystate[PLAYER_CAP];

void ingame_client_keyboard();


#endif

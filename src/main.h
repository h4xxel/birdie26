#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdbool.h>
#include <darnit/darnit.h>

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
//1338 för oss som är lite bättre
#define PORT 1338

#define NAME_LEN_MAX 64
#define	PLAYER_CAP 4

#define PANE_R 0xDD
#define PANE_G 0xDD
#define PANE_B 0xCD

#include "movable.h"
#include "bullet.h"
#include "player.h"
#include "healthbar.h"
#include "soundeffects.h"

typedef enum GameState GameState;
enum GameState {
	GAME_STATE_MENU,
	GAME_STATE_SELECT_NAME,
	GAME_STATE_CHARACTERS,
	GAME_STATE_LOBBY,
	GAME_STATE_ENTER_IP,
	GAME_STATE_HOST,
	GAME_STATE_GAMEROOM,
	GAME_STATE_GAME,
	GAME_STATE_GAME_OVER,
	GAME_STATE_QUIT,
	GAME_STATES,
};

typedef struct Gfx Gfx;
struct Gfx {
	struct {
		DARNIT_FONT *small;
		DARNIT_FONT *large;
	} font;
};

extern Gfx gfx;
extern char player_name[];
extern int server_sock;


struct GameStateStruct {
	int	player_id;
	bool is_host;
	MOVABLE			movable;
	DARNIT_MAP		*active_level;
	BULLET			bullet;
	struct HealthbarStruct	healthbar;
	struct PlayerState	player[PLAYER_CAP];
	struct SoundeffectStruct sfx;
	struct {
				int x;
				int y;
				int follow;
	} camera;
};

extern struct GameStateStruct *s;

void game_state(GameState state);
void restart_to_menu(const char *name);
int join_game(unsigned long sip);

#endif

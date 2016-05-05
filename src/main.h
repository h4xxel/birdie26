#ifndef __MAIN_H__
#define __MAIN_H__

#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
//1338 för oss som är lite bättre
#define PORT 1338

#define NAME_LEN_MAX 64

typedef enum GameState GameState;
enum GameState {
	GAME_STATE_MENU,
	GAME_STATE_SELECT_NAME,
	GAME_STATE_LOBBY,
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

void game_state(GameState state);

#endif

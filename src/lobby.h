#ifndef __LOBBY_H__
#define __LOBBY_H__

typedef struct Lobby Lobby;
struct Lobby {
	struct UI_PANE_LIST pane;
	UI_WIDGET *label;
	UI_WIDGET *list;
	UI_WIDGET *vbox;
	UI_WIDGET *hbox;
	struct {
		UI_WIDGET *join;
		UI_WIDGET *enter_ip;
		UI_WIDGET *back;
	} button;
};

typedef struct EnterIP EnterIP;
struct EnterIP {
	struct UI_PANE_LIST pane;
	UI_WIDGET *label;
	UI_WIDGET *entry;
	UI_WIDGET *vbox;
	UI_WIDGET *hbox;
	struct {
		UI_WIDGET *join;
		UI_WIDGET *back;
	} button;
};

extern Lobby lobby;
extern EnterIP enter_ip;
void lobby_init();
void lobby_network_handler();

#endif

#ifndef __SERVER_H__
#define __SERVER_H__

void server_start();
void server_start_game();
void server_kick();
bool server_player_is_present(int id);
void server_shutdown();

#endif

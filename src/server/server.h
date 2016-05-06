#ifndef __SERVER_H__
#define __SERVER_H__

#include "../soundeffects.h"

void server_start();
void server_start_game();
void server_kick();
void server_announce_winner(int winning_player);
void server_sound(enum SoundeffectSound sound);
bool server_player_is_present(int id);
void server_shutdown();

#endif

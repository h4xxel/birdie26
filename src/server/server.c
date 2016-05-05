#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <darnit/darnit.h>
#include "../main.h"
#include "../network/network.h"
#include "../network/protocol.h"

bool server_in_game;

int server_lobby(void *arg) {
	PacketLobby pack;
	
	pack.type = PACKET_TYPE_LOBBY;
	pack.size = sizeof(PacketLobby);
	
	strcpy(pack.name, player_name);
	
	for(;;) {
		network_broadcast_udp(&pack, pack.size);
		sleep(2);
	}
	
	return 0;
}

void server_start() {
	server_in_game = false;
	d_util_thread_new(server_lobby, NULL);
}


void server_start_game() {
	server_in_game = true;
}

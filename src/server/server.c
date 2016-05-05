#include <string.h>
#include <unistd.h>
#include <darnit/darnit.h>
#include "../main.h"
#include "../network/network.h"
#include "../network/protocol.h"

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
	d_util_thread_new(server_lobby, NULL);
}

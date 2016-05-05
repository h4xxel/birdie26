#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <darnit/darnit.h>
#include "../main.h"
#include "../network/network.h"
#include "../network/protocol.h"

int usleep(useconds_t usec);

typedef struct ClientList ClientList;
struct ClientList {
	int id;
	int sock;
	char name[NAME_LEN_MAX];
	
	ClientList *next;
};

static volatile bool server_in_game;
static int listen_sock;
static ClientList *client = NULL;
static int clients = 0;

void server_handle_client(ClientList *cli) {
	Packet pack;
	Packet response;
	ClientList *tmp;
	
	if(!network_poll_tcp(cli->sock))
		return;
	
	protocol_recv_packet(cli->sock, &pack);
	
	switch(pack.type) {
		case PACKET_TYPE_JOIN:
			strcpy(cli->name, pack.join.name);
			
			response.type = PACKET_TYPE_JOIN;
			response.size = sizeof(PacketJoin);
			
			for(tmp = client; tmp; tmp = tmp->next) {
				strcpy(response.join.name, tmp->name);
				response.join.id = tmp->id;
				protocol_send_packet(cli->sock, &response);
				
				if(tmp->sock != cli->sock) {
					response.join.id = cli->id;
					strcpy(response.join.name, cli->name);
					protocol_send_packet(tmp->sock, &response);
				}
			}
			
			break;
	}
}

int server_lobby(void *arg) {
	PacketLobby pack;
	ClientList *tmp;
	
	pack.type = PACKET_TYPE_LOBBY;
	pack.size = sizeof(PacketLobby);
	
	strcpy(pack.name, player_name);
	
	for(;;) {
		if(network_poll_tcp(listen_sock)) {
			int sock;
			
			sock = network_accept_tcp(listen_sock);
			printf("accept %i\n", sock);
			
			tmp = malloc(sizeof(ClientList));
			tmp->sock = sock;
			tmp->id = clients++;
			tmp->next = client;
			client = tmp;
		}
		
		for(tmp = client; tmp; tmp = tmp->next)
			server_handle_client(tmp);
		
		network_broadcast_udp(&pack, pack.size);
		usleep(100000);
	}
	
	return 0;
}

void server_start() {
	server_in_game = false;
	if((listen_sock = network_listen_tcp(PORT + 1)) < 0) {
		fprintf(stderr, "Server failed to open listening socket\n");
		exit(1);
	}
	d_util_thread_new(server_lobby, NULL);
}


void server_start_game() {
	server_in_game = true;
}

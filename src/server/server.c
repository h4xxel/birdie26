#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <darnit/darnit.h>
#include "../main.h"
#include "../network/network.h"
#include "../network/protocol.h"

#define HANDLE_KEY(A) do { \
		if(pack.keypress.keypress.A ) \
			ingame_keystate[cli->id].A = 1; \
		if(pack.keypress.keyrelease.A ) \
			ingame_keystate[cli->id].A = 0; \
	} while(0)

int usleep(useconds_t usec);

typedef enum ServerState ServerState;
enum ServerState {
	SERVER_STATE_LOBBY,
	SERVER_STATE_STARTING,
	SERVER_STATE_GAME,
};

typedef struct ClientList ClientList;
struct ClientList {
	int id;
	int sock;
	char name[NAME_LEN_MAX];
	
	ClientList *next;
};

static int listen_sock;
static ClientList *client = NULL;
static int clients = 0;
static volatile ServerState server_state;

DARNIT_SEMAPHORE *sem;

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
				memset(response.join.name, 0, NAME_LEN_MAX);
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
		
		case PACKET_TYPE_KEYPRESS:
			HANDLE_KEY(left);
			HANDLE_KEY(right);
			HANDLE_KEY(jump);
			HANDLE_KEY(grab);
			break;
	}
}


void handle_client_thread() {
	for (;;) {
		struct ClientList *tmp;
		for(tmp = client; tmp; tmp = tmp->next)
			server_handle_client(tmp);
		usleep(1000);
	}
}


int server_thread(void *arg) {
	PacketLobby pack;
	Packet move;
	ClientList *tmp;
	void *p;
	int i;
	
	for(;;) {
		switch(server_state) {
			case SERVER_STATE_LOBBY:
				if(network_poll_tcp(listen_sock)) {
					int sock;
					
					sock = network_accept_tcp(listen_sock);
					
					tmp = malloc(sizeof(ClientList));
					tmp->sock = sock;
					tmp->id = clients++;
					tmp->next = client;
					client = tmp;
				}
				
				for(tmp = client; tmp; tmp = tmp->next)
					server_handle_client(tmp);
				
				
				pack.type = PACKET_TYPE_LOBBY;
				pack.size = sizeof(PacketLobby);
				memset(pack.name, 0, NAME_LEN_MAX);
				strcpy(pack.name, player_name);
				
				network_broadcast_udp(&pack, pack.size);
				usleep(100000);
				break;
				
			case SERVER_STATE_STARTING:
				for(tmp = client; tmp; tmp = tmp->next) {
					PacketStart start;
					
					start.type = PACKET_TYPE_START;
					start.size = sizeof(PacketStart);
					
					start.player_id = tmp->id;
					
					protocol_send_packet(tmp->sock, (void *) &start);
				}
				
				server_state = SERVER_STATE_GAME;
				d_util_thread_new(handle_client_thread, NULL);
				break;
				
			case SERVER_STATE_GAME:
				d_util_semaphore_wait(sem);
				
				move.type = PACKET_TYPE_MOVE_OBJECT;
				move.size = 4 + s->movable.movables*10;
				p = move.raw;
				
				for(i = 0; i < s->movable.movables; i++) {
					*((uint16_t *) p) = (s->movable.movable[i].x)/1000;
					p+= 2;
					*((uint16_t *) p) = (s->movable.movable[i].y)/1000;
					p+= 2;
					*((uint8_t *) p) = s->movable.movable[i].direction;
					p+= 1;
					*((uint8_t *) p) = (s->movable.movable[i].angle / 10 / 2);
					p += 1;
					*((uint16_t *) p) = (s->movable.movable[i].hp);
					p += 2;
					*((uint16_t *) p) = (s->movable.movable[i].hp_max);
					p += 2;
				}
				
				for(tmp = client; tmp; tmp = tmp->next)
					if(tmp->id != s->player_id)
						protocol_send_packet(tmp->sock, &move);
				
				for(tmp = client; tmp; tmp = tmp->next)
					server_handle_client(tmp);
				
				break;
		}
	}
	
	return 0;
}

void server_start() {
	sem = d_util_semaphore_new(0);
	server_state = SERVER_STATE_LOBBY;
	if((listen_sock = network_listen_tcp(PORT + 1)) < 0) {
		fprintf(stderr, "Server failed to open listening socket\n");
		exit(1);
	}
	d_util_thread_new(server_thread, NULL);
}

void server_sound(enum SoundeffectSound sound) {
	PacketSound pack;
	ClientList *tmp;

	pack.type = PACKET_TYPE_SOUND;
	pack.size = sizeof(pack);
	pack.sound = sound;
	for (tmp = client; tmp; tmp = tmp->next)
		protocol_send_packet(tmp->sock, &pack);
	return;
}

void server_announce_winner(int winning_player) {
	PacketExit pack;
	ClientList *tmp;
	
	pack.type = PACKET_TYPE_EXIT;
	pack.size = sizeof(PacketExit);
	
	pack.player = winning_player;
	
	if(winning_player < 0 || winning_player >= PLAYER_CAP);
	else {
		for(tmp = client; tmp; tmp = tmp->next) {
			if(tmp->id == winning_player) {
				strcpy(pack.name, tmp->name);
				goto send_pack;
			}
		}
	}
	strcpy(pack.name, "No-one");
	
	send_pack:
	for(tmp = client; tmp; tmp = tmp->next)
		protocol_send_packet(tmp->sock, (void *) &pack);
}

void server_start_game() {
	server_state = SERVER_STATE_STARTING;
}

void server_kick() {
	d_util_semaphore_add(sem, 1);
}

bool server_player_is_present(int id) {
	struct ClientList *next;

	for (next = client; next; next = next->next)
		if (next->id == id)
			return true;
	return false;
}

void server_shutdown() {
	ClientList *tmp;
	
	network_close_udp();
	
	for(tmp = client; tmp; tmp = tmp->next)
		network_close_tcp(tmp->sock);
	
	network_close_tcp(listen_sock);
	
}


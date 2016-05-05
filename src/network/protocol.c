#include "network.h"
#include "protocol.h"

int protocol_send_packet(int sock, Packet *pack) {
	return network_send_tcp(sock, (void *) pack, pack->size);
}

int protocol_recv_packet(int sock, Packet *pack) {
	if(network_poll_tcp(sock) == 0)
		return -1;
	
	network_recv_tcp(sock, (void *) pack, 4);
	network_recv_tcp(sock, ((void *) pack) + 4, pack->size - 4);
	
	return 0;
}

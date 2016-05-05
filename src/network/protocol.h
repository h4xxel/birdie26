#ifndef __COMMON_PROTOCOL_H__
#define __COMMON_PROTOCOL_H__

#include <stdint.h>
#include "../main.h"


typedef enum PacketType PacketType;
enum PacketType {
	PACKET_TYPE_LOBBY,
	PACKET_TYPE_SETUP,
	PACKET_TYPE_EXIT,
};

typedef struct PacketLobby PacketLobby;
struct PacketLobby {
	uint16_t type;
	uint16_t size;
	
	char name[NAME_LEN_MAX];
};

typedef struct PacketSetup PacketSetup;
struct PacketSetup {
	uint16_t type;
	uint16_t size;
	
	uint32_t id;
	uint32_t objects;
//	double width;
	double map_width;
	uint32_t pre_simulations;
	uint32_t home;
};

typedef struct PacketExit PacketExit;
struct PacketExit {
	uint16_t type;
	uint16_t size;
};

typedef union Packet Packet;
union Packet {
	struct {
		uint16_t type;
		uint16_t size;
	};
	
	PacketLobby lobby;
	PacketSetup setup;
	PacketExit exit;
	
	uint8_t raw[512];
};

int protocol_send_packet(int sock, Packet *pack);
int protocol_recv_packet(int sock, Packet *pack);

#endif

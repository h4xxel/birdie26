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
	uint32_t type;
	
	uint32_t begin;
	char name[NAME_LEN_MAX];
};

typedef struct PacketSetup PacketSetup;
struct PacketSetup {
	uint32_t type;
	
	uint32_t id;
	uint32_t objects;
//	double width;
	double map_width;
	uint32_t pre_simulations;
	uint32_t home;
};

typedef struct PacketExit PacketExit;
struct PacketExit {
	uint32_t type;
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

#endif

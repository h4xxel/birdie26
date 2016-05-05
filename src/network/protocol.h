#ifndef __COMMON_PROTOCOL_H__
#define __COMMON_PROTOCOL_H__

#include <stdint.h>
#include "../main.h"


typedef enum PacketType PacketType;
enum PacketType {
	PACKET_TYPE_LOBBY,
	PACKET_TYPE_JOIN,
	PACKET_TYPE_START,
	PACKET_TYPE_EXIT,
};

typedef struct PacketLobby PacketLobby;
struct PacketLobby {
	uint16_t type;
	uint16_t size;
	
	char name[NAME_LEN_MAX];
};

typedef struct PacketJoin PacketJoin;
struct PacketJoin {
	uint16_t type;
	uint16_t size;
	
	uint32_t id;
	char name[NAME_LEN_MAX];
};

typedef struct PacketStart PacketStart;
struct PacketStart {
	uint16_t type;
	uint16_t size;
	
	uint32_t player_id;
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
	PacketJoin join;
	PacketStart start;
	PacketExit exit;
	
	uint8_t raw[512];
};

int protocol_send_packet(int sock, Packet *pack);
int protocol_recv_packet(int sock, Packet *pack);

#endif

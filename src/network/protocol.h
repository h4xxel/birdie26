#ifndef __COMMON_PROTOCOL_H__
#define __COMMON_PROTOCOL_H__

#include <stdint.h>
#include "../main.h"
#include "../ingame.h"


typedef enum PacketType PacketType;
enum PacketType {
	PACKET_TYPE_LOBBY,
	PACKET_TYPE_JOIN,
	PACKET_TYPE_START,
	PACKET_TYPE_MOVE_OBJECT,
	PACKET_TYPE_KEYPRESS,
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

typedef struct PacketKeypress PacketKeypress;
struct PacketKeypress {
	uint16_t type;
	uint16_t size;

	struct InGameKeyStateEntry keypress, keyrelease;
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
		uint8_t raw[512];
	};
	
	PacketLobby lobby;
	PacketJoin join;
	PacketStart start;
	PacketKeypress keypress;
	PacketExit exit;
};

int protocol_send_packet(int sock, Packet *pack);
int protocol_recv_packet(int sock, Packet *pack);

#endif

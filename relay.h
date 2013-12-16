/*
 * relay.h
 *
 * DHCP relay
 */

#ifndef RELAY_H_
#define RELAY_H_

#include "Include/GenericTypeDefs.h"

#include "Include/TCPIP_Stack/TCPIP.h"
#include "Include/TCPIP_Stack/DHCP.h"

// DHCP server IP: 192.168.0.42
#define DHCP_SERVER_IP1 (192ul)
#define DHCP_SERVER_IP2 (168ul)
#define DHCP_SERVER_IP3 (0ul)
#define DHCP_SERVER_IP4 (42ul)

// DHCP server MAC (broadcast by default)
#define DHCP_SERVER_MAC1 0xFF
#define DHCP_SERVER_MAC2 0xFF
#define DHCP_SERVER_MAC3 0xFF
#define DHCP_SERVER_MAC4 0xFF
#define DHCP_SERVER_MAC5 0xFF
#define DHCP_SERVER_MAC6 0xFF

// Size of one packet fragment
// Arrays larger than 256 bytes are not supported,
// so fragmentation is required
#define RELAY_FRAGMENT_SIZE 128

// Packet fragment
typedef struct {
	BYTE size;
	BYTE *contents;
} RELAY_FRAGMENT;

// Size of one packet
#define RELAY_PACKET_SIZE (4 * RELAY_FRAGMENT_SIZE)

// Packet containing 4 fragments
typedef struct {
	RELAY_FRAGMENT fragment1;
	RELAY_FRAGMENT fragment2;
	RELAY_FRAGMENT fragment3;
	RELAY_FRAGMENT fragment4;
} RELAY_PACKET;

void RelayInit(void);
void RelayTask(void);

void RelayClientRequest(void);
void RelayServerReply(void);

BOOL RelayResolveServer(void);
void RelayCreateSockets(void);

BOOL RelayPacketGet(UDP_SOCKET socket, RELAY_PACKET *packet);
BOOL RelayPacketPut(UDP_SOCKET socket, RELAY_PACKET *packet);

BOOL MACAddrEquals(const MAC_ADDR *mac1, const MAC_ADDR *mac2);
void MACAddrCopy(MAC_ADDR *dest, const MAC_ADDR *src);

#endif /* RELAY_H_ */

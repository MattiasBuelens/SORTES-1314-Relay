/*
 * relay.h
 *
 * DHCP relay state machine
 */

#ifndef RELAY_H_
#define RELAY_H_

#include "Include/TCPIP_Stack/TCPIP.h"
#include "Include/TCPIP_Stack/DHCP.h"

// DHCP server IP: 10.0.0.1
#define DHCP_SERVER_IP1 (10ul)
#define DHCP_SERVER_IP2 (0ul)
#define DHCP_SERVER_IP3 (0ul)
#define DHCP_SERVER_IP4 (1ul)

// DHCP server MAC (broadcast by default)
#define DHCP_SERVER_MAC1 0xFF
#define DHCP_SERVER_MAC2 0xFF
#define DHCP_SERVER_MAC3 0xFF
#define DHCP_SERVER_MAC4 0xFF
#define DHCP_SERVER_MAC5 0xFF
#define DHCP_SERVER_MAC6 0xFF

#define DHCP_PACKET_MAX_SIZE 576
#define BOOTP_HEADER_SIZE 44
#define DHCP_PACKET_MAX_DATA 532 // (DHCP_PACKET_MAX_SIZE - BOOTP_HEADER_SIZE)void RelayInit(void);
void RelayTask(void);

void RelayClientRequest(void);
void RelayServerReply(void);

BOOL RelayResolveServer(void);

BOOL MACAddrEquals(const MAC_ADDR *mac1, const MAC_ADDR *mac2);
BOOL MACAddrCopy(MAC_ADDR *dest, const MAC_ADDR *src);

#endif /* RELAY_H_ */

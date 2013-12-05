/*
 * relay.c
 *
 * DHCP relay state machine
 */

// Some macros
#define LOW(a)     (a & 0xFF)
#define HIGH(a)    ((a>>8) & 0xFF)

// Define processor and compiler
#define __18F97J60
#define __SDCC__

// Include all hardware and compiler dependent definitions
#include "Include/HardwareProfile.h"

// Include platform functions
#include "platform/display.h"
#include "platform/led.h"

// Include all headers for any enabled TCPIP Stack functions
#include "Include/TCPIP_Stack/TCPIP.h"
#include "Include/TCPIP_Stack/Delay.h"
#include "Include/TCPIP_Stack/UDP.h"

#include "relay.h"

NODE_INFO serverInfo;

UDP_SOCKET serverSocket;
UDP_SOCKET clientSocket;

void RelayInit(void) {
	// Initialize server node info
	serverInfo.IPAddr.Val = DHCP_SERVER_IP1 |
	DHCP_SERVER_IP2 << 8ul | DHCP_SERVER_IP3 << 16ul |
	DHCP_SERVER_IP4 << 24ul;
	// Start off with MAC broadcast address
	serverInfo.MACAddr.v[0] = DHCP_SERVER_MAC1;
	serverInfo.MACAddr.v[1] = DHCP_SERVER_MAC2;
	serverInfo.MACAddr.v[2] = DHCP_SERVER_MAC3;
	serverInfo.MACAddr.v[3] = DHCP_SERVER_MAC4;
	serverInfo.MACAddr.v[4] = DHCP_SERVER_MAC5;
	serverInfo.MACAddr.v[5] = DHCP_SERVER_MAC6;

	// Initialize sockets
	// Socket to DHCP server
	serverSocket = UDPOpen(DHCP_CLIENT_PORT, &serverInfo, DHCP_SERVER_PORT);
	// Socket to clients
	clientSocket = UDPOpen(DHCP_SERVER_PORT, NULL, DHCP_CLIENT_PORT);

	// Resolve server
	RelayResolveServer();

	// Write something on the screen
	display_string(0, 0, "DHCP Relay");
}

void RelayTask(void) {
	RelayClientRequest();
	RelayServerReply();
}

void RelayClientRequest(void) {
	BYTE request[DHCP_PACKET_MAX_SIZE];
	BOOTP_HEADER * pheader;
	WORD nBytes;

	nBytes = UDPIsGetReady(clientSocket);
	if (nBytes == 0)
		return;

	// Read client request
	UDPGetArray(request, nBytes);
	UDPDiscard();

	// Set relay agent IP
	pheader = (BOOTP_HEADER *) request;
	pheader->RelayAgentIP.Val = AppConfig.MyIPAddr.Val;

	// Resolve the DHCP server if needed
	// If not yet resolved, the socket will just do a MAC broadcast
	RelayResolveServer();

	// Relay to server
	if (UDPIsPutReady(serverSocket)) {
		// Put request
		UDPPutArray(request, nBytes);
		// Add zero padding to ensure compatibility with old BOOTP relays that
		// discard small packets (<300 UDP octets)
		while (UDPTxCount < 300u)
			UDPPut(0);
		// Send
		UDPFlush();
	}
}

void RelayServerReply(void) {
	BYTE reply[DHCP_PACKET_MAX_SIZE];
	WORD nBytes;

	nBytes = UDPIsGetReady(serverSocket);
	if (nBytes == 0)
		return;

	// Read server reply
	UDPGetArray(reply, nBytes);
	UDPDiscard();

	// Broadcast to client
	if (UDPIsPutReady(clientSocket)) {
		// Put reply
		UDPPutArray(reply, nBytes);
		// Add zero padding to ensure compatibility with old BOOTP relays that
		// discard small packets (<300 UDP octets)
		while (UDPTxCount < 300u)
			UDPPut(0);
		// Send
		UDPFlush();
	}
}

BOOL RelayResolveServer(void) {
	MAC_ADDR serverMAC;

	// Resolve server IP again (for later)
	ARPResolve(&(serverInfo.IPAddr));

	// Check if server IP was already resolved
	if (!ARPIsResolved(&(serverInfo.IPAddr), &serverMAC)) {
		// Not yet resolved
		return FALSE;
	}

	// Check if MAC has changed
	if (!MACAddrEquals(&serverMAC, &(serverInfo.MACAddr))) {
		// Store new MAC
		MACAddrCopy(&(serverInfo.MACAddr), &serverMAC);
		// Re-create socket with new MAC
		UDPClose(serverSocket);
		serverSocket = UDPOpen(DHCP_CLIENT_PORT, &serverInfo, DHCP_SERVER_PORT);
	}
	return TRUE;
}

BOOL MACAddrEquals(const MAC_ADDR *mac1, const MAC_ADDR *mac2) {
	int i;
	for (i = 0; i < 6; i++) {
		if (mac1->v[i] != mac2->v[i])
			return FALSE;
	}
	return TRUE;
}

BOOL MACAddrCopy(MAC_ADDR *dest, const MAC_ADDR *src) {
	int i = 0;
	for (i = 0; i < 6; i++) {
		dest->v[i] = src->v[i];
	}
	return TRUE;
}

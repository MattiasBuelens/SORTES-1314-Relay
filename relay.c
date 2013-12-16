/*
 * relay.c
 *
 * DHCP relay state machine
 */

// Some macros
#define LOW(a)     (a & 0xFF)
#define HIGH(a)    ((a>>8) & 0xFF)
#define MAX(a,b)   (((a)>(b))?(a):(b))
#define MIN(a,b)   (((a)<(b))?(a):(b))

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

// Fragment buffers
BYTE serverFragment1[RELAY_FRAGMENT_SIZE];
BYTE serverFragment2[RELAY_FRAGMENT_SIZE];
BYTE serverFragment3[RELAY_FRAGMENT_SIZE];
BYTE serverFragment4[RELAY_FRAGMENT_SIZE];
BYTE clientFragment1[RELAY_FRAGMENT_SIZE];
BYTE clientFragment2[RELAY_FRAGMENT_SIZE];
BYTE clientFragment3[RELAY_FRAGMENT_SIZE];
BYTE clientFragment4[RELAY_FRAGMENT_SIZE];

// Packet buffers
RELAY_PACKET serverPacket = { { 0, serverFragment1 }, { 0, serverFragment2 }, {
		0, serverFragment3 }, { 0, serverFragment4 } };
RELAY_PACKET clientPacket = { { 0, clientFragment1 }, { 0, clientFragment2 }, {
		0, clientFragment3 }, { 0, clientFragment4 } };

// Connections
NODE_INFO serverInfo;
UDP_SOCKET serverSocket = -1, clientSocket = -1;

void RelayInit(void) {
	// Write something on the screen
	display_string(0, 0, "DHCP Relay");
	DelayMs(100);

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
	RelayCreateSockets();

	// Resolve server
	RelayResolveServer();
}

void RelayCreateSockets(void) {
	// Unicast socket to DHCP server
	if (serverSocket > 0)
		UDPClose(serverSocket);
	serverSocket = UDPOpen(DHCP_SERVER_PORT, &serverInfo, DHCP_SERVER_PORT);
	// Broadcast socket to clients
	if (clientSocket > 0)
		UDPClose(clientSocket);
	clientSocket = UDPOpen(DHCP_SERVER_PORT, NULL, DHCP_CLIENT_PORT);
}

void RelayTask(void) {
	RelayClientRequest();
	RelayServerReply();
}

void RelayClientRequest(void) {
	BOOTP_HEADER *pheader;

	// Read client request
	if (!RelayPacketGet(clientSocket, &clientPacket)) {
		// Nothing to read or too large
		return;
	}

	pheader = (BOOTP_HEADER *) (clientPacket.fragment1.contents);
	// Increment hop count
	pheader->Hops++;
	// Set relay agent IP
	pheader->RelayAgentIP.Val = AppConfig.MyIPAddr.Val;

	// Resolve the DHCP server if needed
	// If not yet resolved, the socket will just do a MAC broadcast
	RelayResolveServer();

	// Sockets need to be recreated before put
	// since UDP changes remote node from broadcast to unicast
	RelayCreateSockets();
	// Relay client packet to server
	RelayPacketPut(serverSocket, &clientPacket);

	// Log on display
	display_string(1, 0, "Client > Server");
}

void RelayServerReply(void) {
	BOOTP_HEADER *pheader;

	// Read server reply
	if (!RelayPacketGet(serverSocket, &serverPacket)) {
		// Nothing to read or too large
		return;
	}

	pheader = (BOOTP_HEADER *) (serverPacket.fragment1.contents);
	// Increment hop count
	pheader->Hops++;

	// Sockets need to be recreated before put
	// since UDP changes remote node from broadcast to unicast
	RelayCreateSockets();
	// Broadcast to client
	RelayPacketPut(clientSocket, &serverPacket);

	// Log on display
	display_string(1, 0, "Server > Client");
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
	}
	return TRUE;
}

void RelayFragmentGet(RELAY_FRAGMENT *fragment, WORD *remainingSize) {
	// Read at most RELAY_FRAGMENT_SIZE bytes into fragment
	BYTE fragmentSize = MIN(*remainingSize, RELAY_FRAGMENT_SIZE);
	fragment->size = fragmentSize;
	// Check if anything to read
	if (fragmentSize == 0) {
		return;
	}
	// Read fragment
	UDPGetArray(fragment->contents, fragmentSize);
	// Update remaining packet size
	*remainingSize -= fragmentSize;
}

BOOL RelayPacketGet(UDP_SOCKET socket, RELAY_PACKET *packet) {
	WORD packetSize;

	// Get packet size to read
	packetSize = UDPIsGetReady(socket);
	if (packetSize == 0) {
		// Nothing to read
		return FALSE;
	} else if (packetSize > RELAY_PACKET_SIZE) {
		// Packet too large
		return FALSE;
	}

	// Read into fragments
	RelayFragmentGet(&(packet->fragment1), &packetSize);
	RelayFragmentGet(&(packet->fragment2), &packetSize);
	RelayFragmentGet(&(packet->fragment3), &packetSize);
	RelayFragmentGet(&(packet->fragment4), &packetSize);

	return TRUE;
}

BOOL RelayPacketPut(UDP_SOCKET socket, RELAY_PACKET *packet) {
	if (!UDPIsPutReady(socket)) {
		// Put not ready
		return FALSE;
	}

	// Read from fragments
	UDPPutArray(packet->fragment1.contents, packet->fragment1.size);
	UDPPutArray(packet->fragment2.contents, packet->fragment2.size);
	UDPPutArray(packet->fragment3.contents, packet->fragment3.size);
	UDPPutArray(packet->fragment4.contents, packet->fragment4.size);

	// Add zero padding to ensure compatibility with old BOOTP relays that
	// discard small packets (<300 UDP octets)
	while (UDPTxCount < 300u)
		UDPPut(0);

	// Send
	UDPFlush();

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

void MACAddrCopy(MAC_ADDR *dest, const MAC_ADDR *src) {
	int i = 0;
	for (i = 0; i < 6; i++) {
		dest->v[i] = src->v[i];
	}
}

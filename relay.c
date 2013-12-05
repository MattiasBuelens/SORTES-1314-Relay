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
#include "Include/TCPIP_Stack/DHCP.h"

void RelayInit(void) {
	// Write something on the screen
	display_string(0, 0, "DHCP Relay");
}

void RelayTask(void) {

}

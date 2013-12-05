/*
 * main.c
 *
 * Main program
 */

/*
 * This symbol uniquely defines this file as the main entry point.
 * There should only be one such definition in the entire project,
 * and this file must define the AppConfig variable as described below.
 * The processor configuration will be included in HardwareProfile.h
 * if this symbol is defined.
 */
#define THIS_INCLUDES_THE_MAIN_FUNCTION
#define THIS_IS_STACK_APPLICATION

// Define processor and compiler
#define __18F97J60
#define __SDCC__

// Include all hardware and compiler dependent definitions
#include "Include/HardwareProfile.h"

// Include all headers for any enabled TCPIP Stack functions
#include "Include/TCPIP_Stack/TCPIP.h"
#include "Include/TCPIP_Stack/Delay.h"
#include "Include/TCPIP_Stack/DHCP.h"

// Include own functions
#include "platform/display.h"
#include "platform/led.h"
#include "relay.h"

// Declare AppConfig structure
APP_CONFIG AppConfig;

/**
 * Low-priority interrupt routine.
 */
#if defined(__18CXX)
    #if defined(HI_TECH_C)
        void interrupt low_priority LowISR(void)
    #elif defined(__SDCC__)
        void LowISR(void) __interrupt (2) //ML for sdcc
    #else
        #pragma interruptlow LowISR
        void LowISR(void)
    #endif
    {
        TickUpdate();
    }
    #if !defined(__SDCC__) && !defined(HI_TECH_C)
           //automatic with these compilers
        #pragma code lowVector=0x18
	void LowVector(void){_asm goto LowISR _endasm}
	#pragma code // Return to default code section
    #endif

	/**
	 * High-priority interrupt routine.
	 */
	#if defined(HI_TECH_C)
		void interrupt HighISR(void)
	#elif defined(__SDCC__)
		void HighISR(void) __interrupt(1) //ML for sdcc
	#else
		#pragma interruptlow HighISR
		void HighISR(void)
	#endif
	{
	  // TODO Insert here code for high level interrupt, if any
	}
	#if !defined(__SDCC__) && !defined(HI_TECH_C)
		   //automatic with these compilers
	#pragma code highVector=0x8
	void HighVector(void){_asm goto HighISR _endasm}
	#pragma code // Return to default code section
	#endif
#endif // defined(__18CXX)

/**
 * Main routine
 */
#if defined(__18CXX) || defined(__SDCC__)
void main(void)
#else
int main(void)
#endif
{
	// Initialize board
	InitializeBoard();

	// LCD
	display_init();

    // Initialize Timer0, and low priority interrupts, used as clock.
    TickInit();

    // Initialize Stack and application related variables in AppConfig.
    InitAppConfig();

    // Initialize core stack layers (MAC, ARP, TCP, UDP) and
    // application modules (HTTP, SNMP, etc.)
    StackInit();

    // Initialize relay
    RelayInit();

    // Cooperative multitasking loop
	while (TRUE) {
		// This task performs normal stack task including checking
		// for incoming packet, type of packet and calling
		// appropriate stack entity to process it.
		StackTask();

		// Relay tasks
		RelayTask();
	}
}

/****************************************************************************
  Function:
    static void InitializeBoard(void)

  Description:
    This routine initializes the hardware.  It is a generic initialization
    routine for many of the Microchip development boards, using definitions
    in HardwareProfile.h to determine specific initialization.

  Precondition:
    None

  Parameters:
    None - None

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
static void InitializeBoard(void) {
	// LEDs
	led_init();

	// Set clock to 25 MHz

	// Enable PLL but disable pre and postscalers: the primary oscillator
	// runs at the speed of the 25MHz external quartz
	OSCTUNE = 0x40;

	// Switch to primary oscillator mode,
	// regardless of if the config fuses tell us to start operating using
	// the the internal RC
	// The external clock must be running and must be 25MHz for the
	// Ethernet module and thus this Ethernet bootloader to operate.
	if (OSCCONbits.IDLEN) //IDLEN = 0x80; 0x02 selects the primary clock
		OSCCON = 0x82;
	else
		OSCCON = 0x02;

	// Enable Interrupts
	RCONbits.IPEN = 1;		// Enable interrupt priorities
	INTCONbits.GIEH = 1;
	INTCONbits.GIEL = 1;
}

/*********************************************************************
 * Function:        void InitAppConfig(void)
 *
 * PreCondition:    MPFSInit() is already called.
 *
 * Input:           None
 *
 * Output:          Write/Read non-volatile config variables.
 *
 * Side Effects:    None
 *
 * Overview:        None
 *
 * Note:            None
 ********************************************************************/
static void InitAppConfig(void) {
	AppConfig.Flags.bIsDHCPEnabled = TRUE;
	AppConfig.Flags.bInConfigMode = TRUE;

	// ML using sdcc (MPLAB has a trick to generate serial numbers)
	// first 3 bytes indicate manufacturer; last 3 bytes are serial number
	AppConfig.MyMACAddr.v[0] = 0;
	AppConfig.MyMACAddr.v[1] = 0x04;
	AppConfig.MyMACAddr.v[2] = 0xA3;
	AppConfig.MyMACAddr.v[3] = 0x01;
	AppConfig.MyMACAddr.v[4] = 0x02;
	AppConfig.MyMACAddr.v[5] = 0x03;

	// ML if you want to change, see TCPIPConfig.h
	AppConfig.MyIPAddr.Val = MY_DEFAULT_IP_ADDR_BYTE1 |
		MY_DEFAULT_IP_ADDR_BYTE2 << 8ul | MY_DEFAULT_IP_ADDR_BYTE3 << 16ul |
		MY_DEFAULT_IP_ADDR_BYTE4 << 24ul;
	AppConfig.DefaultIPAddr.Val = AppConfig.MyIPAddr.Val;
	AppConfig.MyMask.Val = MY_DEFAULT_MASK_BYTE1 |
		MY_DEFAULT_MASK_BYTE2 << 8ul | MY_DEFAULT_MASK_BYTE3 << 16ul |
		MY_DEFAULT_MASK_BYTE4 << 24ul;
	AppConfig.DefaultMask.Val = AppConfig.MyMask.Val;
	AppConfig.MyGateway.Val = MY_DEFAULT_GATE_BYTE1 |
		MY_DEFAULT_GATE_BYTE2 << 8ul | MY_DEFAULT_GATE_BYTE3 << 16ul |
		MY_DEFAULT_GATE_BYTE4 << 24ul;
	AppConfig.PrimaryDNSServer.Val = MY_DEFAULT_PRIMARY_DNS_BYTE1 |
		MY_DEFAULT_PRIMARY_DNS_BYTE2 << 8ul |
		MY_DEFAULT_PRIMARY_DNS_BYTE3 << 16ul |
		MY_DEFAULT_PRIMARY_DNS_BYTE4 << 24ul;
	AppConfig.SecondaryDNSServer.Val = MY_DEFAULT_SECONDARY_DNS_BYTE1 |
		MY_DEFAULT_SECONDARY_DNS_BYTE2 << 8ul |
		MY_DEFAULT_SECONDARY_DNS_BYTE3 << 16ul |
		MY_DEFAULT_SECONDARY_DNS_BYTE4 << 24ul;
}

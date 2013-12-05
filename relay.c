/*
 * relay.c
 *
 * Main program
 */

#define __SDCC__
#define THIS_INCLUDES_THE_MAIN_FUNCTION

#include "platform/display.h"
#include "platform/led.h"

#define LOW(a)     (a & 0xFF)
#define HIGH(a)    ((a>>8) & 0xFF)

/**
 * Main routine
 */
void main(void) {
	// Initialize display
	led_init();
	display_init();
	led_set_all(FALSE);

	// TODO
}

/**
 * Low-priority interrupt routine.
 */
void low_isr(void)
__interrupt (2) {
	// TODO
}

/**
 * High-priority interrupt routine.
 */
void high_isr(void)
__interrupt (1) {
	// TODO
}

/*
 * pic16/led.c
 */

#define __18F97J60
#define __SDCC__
#include "../Include/HardwareProfile.h"

#include "../platform/led.h"

void led_init(void) {
	LED0_TRIS = 0;  //LED0 (yellow)
	LED1_TRIS = 0;  //LED1 (red)
	LED2_TRIS = 0;  //LED2 (red)
	LED3_TRIS = 0;  //LED_LCD1
	LED4_TRIS = 0;  //LED_LCD2
	LED5_TRIS = 0;  //LED5=RELAY1
	LED6_TRIS = 0;  //LED7=RELAY2
#if (!defined(EXPLORER_16) &&!defined(OLIMEX_MAXI))
	// Pin multiplexed with a button
	// on EXPLORER_16 and not used on OLIMEX_MAXI
	LED7_TRIS = 0;
#endif
	LED_PUT(0x00);  // turn off LED0 - LED2
	RELAY_PUT(0x00); // turn relays off to save power
}

BOOL led_get(enum led_e led) {
	switch (led) {
	case LED0:
		return LED0_IO != 0u;
	case LED1:
		return LED1_IO != 0u;
	case LED2:
		return LED2_IO != 0u;
	default:
		return FALSE;
	}
}

void led_set(enum led_e led, BOOL state) {
	switch (led) {
	case LED0:
		LED0_IO = (state ? 1u : 0u);
		break;
	case LED1:
		LED1_IO = (state ? 1u : 0u);
		break;
	case LED2:
		LED2_IO = (state ? 1u : 0u);
		break;
	}
}

void led_set_all(BOOL state) {
	LED0_IO = LED1_IO = LED2_IO = (state ? 1u : 0u);
}

void led_toggle(enum led_e led) {
	switch (led) {
	case LED0:
		LED0_IO ^= 1;
		break;
	case LED1:
		LED1_IO ^= 1;
		break;
	case LED2:
		LED2_IO ^= 1;
		break;
	}
}

void led_toggle_all() {
	LED0_IO ^= 1;
	LED1_IO ^= 1;
	LED2_IO ^= 1;
}

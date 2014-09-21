//Test program for the ws2812 driver

#include <avr/io.h>
#include <util/delay.h>
#include "ws2812_driver.h"

//Number of LED's to update
#define NUM_LEDS    5

//Color wheel speed
#define COLOR_WHEEL_SPEED 42

//Delay between refreshes of LED's and advancement of hue in color wheel
#define DELAY_MS    15

//helper defines for color wheel
#define HUE_STEPS (6*256-1)
#define HUE_TOP (HUE_STEPS-1)


void color_hexagon(uint16_t hue, struct led_grb* led) {
    uint8_t frac = hue >> 8;
    uint8_t ci = hue & 0xFF;
    uint8_t cd = 0xFF - ci;
    uint8_t cs = 0xFF;
    switch (frac) {
        case 0: led->r = cs;    led->g = ci;    led->b = 0; break;      //R1    G+  B0
        case 1: led->r = cd;    led->g = cs;    led->b = 0; break;      //R-    G1  B0
        case 2: led->r = 0; led->g = cs;    led->b = ci; break; //R0    G1  B+
        case 3: led->r = 0; led->g = cd;    led->b = cs; break; //R0    G-  B1
        case 4: led->r = ci;    led->g = 0; led->b = cs; break; //R+    G0  B1
        case 5: led->r = cs;    led->g = 0; led->b = cd; break; //R1    G0  B-
    }
}




// program entry point
int main(void) {

    //Init port to zero and set direction to output
    ws2812_init();
    
    //Color wheel hue    
    uint16_t hue=0;

    //LED buffer
    struct led_grb led_data[NUM_LEDS];

    while (1) {

        hue = (hue + COLOR_WHEEL_SPEED) % HUE_STEPS;

        uint16_t i;
        for (i=0; i<NUM_LEDS; i++) {
            color_hexagon( (hue + (i<<8)) % HUE_STEPS, &led_data[i]);
        }

        ws2812_send(led_data, NUM_LEDS);
        
        //Wait a bit before next refresh
        _delay_ms(DELAY_MS);
    }
    
    return 0;
}

#ifndef WS2812_DRIVER_H
#include "ws2812_config.h"

//Struct for LED data
struct led_grb {
    uint8_t g;
    uint8_t r;
    uint8_t b;
};

//Function calls in driver
extern void ws2812_init();
extern void ws2812_send(struct led_grb* buffer, uint8_t length);


#define WS2812_DRIVER_H
#endif

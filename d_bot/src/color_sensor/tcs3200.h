/**
 * @file
 * Author : Mario M.
 * 
 * @brief Public API for ...
 */

#ifndef TCS3200_H
#define TCS3200_H

#include <zephyr.h>

typedef enum { red_f = 0, green_f, blue_f } filter_t;
typedef enum { R, G, B, CLEAR } color_t;

void color_sensor_init();
u32_t us_get(filter_t filter);
u8_t color_get();

#endif /*TCS3200_H*/
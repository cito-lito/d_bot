/**
 * @file
 * Author : Mario M.
 * 
 * @brief Public API for the use od the tcs3200 color sensor
 */

#ifndef TCS3200_H
#define TCS3200_H

#include <zephyr.h>

typedef enum { red_f = 0, green_f, blue_f } filter_t;
typedef enum { RED, GREEN, BLUE, CLEAR } color_t;

/**
 * @brief Sensor init.
 * @return 0 if successful, not 0 on failure.
 */
s8_t color_sensor_init();


/**
 * @brief Detect a Color, based on the frequency that a color reflect the light. 
 * Note that the values in this function worked in a specific environment. 
 * The values can be adjusted for other environments.
 * @return detected color.
 */
color_t color_get();

#endif /*TCS3200_H*/
/**
 * @file
 * Author : Mario M.
 * 
 * @brief Public API for the ultra sonic sensor
 */

#ifndef USONIC_H
#define USONIC_H
#include <zephyr.h>


/**
 * @brief Sensor init.
 * @return 0 if successful, not 0 on failure.
 */
u8_t usonic_init();

/**
 * @brief Activate the trigger and wait for the echo response.
 * @return The distance in cm
 */
u32_t usonic_distance_cm_get();

#endif /*USONIC_H*/
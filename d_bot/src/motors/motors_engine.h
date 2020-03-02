/**
 * @file
 * Author : Mario M.
 * 
 * @brief Public API for the control of the DC Motors
 */

#ifndef MOTORS_ENGINE_H
#define MOTORS_ENGINE_H
#include <zephyr.h>

/*drive directions*/
typedef enum {
	forwards = 0,
	backwards,
	rightwards,
	leftwards,
	stop
} direction_t;

/*spped of motors*/
typedef enum { normal = 0, fast } speed_t;

/**
 * @brief Motors init.
 * @return 0 if successful, not 0 on failure.
 */
s8_t motors_init();

/**
 * @brief Set speed of motors.
 * @param speed: normal or fast.
 * @return 0 if successful, not 0 on failure.
 */
s8_t speed(speed_t speed);

/**
 * @brief Set the drive direction.
 * @param dir: Directions and stop.
 * @return 0 if successful, not 0 on failure.
 */
s8_t drive(direction_t dir);

#endif /*MOTORS_ENGINE_H*/
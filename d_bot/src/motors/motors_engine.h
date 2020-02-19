/**
 * @file
 * Author : Mario M.
 * 
 * @brief Public API for ...
 */

#ifndef MOTORS_ENGINE_H
#define MOTORS_ENGINE_H
#include <zephyr.h>
/*drive directions*/
typedef enum direction {
	forwards = 0,
	backwards,
	rightwards,
	leftwards,
	stop
} dir_t;

/*spped of motors*/
typedef enum speed { normal = 0, fast } speed_t;

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
s8_t drive(dir_t dir);

#endif /*MOTORS_ENGINE_H*/
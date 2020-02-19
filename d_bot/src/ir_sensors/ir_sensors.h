/**
 * @file
 * Author : Mario M.
 * 
 * @brief Public API for ...
 */

#ifndef IR_SENSORS_H
#define IR_SENSORS_H
#include <zephyr.h>

u8_t ir_sensors_init();
u8_t ir_sensor_left();
u8_t ir_sensor_right();

#endif /*IR_SENSORS_H*/
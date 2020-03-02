/**
 * @file
 * Author : Mario M.
 * 
 * @brief Public API for the ir sensors
 */

#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#define IR_LEFT 12 /*PA_12*/
#define IR_RIGTH 11 /*PA_11*/

#include <zephyr.h>

/*Gloval vars*/
extern struct device *port_a;
extern struct gpio_callback gpio_cb_r;
extern struct gpio_callback gpio_cb_l;

/******************Interrups*****************/

/**
 * @brief drives left for 10ms
 */
void ir_left_int(struct device *port_a, struct gpio_callback *cb, u32_t pin);

/**
 * @brief drives right for 10ms
 */
void ir_right_int(struct device *port_a, struct gpio_callback *cb, u32_t pin);

/****************Public Fucntions************/

/**
 * @brief Sensor init.
 * @return 0 if successful, not 0 on failure.
 */
u8_t ir_sensors_init();

/**
 * @brief Get the value of ir sensor of the left
 * @return 0 if no line detected , 1 on line.
 */
u8_t ir_sensor_left();

/**
 * @brief Get the value of ir sensor of the right
 * @return 0 if no line detected , 1 on line.
 */
u8_t ir_sensor_right();

#endif /*IR_SENSORS_H*/
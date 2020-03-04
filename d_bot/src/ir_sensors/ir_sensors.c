/**
 *  Author: Mario M.
 * 
 * @brief This program give the values from the ir sensors
 * 1 for a black line and 0 if clear.
 * 
 * Commented out code is because interrups not working as spected, need to be fixed
 * For now read polling data.
 * 
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <misc/printk.h>
#include "d_bot_util.h"
#include "ir_sensors.h"
#include "motors_engine.h"

/*this defines are not in header because they are only use here*/
#define A_PORT "GPIOA" /*port A*/

/*Interrups Flags*/
#define INT_FLAGS (GPIO_DIR_IN | GPIO_INT | GPIO_PUD_PULL_UP | GPIO_INT_EDGE)

/*device drivers*/
struct device *port_a;

/*gpio callback struct*/
struct gpio_callback gpio_cb_r;
struct gpio_callback gpio_cb_l;


/******************Privates Functions*****************/
PRIVATE u8_t init();
PRIVATE u8_t ir_left();
PRIVATE u8_t ir_right();

/**
 * @brief Init configuration.
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE u8_t init()
{
	port_a = device_get_binding(A_PORT);
	if (!port_a) {
		printk("Cannot find %s!\n", A_PORT);
		return E_FAIL;
	}
	s8_t ret = E_OK; /*no error*/

	ret |= gpio_pin_configure(port_a, IR_LEFT, GPIO_DIR_IN);
	ret |= gpio_pin_configure(port_a, IR_RIGTH, GPIO_DIR_IN);

	// /*init interrups*/
	// ret |= gpio_pin_configure(port_a, IR_LEFT, INT_FLAGS);
	// gpio_init_callback(&gpio_cb_l, ir_left_int, BIT(IR_LEFT));
	// ret |= gpio_add_callback(port_a, &gpio_cb_l);
	// ret |= gpio_pin_enable_callback(port_a, IR_LEFT);

	// ret |= gpio_pin_configure(port_a, IR_RIGTH, INT_FLAGS);
	// gpio_init_callback(&gpio_cb_r, ir_right_int, BIT(IR_RIGTH));
	// ret |= gpio_add_callback(port_a, &gpio_cb_r);
	// ret |= gpio_pin_enable_callback(port_a, IR_RIGTH);
	return ret;
}

/**
 * @brief Get the value of ir sensor of the left
 * @return 0 if no line detected , 1 on line.
 */
PRIVATE u8_t ir_left()
{
	u32_t var;
	gpio_pin_read(port_a, IR_LEFT, &var);
	return var;
}

/**
 * @brief Get the value of ir sensor of the right
 * @return 0 if no line detected , 1 on line.
 */
PRIVATE u8_t ir_right()
{
	u32_t var;
	gpio_pin_read(port_a, IR_RIGTH, &var);
	return var;
}

/****************Public Fucntions*********************/
u8_t ir_sensors_init()
{
	return init();
}

u8_t ir_sensor_left()
{
	return ir_left();
}
u8_t ir_sensor_right()
{
	return ir_right();
}

/******************Interrups*****************/

// s8_t disable_ir_sensors_interrupts()
// {
// 	s8_t ret = E_OK;
// 	ret |= gpio_pin_disable_callback(port_a, IR_RIGTH);
// 	ret |= gpio_pin_disable_callback(port_a, IR_LEFT);
// 	return ret;
// }

// s8_t enable_ir_sensors_interrupts()
// {
// 	s8_t ret = E_OK;
// 	ret |= gpio_pin_enable_callback(port_a, IR_RIGTH);
// 	ret |= gpio_pin_enable_callback(port_a, IR_LEFT);
// 	return ret;
// }

// void ir_left_int(struct device *port_a, struct gpio_callback *cb, u32_t pin)
// {
// 	drive(leftwards);
// 	k_busy_wait(100);
// 	// drive(forwards);

// }

// void ir_right_int(struct device *port_a, struct gpio_callback *cb, u32_t pin)
// {
// 	drive(rightwards);
// 	k_busy_wait(100);
// 	// drive(forwards);

// }

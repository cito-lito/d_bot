/**
 *  Author: Mario M
 * 
 * @brief This program determines the color read from the color sensor 
 * by counting the time of reflection of a color.
 * 
 */

#include <device.h>
#include <drivers/gpio.h>
#include <misc/printk.h>
#include <sys/printk.h>
#include "d_bot_util.h"
#include "tcs3200.h"

/*this defines are not in header because they are only use here*/
#define A_PORT "GPIOA" /*port A*/

/*Sensor Pins*/
#define S0 1 /*PA 1*/
#define S1 4 /*PA_4*/
#define S2 13 /*PA_13*/
#define S3 14 /*PA_14*/
#define S_OUT 15 /*PA_15*/

/*Interrups Flags*/
#define INT_FLAGS                                                              \
	(GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE | GPIO_INT_DOUBLE_EDGE)

/*Gloval( for this file ) variables*/
PRIVATE u32_t red, green, blue;
PRIVATE u32_t call_count_cb;
PRIVATE u32_t start_cycles_cb;
PRIVATE u32_t stop_cycles_cb;
PRIVATE u32_t cycles_spent_cb;
PRIVATE u32_t us_spent_cb;

/*device drivers*/
PRIVATE struct device *port_a;

/*gpio callback struct*/
PRIVATE struct gpio_callback gpio_cb;

/******************Interrups***************************/

void us_callback(struct device *gpio, struct gpio_callback *cb, u32_t pins);

/******************Privates Functions*****************/

PRIVATE s8_t init();
PRIVATE u32_t us_get(filter_t filter);

/**
 * @brief Init configuration.
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE s8_t init()
{
	port_a = device_get_binding(A_PORT);
	if (!port_a) {
		printk("Cannot find %s!\n", A_PORT);
		return E_FAIL;
	}
	s8_t ret = E_OK; /*no error*/
	ret |= gpio_pin_configure(port_a, S0, GPIO_DIR_OUT);
	ret |= gpio_pin_configure(port_a, S1, GPIO_DIR_OUT);
	ret |= gpio_pin_configure(port_a, S2, GPIO_DIR_OUT);
	ret |= gpio_pin_configure(port_a, S3, GPIO_DIR_OUT);

	/*set output frequency scaling (20%)*/
	ret |= gpio_pin_write(port_a, S0, 1);
	ret |= gpio_pin_write(port_a, S1, 0);

	/*init interrups*/
	ret |= gpio_pin_configure(port_a, S_OUT, INT_FLAGS);
	gpio_init_callback(&gpio_cb, us_callback, BIT(S_OUT));
	ret |= gpio_add_callback(port_a, &gpio_cb);
	ret |= gpio_pin_enable_callback(port_a, S_OUT);

	return ret;
}

/**
 * @brief Get the time reflection in us depending on the color filter,
 * distance and color of the surface. 
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE u32_t us_get(filter_t filter)
{
	switch (filter) {
	case red_f:
		// red filter
		gpio_pin_write(port_a, S2, 0);
		k_usleep(60);
		gpio_pin_write(port_a, S3, 0);
		break;
	case green_f:
		//green filter
		gpio_pin_write(port_a, S2, 1);
		k_usleep(60);
		gpio_pin_write(port_a, S3, 1);
		break;
	case blue_f:
		// blue filter
		gpio_pin_write(port_a, S2, 0);
		k_usleep(60);
		gpio_pin_write(port_a, S3, 1);
		break;
	}

	u32_t val = 0;
	do {
		gpio_pin_read(port_a, S_OUT, &val);

	} while (val == 0);
	return (us_spent_cb);
}


/******************Interrups*****************/

/**
 * @brief Counts the time reflection depending on
 * the color surface and distance
 */
void us_callback(struct device *gpio, struct gpio_callback *cb, u32_t pins)
{
	if (0 == (call_count_cb % 2)) {
		/*Read the hardware clock*/
		start_cycles_cb = k_cycle_get_32();
		call_count_cb++;
	} else if (1 == (call_count_cb % 2)) {
		/*Read the hardware clock*/
		stop_cycles_cb = k_cycle_get_32();
		cycles_spent_cb = stop_cycles_cb - start_cycles_cb;
		/*Convert hardware cycles to microseconds*/
		us_spent_cb = k_cyc_to_us_floor32(cycles_spent_cb);
		call_count_cb++;
	}
}

/****************Public Fucntions*********************/

s8_t color_sensor_init()
{
	return init();
}

color_t color_get()
{
	red = us_get(red_f);
	k_sleep(10);
	green = us_get(green_f);
	k_sleep(10);
	blue = us_get(blue_f);
	k_sleep(10);
	if (red > 80 && green < 35 && green > 20) {
		return RED;
	} else if (red < 40 && blue < 40 && green < 20) {
		return BLUE;
	} else {
		return CLEAR;
	}
}

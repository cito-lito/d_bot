/**
 *  Author: Mario M, based on Arduino Code 
 * 
 * 
 */

#include <device.h>
#include <drivers/gpio.h>
#include <misc/printk.h>
#include <sys/printk.h>
#include "d_bot_util.h"
#include "tcs3200.h"

#define A_PORT "GPIOA" //

/*Sensor Pins*/
#define S0 1
#define S1 4

#define S2 13
#define S3 14
#define S_OUT 15

static u32_t red, green, blue;
static struct device *port_a;

static struct gpio_callback gpio_cb;

#define INT_FLAGS                                                              \
	(GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE | GPIO_INT_DOUBLE_EDGE)

void us_callback(struct device *gpio, struct gpio_callback *cb, uint32_t pins);

PRIVATE void init()
{
	port_a = device_get_binding(A_PORT);

	gpio_pin_configure(port_a, S0, GPIO_DIR_OUT);
	gpio_pin_configure(port_a, S1, GPIO_DIR_OUT);
	gpio_pin_configure(port_a, S2, GPIO_DIR_OUT);
	gpio_pin_configure(port_a, S3, GPIO_DIR_OUT);

	// 20 ppor ciento
	gpio_pin_write(port_a, S0, 1);
	gpio_pin_write(port_a, S1, 0);

	gpio_pin_configure(port_a, S_OUT, INT_FLAGS);
	gpio_init_callback(&gpio_cb, us_callback, BIT(S_OUT));
	gpio_add_callback(port_a, &gpio_cb);
	gpio_pin_enable_callback(port_a, S_OUT);
}
static uint32_t count_cb;
static uint32_t start_cycles_cb;
static uint32_t stop_cycles_cb;
static uint32_t cycles_spent_cb;
static uint32_t us_spent_cb;

void us_callback(struct device *gpio, struct gpio_callback *cb, uint32_t pins)
{
	if (0 == (count_cb % 2)) {
		start_cycles_cb = k_cycle_get_32();
		count_cb++;
	} else if (1 == (count_cb % 2)) {
		stop_cycles_cb = k_cycle_get_32();
		cycles_spent_cb = stop_cycles_cb - start_cycles_cb;
		us_spent_cb = k_cyc_to_us_floor32(cycles_spent_cb);
		count_cb++;
	}
}

void color_sensor_init()
{
	init();
}

u32_t us_get(filter_t filter)
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

u8_t color_get()
{
	red = us_get(red_f);
	k_sleep(10);
	green = us_get(green_f);
	k_sleep(10);
	blue = us_get(blue_f);
	k_sleep(10);
	if (red > 80 && green < 35 && green > 20) {
		return R;
	} else if (red < 40 && blue < 40 && green < 20) {
		return B;
	} else {
		return CLEAR;
	}
}
// void main(void)
// {
// 	init();
// 	while (1) {
// 		red = us_get(red_f);

// 		k_sleep(100);
// 		green = us_get(green_f);

// 		k_sleep(100);
// 		blue = us_get(blue_f);

// 		k_sleep(100);

// 		if (red > 80 && green < 35 && green > 20) {
// 			printk("RED detected\n");
// 		} else if (red < 40 && blue < 40 && green < 20) {
// 			printk("BLUE detected\n");
// 		} else {
// 			printk("NO COLORs \n");
// 		}
// 	}
// }
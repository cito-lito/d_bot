/**
 *  Author: Mario M.
 * 
 * 
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <misc/printk.h>
#include "d_bot_util.h"

#define A_PORT "GPIOA" //
#define IR_LEFT 12 /**/
#define IR_RIGTH 11 /**/
#define INT_FLAGS (GPIO_DIR_IN | GPIO_INT | GPIO_PUD_PULL_UP | GPIO_INT_EDGE)

static struct device *port_a;
static struct gpio_callback gpio_cb_r;
static struct gpio_callback gpio_cb_l;

PRIVATE u8_t init()
{
	u8_t ret = E_OK;
	port_a = device_get_binding(A_PORT);
	if (!port_a) {
		printk("Cannot find %s!\n", A_PORT);
		return E_FAIL;
	}
	ret |= gpio_pin_configure(port_a, IR_LEFT, GPIO_DIR_IN);
	ret |= gpio_pin_configure(port_a, IR_RIGTH, GPIO_DIR_IN);
	return ret;
}

PRIVATE  u8_t ir_left()
{
	u32_t var;
	gpio_pin_read(port_a, IR_LEFT, &var);
	return var;
}

PRIVATE  u8_t ir_right()
{
	u32_t var;
	gpio_pin_read(port_a, IR_RIGTH, &var);
	return var;
}

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

// void ir_left_int(struct device *port_a, struct gpio_callback *cb, u32_t pin)
// {
// 	printk("left");
// }

// void ir_right_int(struct device *port_a, struct gpio_callback *cb, u32_t pin)
// {
// 	printk("right");
// }

// PRIVATE void init_interrupts()
// {
// 	/**/
// 	gpio_pin_configure(port_a, IR_LEFT, INT_FLAGS);
// 	gpio_init_callback(&gpio_cb_l, ir_left_int, BIT(IR_LEFT));
// 	gpio_add_callback(port_a, &gpio_cb_l);
// 	gpio_pin_enable_callback(port_a, IR_LEFT);

// 	/**/
// 	gpio_pin_configure(port_a, IR_RIGTH, INT_FLAGS);
// 	gpio_init_callback(&gpio_cb_r, ir_right_int, BIT(IR_RIGTH));
// 	gpio_add_callback(port_a, &gpio_cb_r);
// 	gpio_pin_enable_callback(port_a, IR_RIGTH);
// }

// void main()
// {
// 	init();
// 	init_interrupts();
// 	// s32_t var;
// 	// while (1) {
// 	// 	gpio_pin_read(port_a, IR_LEFT, &var);
// 	// 	k_sleep(200);
// 	// 	printk("left %d\n", var);
// 	// 	gpio_pin_read(port_a, IR_RIGTH, &var);
// 	// 	k_sleep(200);
// 	// 	printk("right %d\n", var);
// 	// }
// }
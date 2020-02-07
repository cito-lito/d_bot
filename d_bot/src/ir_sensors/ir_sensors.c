/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <misc/printk.h>

#define A_PORT "GPIOA" //
#define IR_LEFT 13 /**/
#define IR_RIGTH 14 /**/
#define INT_FLAGS (GPIO_DIR_IN | GPIO_INT | GPIO_PUD_PULL_UP | GPIO_INT_EDGE)
#define HIGH 1 /*logic hihg ~2.5V to ~5V*/
#define LOW 0 /*logic low ~0V to ~1V*/

#define PRIVATE static
static struct device *port_a;
static struct gpio_callback gpio_cb_r;
static struct gpio_callback gpio_cb_l;

void init()
{
	port_a = device_get_binding(A_PORT);
	if (!port_a) {
		printk("error\n");
		return;
	}
	gpio_pin_configure(port_a, IR_LEFT, GPIO_DIR_IN);
	gpio_pin_configure(port_a, IR_RIGTH, GPIO_DIR_IN);
}

void ir_left_int(struct device *port_a, struct gpio_callback *cb, u32_t pin)
{
}

void ir_right_int(struct device *port_a, struct gpio_callback *cb, u32_t pin)
{
}

void init_interrupts()
{
	/**/
	gpio_pin_configure(port_a, IR_LEFT, INT_FLAGS);
	gpio_init_callback(&gpio_cb_l, ir_left_int, BIT(IR_LEFT));
	gpio_add_callback(port_a, &gpio_cb_l);
	gpio_pin_enable_callback(port_a, IR_LEFT);

	/**/
	gpio_pin_configure(port_a, IR_RIGTH, INT_FLAGS);
	gpio_init_callback(&gpio_cb_r, ir_right_int, BIT(IR_RIGTH));
	gpio_add_callback(port_a, &gpio_cb_r);
	gpio_pin_enable_callback(port_a, IR_RIGTH);
}

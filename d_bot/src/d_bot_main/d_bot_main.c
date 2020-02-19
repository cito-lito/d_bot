/**
 *  Author: Mario M.
 * 
 * 
 */

#include "d_bot_util.h"
#include "motors_engine.h"
#include "tcs3200.h"
#include "ir_sensors.h"
#include "usonic.h"
#define N 3
#define K 3
void init()
{
	motors_init();
	k_sleep(100);
	usonic_init();
	k_sleep(100);
	ir_sensors_init();
	k_sleep(100);
	color_sensor_init();
	k_sleep(100);
}

void init_setups()
{
	speed(normal);
}

u8_t mapN[N] = { 0, 1, 2 };
u8_t mapK[K] = { 0, 1, 2 };

u8_t n_counter = 0;


void follow_line()
{
	drive(forwards);
	k_sleep(10);
	if (ir_sensor_right()) {
		drive(rightwards);
		k_sleep(20);
	}

	if (ir_sensor_left()) {
		drive(leftwards);
		k_sleep(20);
	}
}

void wait_color_change()
{
	for (;;) {
		if (color_get() == B) {
			printk("B there\n");
			follow_line();
			k_sleep(10);
		} else {
			break;
		}
	}
}
void find_N(u8_t n)
{
	speed(normal);
	while ((mapN[n_counter]) < n) {
		follow_line();
		while(usonic_distance_cm_get() < 20){
			drive(stop);
		}
		speed(normal);
		follow_line();
		if (color_get() == B && (n_counter % 2)) {
			printk("impar %d\n", n_counter);
			follow_line();
			wait_color_change();
		}
		printk("main: %d\n", n_counter);
		follow_line();
		if (color_get() == B) {
			//printk("B detected\n");
			n_counter++;
			printk("B detected : %d\n", n_counter);
			break;
		}
	}
	drive(stop);
	k_sleep(100);
}

void turn_right()
{
	speed(normal);
	while (1) {
		drive(rightwards);
		if (ir_sensor_left()) {
			printk("r there\n");
			drive(stop);
			break;
		}
	}
	speed(normal);
}

void main()
{
	init();
	init_setups();

	// while (1) {
	// 	printk("L = %d\n", ir_sensor_left());
	// 	printk("R = %d\n", ir_sensor_right());
	// 	printk("color =  %d\n", color_get());
	// 	k_sleep(100);
	// }

	while (n_counter < 2) {
		find_N(2);
	}
	printk("turn right\n");
	printk(" %d  ", ir_sensor_right());
	
	turn_right();

	n_counter = 0;
	while (n_counter < 2) {
		find_N(2);
	}
}
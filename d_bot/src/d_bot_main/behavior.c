/**
 *  Author: Mario M
 * 
 * @brief This program implements the behavior functions
 * 
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include "d_bot_util.h"
#include "motors_engine.h"
#include "tcs3200.h"
#include "ir_sensors.h"
#include "usonic.h"

/* Set the size of the Map (n*k matrix)*/
#define N 3
#define K 3

u8_t blue_flag;
u8_t node_counter;

u8_t map_nodes_n[N] = { 0, 1, 2 };
u8_t map_nodes_k[K] = { 0, 1, 2 };

direction_flag_t dir_t = FORWARDS;

// void follow_line()
// {
// 	drive(forwards);
// 	k_sleep(10);
// 	if (ir_sensor_right()) {
// 		drive(rightwards);
// 		k_sleep(10);
// 	}

// 	if (ir_sensor_left()) {
// 		drive(leftwards);
// 		k_sleep(10);
// 	}
// }

void follow_line()
{
	dir_t = FORWARDS;
	drive(forwards);
	k_sleep(10);
	switch (dir_t) {
	case RIGHTWARDS:
		drive(rightwards);
		k_sleep(10);
		break;
	case LEFTWARDS:
		drive(leftwards);
		k_sleep(10);
		break;
	}
}

void turn_right()
{
	disable_ir_sensors_interrupts();
	while (1) {
		drive(rightwards);
		if (ir_sensor_left()) {
			drive(stop);
			break;
		}
	}
	enable_ir_sensors_interrupts();
}

void wait_color_change()
{
	while (1) {
		if (color_get() == BLUE) {
			follow_line();
		} else {
			break;
		}
	}
}

void find_node(u8_t n)
{
	while ((map_nodes_n[node_counter]) < n) {
		follow_line();

		while (usonic_distance_cm_get() < 20) {
			drive(stop);
		}

		follow_line();

		if ((color_get() == BLUE) && blue_flag) {
			wait_color_change();
			blue_flag = 0;
		}

		follow_line();

		if (color_get() == BLUE) {
			blue_flag = 1;
			node_counter++;
			break;
		}
	}
	drive(stop);
}

/*Not working with interups*/
// void follow_line()
// {
//   k_sleep(10);
// 	drive(forwards);
// }

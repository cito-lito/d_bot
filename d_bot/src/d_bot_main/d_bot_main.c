/**
 *  Author: Mario M
 * 
 * @brief This program implements the drive to goal algorithm
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
#include "behavior.h"

PRIVATE s8_t d_boot_init();
PRIVATE s8_t find_destination_node(u8_t n, u8_t k);

PRIVATE s8_t d_boot_init()
{
	s8_t ret = E_OK;

	ret |= motors_init();
	k_sleep(100);

	ret |= usonic_init();
	k_sleep(100);
	ret |= ir_sensors_init();
	k_sleep(100);

	ret |= color_sensor_init();
	k_sleep(100);

	/*set speed*/
	speed(normal);
	k_sleep(100);

	return ret;
}

/**
 * @brief given 2 points, find the destination point.
 * first drive in Y+, then in X+
 */
PRIVATE s8_t find_destination_node(u8_t n, u8_t k)
{
	if (n > N || k > K) {
		printk("ERROR: Enter a valid point\n");
		return E_FAIL;
	}
	/*set node counter to 0*/
	node_counter = 0;

	/*find fírst node*/
	while (node_counter < n) {
		find_node(n);
	}

	/*turn right when first node has been found*/
	turn_right();

	/*set node counter to 0*/
	node_counter = 0;

	/*finde second node*/
	while (node_counter < k) {
		find_node(k);
	}
	return E_OK;
}

int main(void)
{
	if (d_boot_init()) {
		return E_FAIL;
	}

	/*find the node 2, 2
	*
	*	2	1  -2-	*..... 
	*
	*	1	1	2	*.....
	*	
	*	0	1	2	*.....
	*	
	*/
	find_destination_node(2, 2);

	return E_OK;
}
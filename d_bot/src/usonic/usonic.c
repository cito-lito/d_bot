#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include "d_bot_util.h"

#define PORT_A "GPIOA"

#define TRIGGER 8 /*PA_8*/
#define ECHO 9 /*PA_9*/

/**
 * speed of sound is 343 m/s: 0.343/us.
 * Distance = Time * speed
 * Distance = us * (0.0343 / 2) // Divided by 2 because 
 * echo need to go and bounce back.
 */
#define DISTANCE_CM(x) ((u32_t)(x * 0.01715f))

#define INT_FLAGS                                                              \
	(GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE | GPIO_INT_DOUBLE_EDGE)

PRIVATE struct gpio_callback gpio_cb;

PRIVATE u32_t count_cb;
PRIVATE u32_t start_cycles_cb;
PRIVATE u32_t stop_cycles_cb;
PRIVATE u32_t cycles_spent_cb;
PRIVATE u32_t echo_us_cb;

PRIVATE struct device *a_port;

/**
 * Interrupt to count the time (in us) that an Echo signal
 * needs to go and bounce back.
 */
void echo_us_callback(struct device *gpio, struct gpio_callback *cb, u32_t pins)
{
	if (0 == (count_cb % 2)) {
		start_cycles_cb = k_cycle_get_32();
		count_cb++;
	} else if (1 == (count_cb % 2)) {
		stop_cycles_cb = k_cycle_get_32();
		cycles_spent_cb = stop_cycles_cb - start_cycles_cb;
		echo_us_cb = k_cyc_to_us_floor32(cycles_spent_cb);
		count_cb++;
	}
}

/**
 * @brief Init configuration.
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE u8_t init()
{
	a_port = device_get_binding(PORT_A);
	if (!a_port) {
		printk("Cannot find %s!\n", PORT_A);
		return E_FAIL;
	}
	s8_t ret = E_OK; /*no error*/
	ret |= gpio_pin_configure(a_port, TRIGGER, GPIO_DIR_OUT);
	ret |= gpio_pin_configure(a_port, ECHO, INT_FLAGS);
	/*interrups*/
	gpio_init_callback(&gpio_cb, echo_us_callback, BIT(ECHO));
	ret |= gpio_add_callback(a_port, &gpio_cb);
	ret |= gpio_pin_enable_callback(a_port, ECHO);
	return ret;
}

u8_t usonic_init()
{
	return init();
}

u32_t usonic_distance_cm_get()
{
	u32_t val = 0;
	gpio_pin_write(a_port, TRIGGER, LOW);
	k_usleep(60);
	gpio_pin_write(a_port, TRIGGER, HIGH);

	do {
		if (gpio_pin_read(a_port, ECHO, &val))
			return E_FAIL;
	} while (val);

	return (u32_t)DISTANCE_CM(echo_us_cb);
}
// void main()
// {
// 	printk("starting\n");
// 	init();
// 	while (1) {
// 		u32_t a = usonic_distance_cm_get();
// 		printk("%d\n", a);
// 		k_sleep(100);
// 	}
// }
/*
 * 
 *
 * 
 */
#include <device.h>
#include <drivers/gpio.h>
#include <misc/printk.h>
#include <sys/printk.h>
#include <zephyr.h>

#define A_PORT "GPIOA" //

/*Sensor Pins*/
#define S0 8
#define S1 10

#define S2 0
#define S3 1
#define S_OUT 4

static u32_t red, green, blue;
static struct device *port_a;

static struct gpio_callback gpio_cb;

#define SOUT_FLAGS                                                             \
	(GPIO_DIR_IN | GPIO_INT | GPIO_INT_EDGE | GPIO_INT_DOUBLE_EDGE)

void us_callback(struct device *gpio, struct gpio_callback *cb, uint32_t pins);

void init()
{
	port_a = device_get_binding(A_PORT);

	gpio_pin_configure(port_a, S0, GPIO_DIR_OUT);
	gpio_pin_configure(port_a, S1, GPIO_DIR_OUT);
	gpio_pin_configure(port_a, S2, GPIO_DIR_OUT);
	gpio_pin_configure(port_a, S3, GPIO_DIR_OUT);

	//gpio_pin_configure(port_a, S_OUT, GPIO_DIR_IN);

	// 20 ppor ciento
	gpio_pin_write(port_a, S0, 1);
	gpio_pin_write(port_a, S1, 0);

	//

	gpio_pin_configure(port_a, S_OUT, SOUT_FLAGS);
	gpio_init_callback(&gpio_cb, us_callback, BIT(S_OUT));
	gpio_add_callback(port_a, &gpio_cb);
	gpio_pin_enable_callback(port_a, S_OUT);
}
static uint32_t count_cb;
static uint32_t start_time_cb;
static uint32_t stop_time_cb;
static uint32_t cycles_spent_cb;
static uint32_t us_spent_cb;

void us_callback(struct device *gpio, struct gpio_callback *cb, uint32_t pins)
{
	if (0 == (count_cb % 2)) {
		start_time_cb = k_cycle_get_32();
		count_cb++;
	} else if (1 == (count_cb % 2)) {
		stop_time_cb = k_cycle_get_32();
		cycles_spent_cb = stop_time_cb - start_time_cb;
		us_spent_cb = k_us_to_cyc_floor32(cycles_spent_cb);
		// length_mm_cb = (cycles_spent_cb * 0.001657);
		count_cb++;
	}
}
typedef enum filter { red_f = 0, green_f, blue_f } filter_t;

u32_t us_get(filter_t filter)
{
	switch (filter) {
	case red_f:
		// red filter
		gpio_pin_write(port_a, S2, 0);
		gpio_pin_write(port_a, S3, 0);
		break;
	case green_f:
		//green filter
		gpio_pin_write(port_a, S2, 1);
		gpio_pin_write(port_a, S3, 1);
		break;
	case blue_f:
		// blue filter
		gpio_pin_write(port_a, S2, 0);
		gpio_pin_write(port_a, S3, 1);
		break;
	}

	u32_t val = 2;
	do {
		gpio_pin_read(port_a, S_OUT, &val);

	} while (val == 0);
	return (us_spent_cb/10000 );
}
void main(void)
{
	init();
	while (1) {
		red = us_get(red_f);
		printk("RED: ");
		printk("%d\n", red);
		k_sleep(300);

		// //clear
		// gpio_pin_write(port_a, S2, 1);
		// gpio_pin_write(port_a, S3, 0);
		green = us_get(green_f);
		printk("GREEN: ");
		printk("%d\n", red);
		k_sleep(300);

		// blue = us_get(blue_f);
		// printk("BLUE: ");
		// printk("%d\n", red);
		// k_sleep(300);
	}
}
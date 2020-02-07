#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
//#include <misc/printk.h>

#define PORT_A "GPIOA"

#define SONIC_PORT PORT_A
#define SONIC_TRIG 8
#define SONIC_ECHO 9

#define ECHO_FLAGS                                                             \
	GPIO_DIR_IN | GPIO_PUD_PULL_DOWN | GPIO_INT | GPIO_INT_EDGE |          \
		GPIO_INT_DOUBLE_EDGE

#define NS_TO_US(n) ((((n / 10) / 10) / 10))

static struct gpio_callback gpio_cb;

static uint32_t count_cb = 0U;
static uint32_t start_time_cb = 0U;
static uint32_t stop_time_cb = 0U;
static uint32_t cycles_spent_cb = 0U;
static uint32_t us_spent_cb = 0U;
static uint32_t length_mm_cb = 0U;

static struct device *sonic_trig;
static struct device *sonic_echo;

void echo_time_ns_callback(struct device *gpio, struct gpio_callback *cb,
			   uint32_t pins)
{
	if (0 == (count_cb % 2)) {
		start_time_cb = k_cycle_get_32();
		count_cb++;
	} else if (1 == (count_cb % 2)) {
		stop_time_cb = k_cycle_get_32();
		cycles_spent_cb = stop_time_cb - start_time_cb;
		us_spent_cb = k_us_to_cyc_floor32(cycles_spent_cb);
		length_mm_cb = (cycles_spent_cb * 0.001657f);
		count_cb++;
	}
}

void init()
{
	sonic_trig = device_get_binding(SONIC_PORT);
	if (!sonic_trig) {
		printk("Device get binding on sonic trigger device failed!\n");
	}

	sonic_echo = device_get_binding(SONIC_PORT);
	if (!sonic_echo) {
		printk("Device get binding on sonic echo device failed!\n");
	}

	if (!(0 == gpio_pin_configure(sonic_trig, SONIC_TRIG, GPIO_DIR_OUT))) {
		printk("GPIO pin configure failed!\n");
	}

	if (!(0 == gpio_pin_configure(sonic_echo, SONIC_ECHO, ECHO_FLAGS))) {
		printk("GPIO pin configure failed!\n");
	}

	gpio_init_callback(&gpio_cb, echo_time_ns_callback, BIT(SONIC_ECHO));

	if (!(0 == gpio_add_callback(sonic_echo, &gpio_cb))) {
		printk("GPIO add callback on sonic echo device failed!\n");
	}

	if (!(0 == gpio_pin_enable_callback(sonic_echo, SONIC_ECHO))) {
		printk("GPIO pin enable callback on sonic echo edevice failed!\n");
	}
}

int32_t odm_sonic_value_get()
{
	u32_t val = 0;

	gpio_pin_write(sonic_trig, SONIC_TRIG, 0);
	k_usleep(60);
	gpio_pin_write(sonic_trig, SONIC_TRIG, 1);

	do {
		if (!(0 == gpio_pin_read(sonic_echo, SONIC_ECHO, &val)))
			printk("GPIO pin read failed!\n");
	} while (val);

	return (u32_t)length_mm_cb;
}
void main()
{
	printk("starting\n");
	init();
	while (1) {
		u32_t a = odm_sonic_value_get();
		printk("%d\n", a);
		k_sleep(100);
	}
}
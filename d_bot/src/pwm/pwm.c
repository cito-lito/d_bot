#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <drivers/pwm.h>

#define PWM_DRIVER "PWM_2"
#define PWM_CHANNEL 1

void main(void)
{
	struct device *dev;
	dev = device_get_binding(PWM_DRIVER);

	while (1) {
		pwm_pin_set_usec(dev, PWM_CHANNEL, 100000, 100); 

		k_sleep(1000);
	}
}


/*
 * 
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/pwm.h>
#include <sys/printk.h>
#include <misc/printk.h>
#include "motors_engine.h"

static struct device *portb;
//static struct device *pwm_2;

void go_forwards()
{
	gpio_pin_write(portb, RIGHT_FORWARDS, 1);
	gpio_pin_write(portb, RIGHT_BACKWARDS, 0);
	gpio_pin_write(portb, LEFT_FORWARDS, 1);
	gpio_pin_write(portb, LEFT_BACKWARDS, 0);
	k_sleep(100);
	drive(stop);
}

void go_backwards()
{
	gpio_pin_write(portb, RIGHT_FORWARDS, 0);
	gpio_pin_write(portb, RIGHT_BACKWARDS, 1);
	gpio_pin_write(portb, LEFT_FORWARDS, 0);
	gpio_pin_write(portb, LEFT_BACKWARDS, 1);
	k_sleep(100);
	drive(stop);
}

void go_rightwards()
{
	gpio_pin_write(portb, RIGHT_FORWARDS, 0);
	gpio_pin_write(portb, RIGHT_BACKWARDS, 1);
	gpio_pin_write(portb, LEFT_FORWARDS, 1);
	gpio_pin_write(portb, LEFT_BACKWARDS, 0);
	k_sleep(100);
	drive(stop);
}

void go_leftwards()
{
	gpio_pin_write(portb, RIGHT_FORWARDS, 1);
	gpio_pin_write(portb, RIGHT_BACKWARDS, 0);
	gpio_pin_write(portb, LEFT_FORWARDS, 0);
	gpio_pin_write(portb, LEFT_BACKWARDS, 1);
	k_sleep(100);
	drive(stop);
}

void brake()
{
	gpio_pin_write(portb, RIGHT_FORWARDS, 0);
	gpio_pin_write(portb, RIGHT_BACKWARDS, 0);
	gpio_pin_write(portb, LEFT_FORWARDS, 0);
	gpio_pin_write(portb, LEFT_BACKWARDS, 0);
}

PRIVATE void init()
{
	portb = device_get_binding(PORT_B);

	gpio_pin_configure(portb, RIGHT_FORWARDS, GPIO_DIR_OUT);
	gpio_pin_configure(portb, RIGHT_BACKWARDS, GPIO_DIR_OUT);
	gpio_pin_configure(portb, LEFT_FORWARDS, GPIO_DIR_OUT);
	gpio_pin_configure(portb, LEFT_BACKWARDS, GPIO_DIR_OUT);
}

void init_motors()
{
	init();
	drive(forwards);
	drive(stop);
}
void drive(dir_t dir)
{
	switch (dir) {
	case forwards:
		go_forwards();
		break;
	case backwards:
		go_backwards();
		break;
	case rightwards:
		go_rightwards();
		break;
	case leftwards:
		go_leftwards();
		break;
	case stop:
		brake();
		break;
	default:
		break;
	}
}
/* in microseconds */
#define MIN_PERIOD (USEC_PER_SEC / 64U)

/* in microseconds */
#define MAX_PERIOD USEC_PER_SEC
// void main(void)
// {
//     init();

//     u32_t max_period;
//     u32_t period;
//     u8_t dir = 0U;

//     printk("PWM demo app-blink LED\n");

//     pwm_2 = device_get_binding(PWM_2pwm_2);
//     if (!pwm_2)
//     {
//         printk("Cannot find %s!\n", PWM_2pwm_2);
//         return;
//     }

//     /* In case the default MAX_PERIOD value cannot be set for some PWM
// 	 * hardware, try to decrease the value until it fits, but no further
// 	 * than to the value of MIN_PERIOD muliplied by four (to allow the
// 	 * sample to actually show some blinking with changing frequency).
// 	 */
//     max_period = MAX_PERIOD;
//     while (pwm_pin_set_usec(pwm_2, PWM_PIN,
//                             max_period, max_period / 2U))
//     {
//         max_period /= 2U;
//         if (max_period < (4U * MIN_PERIOD))
//         {
//             printk("This sample needs to set a period that is "
//                    "not supported by the used PWM driver.");
//             return;
//         }
//     }

//     period = max_period;
//     while (1)
//     {
//         if (pwm_pin_set_usec(pwm_2, PWM_PIN,
//                              period, period / 2U))
//         {
//             printk("pwm pin set fails\n");
//             return;
//         }

//         if (dir)
//         {
//             period *= 2U;

//             if (period > max_period)
//             {
//                 dir = 0U;
//                 period = max_period;
//             }
//         }
//         else
//         {
//             period /= 2U;

//             if (period < MIN_PERIOD)
//             {
//                 dir = 1U;
//                 period = MIN_PERIOD;
//             }
//         }

//         k_sleep(MSEC_PER_SEC * 4U);
//         // while (1)
//         // {
//         //     // drive(forwards);
//         //     // k_sleep(1000);
//         //     // drive(stop);
//         //     // // drive(backwards);
//         //     // k_sleep(2000);
//         //     // drive(stop);
//         // }
//     }
// }
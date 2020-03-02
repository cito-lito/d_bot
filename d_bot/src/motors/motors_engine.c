/**
 *  Author: Mario M.
 * 
 * @brief This program enable the communication with the DC motors
 *  and let us set the drive directions
 */

#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <drivers/pwm.h>
#include <sys/printk.h>
#include <misc/printk.h>
#include "motors_engine.h"
#include "d_bot_util.h"

/*this defines are not in header because they are only use here*/

/*gpio*/
#define PORT_B "GPIOB" /*port B*/
#define RIGHT_FORWARDS 1 /*PB_1*/
#define LEFT_FORWARDS 14 /*PB_14*/
#define RIGHT_BACKWARDS 15 /*PB_15*/
#define LEFT_BACKWARDS 13 /*PB_13*/
/*pwm*/
#define PWM_DRIVER "PWM_2" /*pwm in Timer 2*/
#define CH1_PWM 1 /*PA_0*/
#define PERIOD 20000
#define FAST_AND_FURIUS 20000
#define NORMAL_SPEED 7000

/*device drivers*/
PRIVATE struct device *portb;
PRIVATE struct device *pwm_2;

/******************Privates Functions*****************/

PRIVATE s8_t init();
PRIVATE s8_t drive_forwards();
PRIVATE s8_t drive_backwards();
PRIVATE s8_t drive_rightwards();
PRIVATE s8_t drive_leftwards();
PRIVATE s8_t drive_brake();

/**
 * @brief Init configuration.
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE s8_t init()
{
	portb = device_get_binding(PORT_B);
	if (!portb) {
		printk("Cannot find %s!\n", PORT_B);
		return E_FAIL;
	}
	pwm_2 = device_get_binding(PWM_DRIVER);
	if (!pwm_2) {
		printk("Cannot find %s!\n", PWM_DRIVER);
		return E_FAIL;
	}
	s8_t ret = E_OK; /*no error*/
	ret |= gpio_pin_configure(portb, RIGHT_FORWARDS, GPIO_DIR_OUT);
	ret |= gpio_pin_configure(portb, RIGHT_BACKWARDS, GPIO_DIR_OUT);
	ret |= gpio_pin_configure(portb, LEFT_FORWARDS, GPIO_DIR_OUT);
	ret |= gpio_pin_configure(portb, LEFT_BACKWARDS, GPIO_DIR_OUT);
	return ret;
}

/**
 * @brief Drive forwards.
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE s8_t drive_forwards()
{
	s8_t ret = E_OK; /*no error*/
	ret |= gpio_pin_write(portb, RIGHT_FORWARDS, HIGH);
	ret |= gpio_pin_write(portb, RIGHT_BACKWARDS, LOW);
	ret |= gpio_pin_write(portb, LEFT_FORWARDS, HIGH);
	ret |= gpio_pin_write(portb, LEFT_BACKWARDS, LOW);
	return ret;
}

/**
 * @brief Drive backwards.
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE s8_t drive_backwards()
{
	s8_t ret = E_OK; /*no error*/
	ret |= gpio_pin_write(portb, RIGHT_FORWARDS, LOW);
	ret |= gpio_pin_write(portb, RIGHT_BACKWARDS, HIGH);
	ret |= gpio_pin_write(portb, LEFT_FORWARDS, LOW);
	ret |= gpio_pin_write(portb, LEFT_BACKWARDS, HIGH);
	return ret;
}

/**
 * @brief Drive rightwards.
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE s8_t drive_rightwards()
{
	s8_t ret = E_OK; /*no error*/
	ret |= gpio_pin_write(portb, RIGHT_FORWARDS, LOW);
	ret |= gpio_pin_write(portb, RIGHT_BACKWARDS, HIGH);
	ret |= gpio_pin_write(portb, LEFT_FORWARDS, HIGH);
	ret |= gpio_pin_write(portb, LEFT_BACKWARDS, LOW);
	return ret;
}

/**
 * @brief Drive leftwards.
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE s8_t drive_leftwards()
{
	s8_t ret = E_OK; /*no error*/
	ret |= gpio_pin_write(portb, RIGHT_FORWARDS, HIGH);
	ret |= gpio_pin_write(portb, RIGHT_BACKWARDS, LOW);
	ret |= gpio_pin_write(portb, LEFT_FORWARDS, LOW);
	ret |= gpio_pin_write(portb, LEFT_BACKWARDS, HIGH);
	return ret;
}

/**
 * @brief Decelerate and brake.
 * @return 0 if successful, not 0 on failure.
 */
PRIVATE s8_t drive_brake()
{
	u32_t period = PERIOD;
	u32_t pulse = NORMAL_SPEED;
	do {
		pulse -= 700;
		if (pwm_pin_set_usec(pwm_2, CH1_PWM, period, pulse)) {
			printk("pwm pin set fails\n");
			return E_FAIL;
		}
		k_sleep(10);

	} while (pulse > 1300);
	if (pwm_pin_set_usec(pwm_2, CH1_PWM, period, 0)) {
		printk("pwm pin set fails\n");
		return E_FAIL;
	}

	s8_t ret = 0; /*no error*/
	ret |= gpio_pin_write(portb, RIGHT_FORWARDS, LOW);
	ret |= gpio_pin_write(portb, RIGHT_BACKWARDS, LOW);
	ret |= gpio_pin_write(portb, LEFT_FORWARDS, LOW);
	ret |= gpio_pin_write(portb, LEFT_BACKWARDS, LOW);

	speed(normal);
	return ret;
}

/****************Public Fucntions*********************/

s8_t motors_init()
{
	return init();
}

s8_t speed(speed_t speed)
{
	switch (speed) {
	case fast:
		if (pwm_pin_set_usec(pwm_2, CH1_PWM, PERIOD, FAST_AND_FURIUS)) {
			printk("pwm pin set fails\n");
			return E_FAIL;
		}
		break;
	case normal:
		if (pwm_pin_set_usec(pwm_2, CH1_PWM, PERIOD, NORMAL_SPEED)) {
			printk("pwm pin set fails\n");
			return E_FAIL;
		}
	}
	return E_OK; /*no error*/
}

s8_t drive(direction_t dir)
{
	switch (dir) {
	case forwards:
		if (drive_forwards()) {
			printk("drive error\n");
			return E_FAIL;
		}
		break;
	case backwards:
		if (drive_backwards()) {
			printk("drive error\n");
			return E_FAIL;
		}
		break;
	case rightwards:
		if (drive_rightwards()) {
			printk("drive error\n");
			return E_FAIL;
		}
		break;
	case leftwards:
		if (drive_leftwards()) {
			printk("drive error\n");
			return E_FAIL;
		}
		break;
	case stop:
		if (drive_brake()) {
			printk("drive error\n");
			return E_FAIL;
		}
	}
	return E_OK; /*no error*/
}

// void main(void)
// {
// 	printk("init\n");
// 	motors_init();

// 	speed(normal);
// 	drive_forwards();
// 	k_sleep(3000);
// 	drive(stop);
// }
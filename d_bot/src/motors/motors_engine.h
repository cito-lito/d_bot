

#ifndef MOTORS_ENGINE_H
#define MOTORS_ENGINE_H
#define PORT_B "GPIOB" /*port B*/

#define PRIVATE static

#define RIGHT_FORWARDS 1 /*PB1*/
#define LEFT_FORWARDS 14 /*PB14*/
#define RIGHT_BACKWARDS 15 /*PB15*/
#define LEFT_BACKWARDS 13 /*PB13*/
/*pwm*/
#define PWM_PIN 2
//#define PWM_2pwm_2 "PWM_2"

typedef enum direction {
	forwards = 0,
	backwards,
	rightwards,
	leftwards,
	stop
} dir_t;

void init_motors();

void drive(dir_t dir);

#endif /*MOTORS_ENGINE_H*/
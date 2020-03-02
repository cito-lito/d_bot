/**
 * @file
 * Author : Mario M.
 * 
 * @brief Public API for the behavior functions
 */

#ifndef BEHAVIOR_H
#define BEHAVIOR_H
#include <zephyr.h>


/* Set the size of the Map (n*k matrix)*/
#define N 3
#define K 3

extern u8_t blue_flag;
extern u8_t node_counter;

/**
 * @brief follow a line forwards
 */
void follow_line();

/**
 * @brief drive aprox 90 degree to the right/until ir sensor left 
 * returns true.
 */
void turn_right();

/**
 * @brief after counting a blue node, wait until the robot 
 * leave the node in order to not counting the node more than once
 */
void wait_color_change();

/**
 * @brief Drive forwards until the node has been found.
 * If an Object is detected, drives stops.
 * @param n the node where to go
 */
void find_node(u8_t n);

#endif /*BEHAVIOR_H*/
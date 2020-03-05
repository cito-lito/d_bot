/**
 * @file
 * Author : Mario M.
 * 
 * @brief
 */

#ifndef D_BOT_UTIL_H
#define D_BOT_UTIL_H


#define HIGH 1 /*logic hihg ~2.5V to ~5V*/
#define LOW 0  /*logic low ~0V to ~1V*/

#define PRIVATE static
typedef enum { FORWARDS, LEFTWARDS, RIGHTWARDS } direction_flag_t;
extern direction_flag_t dir_t;

/*error handling*/
enum error { E_OK = 0, E_FAIL };

#endif /*D_BOT_UTIL_H*/
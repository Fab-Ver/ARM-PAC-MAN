#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdint.h>

/** 
 * Graphical constants
*/
#define SQUARE_SIZE 10
#define INITIAL_Y 50
#define INITIAL_X 5 
#define LIVES_X 65
#define LIVES_Y 280
#define LIVES_X_DIST 15
#define GAME_OVER_X 85
#define GAME_OVER_Y 190
#define VICTORY_X 90
#define VICTORY_Y 190
#define PAUSE_X 100
#define PAUSE_Y 190
#define SCORE_X 180
#define SCORE_Y 30

/**
 * Pills configuration 
 */
#define MAX_PILLS 240
#define STD_PILLS 234
#define POWER_PILLS 6
#define STD_SCORE 10
#define POWER_SCORE 50

/**
 * Variables used for random seed generation
 */
#define MULTIPLIER 1664525
#define INCREMENT 1013904223
#define MODULUS 0xFFFFFFFF

/** 
 * Map dimensions
*/
#define COLUMNS  23
#define ROWS 25

#define PAC_MAN_INITIAL_X 11
#define PAC_MAN_INITIAL_Y 18

#define TIMER0_PRIORITY   1  // Highest priority for game countdown
#define TIMER1_PRIORITY   3  // Random number generation (initial seed, lower priority)
#define TIMER2_PRIORITY   2  // Game update logic
#define RIT_PRIORITY      0  // Joystick sampling (highest priority)

#endif
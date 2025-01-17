#ifndef __SHARED_H
#define __SHARED_H

#include "config.h"
#include <stdbool.h>

extern volatile uint16_t score;
extern volatile uint16_t prev_score;
extern volatile uint8_t countdown;
extern volatile uint8_t prev_countdown;
extern volatile uint8_t lives; 
extern volatile uint8_t prev_lives;
extern volatile power_pill_active;

typedef enum {PLAYING, START, PAUSE, GAME_OVER, VICTORY} game_state;
extern volatile game_state current_game_state;

typedef enum {UP, DOWN, LEFT, RIGHT, NONE} joystick_position;
extern volatile joystick_position curr_joystick_position; 

extern volatile uint8_t map[ROWS][COLUMNS];
extern uint8_t square[SQUARE_SIZE][SQUARE_SIZE];
extern uint8_t pill_circle[SQUARE_SIZE][SQUARE_SIZE];
extern uint8_t pacman_circle[SQUARE_SIZE][SQUARE_SIZE];

#endif
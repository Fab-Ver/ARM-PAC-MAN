#ifndef __SHARED_H
#define __SHARED_H

#include "config.h"
#include <stdbool.h>

extern volatile uint8_t lives;
extern volatile uint8_t prev_lives;
extern volatile uint16_t current_score;
extern volatile uint16_t prev_score;

extern volatile uint8_t pac_man_x; 
extern volatile uint8_t pac_man_y;
extern volatile uint8_t prev_pac_man_x; 
extern volatile uint8_t prev_pac_man_y;

typedef enum {PLAYING, START, PAUSE, GAME_OVER, VICTORY} game_state;
extern volatile game_state current_game_state;

typedef enum {UP, DOWN, LEFT, RIGHT, NONE} joystick_position;
extern volatile joystick_position curr_joystick_position; 


extern uint8_t map[ROWS][COLUMNS];
extern uint8_t square[SQUARE_SIZE][SQUARE_SIZE];
extern uint8_t pills[ROWS][COLUMNS];
extern uint8_t pill_circle[SQUARE_SIZE][SQUARE_SIZE];
extern uint8_t pac_man_circle[SQUARE_SIZE][SQUARE_SIZE];

#endif
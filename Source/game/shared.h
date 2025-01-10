#ifndef __SHARED_H
#define __SHARED_H

#include "config.h"
#include <stdbool.h>

extern volatile uint16_t current_score;
extern volatile uint16_t prev_score;

typedef struct {
    uint8_t x; 
    uint8_t y; 
		uint8_t prev_x; 
    uint8_t prev_y; 
	  uint8_t lives; 
		uint8_t prev_lives; 
} PacMan;

extern volatile PacMan pac_man;

typedef enum {CHASE, FRIGHTENED} GhostState;

typedef struct {
    uint8_t x; 
    uint8_t y; 
		uint8_t prev_x; 
    uint8_t prev_y; 
    GhostState state; 
    bool isAlive; 
} Ghost;

extern volatile Ghost blinky;

extern bool power_pill_active;

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
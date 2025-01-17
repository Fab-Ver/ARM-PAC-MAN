#ifndef __PACMAN_H 
#define __PACMAN_H 

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "game/shared.h"
#include "game/config.h"

extern volatile int8_t pac_man_x;
extern volatile int8_t pac_man_y;

/**
 * Draws PAC-MAN current position
 */
void draw_pac_man();

/**
 * Clears PAC-MAN previous position
 */
void clear_pac_man();

/**
 * Moves pac-man, if any joystick position is available   
 */
void move_pac_man(joystick_position curr_joystick_position);

/**
 * Update pac man position  
 */
void update_pac_man_position(int8_t newX, int8_t newY);

#endif 
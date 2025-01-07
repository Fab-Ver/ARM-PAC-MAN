#ifndef __GAME_H 
#define __GAME_H 

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "config.h"
#include "shared.h"

typedef struct{
	uint8_t x;
	uint8_t y;
}  Position ;

/**
 * Initialize the game
 */
void init_game();

/**
 * Draws game map 
 */
void draw_map();

/**
 * Draws standard pills on the map
 */
void draw_pills();

/**
 * Draws standard pills on the map
 */
void generate_power_pills();

/**
 * Draws PAC-MAN current position
 */
void draw_pac_man(uint8_t x_new, uint8_t y_new, uint8_t x_old, uint8_t y_old);

/**
 * Draws current lives 
 */
void draw_lives();

/**
 * Set VICTORY state 
 */
void victory();

/**
 * Set GAME-OVER state 
 */
void game_over();

/**
 * Set PAUSE state 
 */
void pause();

/**
 * Starts the game (unpause)  
 */
void start();

/**
 * Moves pac-man, if any joystick position is available   
 */
void move(joystick_position curr_joystick_position);

/**
 * Checks if a movement leads to a collision with the map  
 */
bool check_map_collision(uint8_t x, uint8_t y);

/**
 * Checks if pac-man can teleport left or right 
 */
bool check_teleport(uint8_t x, uint8_t y);

/**
 * Update pac-man position according to the specified new coordinates.  
 */
void update_pac_man_position(uint8_t new_x, uint8_t new_y);

/**
 * Update the score, if pac-man eats any pills.  
 */
void update_score(uint8_t new_x, uint8_t new_y);

/**
 * Update the stats graphically
 */
void update_stats();

/**
 * Checks how many pills are left in the map
 */
uint8_t count_remaining_pills();

/**
 * Generates a power pill in a random position between the remaining standard pills
 */
void generate_power_pill();

void enable_interrupts();
void disable_interrupts(); 
uint32_t random_number();

#endif 
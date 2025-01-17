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
void pause_game();

/**
 * Starts the game (unpause)  
 */
void start();

/**
 * Checks if a movement leads to a collision with the map  
 */
bool check_map_collision(uint8_t x, uint8_t y);

/**
 * Checks if pac-man can teleport left or right 
 */
bool check_teleport(uint8_t x, uint8_t y);

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

/**
 * Enable all useful interrupts. 
 */
void enable_interrupts();

/**
 * Disable all useful interrupts. 
 */
void disable_interrupts(); 

/**
 * Generates a random number given the seed (using Linear Congruential Generator). 
 */
uint32_t random_number();

/**
 * Checks if the game is in GAME-OVER or VICTORY state after lives decrement.  
 */
void check_game_status();

/**
 * Draws the figure specified by filter inside a square_size*square_size square. 
 */
void draw_in_square(uint8_t x, uint8_t y, uint8_t square_size, uint16_t color, uint8_t filter[square_size][square_size]);

/**
 * Draws the current countdown 
 */
void draw_countdown();

/*
	Generates a seed for pseudo-random number generation using TIMER 1 and ADC 
*/
uint32_t seed_with_adc_and_timer();
#endif 
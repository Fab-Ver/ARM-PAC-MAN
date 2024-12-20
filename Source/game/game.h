#ifndef __GAME_H 
#define __GAME_H 

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern uint8_t  lives;

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

#endif 
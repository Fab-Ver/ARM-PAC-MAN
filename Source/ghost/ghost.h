#ifndef __GHOST_H 
#define __GHOST_H 

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game/config.h"
#include "game/shared.h"
#include "pacman/pacman.h"

extern volatile int8_t ghost_x;
extern volatile int8_t ghost_y;
extern volatile bool ghost_is_alive; 

typedef enum {CHASE, FRIGHTENED} GhostState;
extern volatile GhostState ghost_state; 

/**
 * Draws ghost current position
 */
void draw_ghost();

/**
 * Clears ghost current position
 */
void clear_ghost();

/**
 * Moves ghost according to ghost state and position of pacman 
 */
void move_ghost();

/**
 * Update ghost position  
 */
void update_ghost_position(int8_t newX, int8_t newY); 

/**
 * Checks if pac-man and ghost are in the same position  
 */
void check_collision();

/**
 * Check if coordinates are one the game bounds 
 */
bool is_valid(int8_t x, int8_t y);

bool is_destination(int8_t x, int8_t y, int8_t destX, int8_t destY);
uint8_t get_distance(int8_t x1, int8_t y1, int8_t x2, int8_t y2);

/**
 * Performs the A* search given start and destination point 
 */
void a_star_search(int8_t startX, int8_t startY, int8_t destX, int8_t destY);

#endif 
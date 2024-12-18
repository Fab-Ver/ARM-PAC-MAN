#ifndef __GAME_MAP_H 
#define __GAME_MAP_H 

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include <stdint.h>

#define BLOCK_SIZE 10
#define INITIAL_Y 50 

extern uint8_t  ROWS;				//Map rows 
extern uint8_t  COLUMNS;		//Map columns
extern uint8_t  Map[];			//Pac-man 2d map

void drawMap(void);			//Draws the PAC-MAN map

#endif 
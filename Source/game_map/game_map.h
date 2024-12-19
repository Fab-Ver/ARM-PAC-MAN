#ifndef __GAME_MAP_H 
#define __GAME_MAP_H 

#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 10
#define INITIAL_Y 50
#define INITIAL_X 5 
#define MAX_PILLS 246
#define NORMAL_PILLS 240
#define POWER_PILLS 6
#define STD_SCORE 10
#define POWER_SCORE 50

extern uint8_t  ROWS;				//Map rows 
extern uint8_t  COLUMNS;		//Map columns
extern uint8_t  Map[];			//Pac-man 2d map
extern uint8_t  PacMan[];			//Pac-man 2d icon
extern uint8_t  Pill[];
extern uint8_t  Pills_Map[];			//Pac-man 2d map
extern uint8_t  current_Pills[25][23];
extern uint8_t  lives;	

typedef struct {
    uint8_t x;
    uint8_t y;
} Position;

void drawMap(void);			//Draws the PAC-MAN map
void drawPacMan(uint8_t x_new, uint8_t y_new, uint8_t x_old, uint8_t y_old); //draws current position of Pac-Man
void drawPills();	//Draws all the pill
void drawLives();

#endif 
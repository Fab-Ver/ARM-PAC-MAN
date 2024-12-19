#ifndef __FUNCT_JOYSTICK_H
#define __FUNCT_JOYSTICK_H

#include <stdint.h>
#include <stdbool.h>
#include "game_map/game_map.h"
#include "RIT/RIT.h"

extern uint8_t  Map[];
extern uint8_t PacMan_X; 
extern uint8_t PacMan_Y;
extern uint8_t  COLUMNS;
extern uint8_t  ROWS;		
extern uint16_t current_score;


void perform_action(uint32_t joystick_state);
bool check_map_collision(uint8_t x, uint8_t y);
bool check_teleport(uint8_t x, uint8_t y);
void update_pac_man_position(uint8_t new_x, uint8_t new_y);
void update_score(uint8_t new_x, uint8_t new_y);
void update_graphic_score(uint16_t curr_score);

#endif
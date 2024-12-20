#ifndef __FUNCT_JOYSTICK_H
#define __FUNCT_JOYSTICK_H

#include <stdint.h>
#include <stdbool.h>
#include "game/game.h"
#include "RIT/RIT.h"
#include "timer/timer.h"

extern uint8_t PacMan_X; 
extern uint8_t PacMan_Y;		
extern uint16_t current_score;
extern uint8_t lives; 

void perform_action(uint32_t joystick_state);
bool check_map_collision(uint8_t x, uint8_t y);
bool check_teleport(uint8_t x, uint8_t y);
void update_pac_man_position(uint8_t new_x, uint8_t new_y);
void update_score(uint8_t new_x, uint8_t new_y);
void update_graphic_score(uint16_t curr_score);

#endif
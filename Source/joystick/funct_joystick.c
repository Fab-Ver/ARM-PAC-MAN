/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "funct_joystick.h"


/*----------------------------------------------------------------------------
  Function that performs the selected movement
 *----------------------------------------------------------------------------*/
void perform_action(uint32_t joystick_state){
	
	switch (joystick_state) {
				uint8_t new_x;
				uint8_t new_y;
        case (1 << 25): //SEL
            //No action
            break;
        case (1 << 26): //DOWN
						new_x = PacMan_X;
						new_y = PacMan_Y + 1;
            if(!check_map_collision(new_x, new_y)){
							update_pac_man_position(new_x, new_y);
							update_score(new_x,  new_y);									
						}
            break;
        case (1 << 27): //LEFT
						if(PacMan_X == 0){
							if(check_teleport(COLUMNS - 1, PacMan_Y)){
								update_pac_man_position(COLUMNS - 1, PacMan_Y);
								update_score(COLUMNS - 1,  PacMan_Y);		
							}
						} else {
							new_x = PacMan_X - 1;
							new_y = PacMan_Y;
							if(!check_map_collision(new_x, new_y)){
								update_pac_man_position(new_x, new_y);
								update_score(new_x,  new_y);		
							}
						}
						
            break;
        case (1 << 28):	//RIGHT
						if(PacMan_X == COLUMNS - 1){
							if(check_teleport(0, PacMan_Y)){
								update_pac_man_position(0, PacMan_Y);
								update_score(0,  PacMan_Y);		
							}
						} else {
							new_x = PacMan_X+1;
							new_y = PacMan_Y;
							if(!check_map_collision(new_x, new_y)){
								update_pac_man_position(new_x, new_y);
								update_score(new_x,  new_y);										
							}
						}
            break;
        case (1 << 29):	//UP
            new_x = PacMan_X;
						new_y = PacMan_Y - 1;
            if(!check_map_collision(new_x, new_y)){
							update_pac_man_position(new_x, new_y);
							update_score(new_x,  new_y);							
						}
            break;
        default:
            //No action
            break;
    }
}


/*----------------------------------------------------------------------------
  Function to check if there is a collision with the map 
 *----------------------------------------------------------------------------*/
bool check_map_collision(uint8_t x, uint8_t y){
	int index = y * COLUMNS + x;
	return Map[index] == 1 ? true : false; 
}

void update_pac_man_position(uint8_t new_x, uint8_t new_y){
	drawPacMan(new_x, new_y, PacMan_X, PacMan_Y);
	PacMan_X = new_x;
	PacMan_Y = new_y; 
}

bool check_teleport(uint8_t x, uint8_t y){
	int index = y * COLUMNS + x;
	return Map[index] == 1 ? false : true; 
}

void update_score(uint8_t new_x, uint8_t new_y){
	if(current_Pills[new_y][new_x] == 1){
		current_score+=STD_SCORE;
		update_graphic_score(current_score);
		current_Pills[new_y][new_x] = 0;
	} else if (current_Pills[new_y][new_x] == 2){
		current_score+=POWER_SCORE;
		update_graphic_score(current_score);
		current_Pills[new_y][new_x] = 0;
	}
	if(current_score == (NORMAL_PILLS*STD_SCORE)+(POWER_PILLS*POWER_SCORE)){
		GUI_Text(85, 190, (uint8_t *) "VICTORY!", Yellow, Blue);
		disable_RIT();
		NVIC_DisableIRQ(EINT1_IRQn);
		LPC_PINCON->PINSEL4    &= ~(1 << 20);     
	}
}

void update_graphic_score(uint16_t curr_score){
	uint8_t str[20];
  snprintf((char *)str, sizeof(str), "%d", curr_score);
	GUI_Text(180, 30, (uint8_t *) "    ", Black, Black);
	GUI_Text(180, 30, (uint8_t *) str, White, Black);
}

#include "pacman.h"

volatile int8_t pac_man_x = PAC_MAN_INITIAL_X; 
volatile int8_t pac_man_y = PAC_MAN_INITIAL_Y; 

void move_pac_man(joystick_position curr_joystick_position){
	switch (curr_joystick_position) {
				int8_t new_x;
				int8_t new_y;
        case NONE: //Double key selection = no movement 
            break;
        case DOWN:
						new_x = pac_man_x;
						new_y = pac_man_y + 1;
            if(!check_map_collision(new_x, new_y)){
							update_pac_man_position(new_x, new_y);
							update_score(new_x, new_y);									
						}
            break;
        case LEFT:
						if(pac_man_x == 0){
							if(check_teleport(COLUMNS - 1, pac_man_y)){
								update_pac_man_position(COLUMNS - 1, pac_man_y);
								update_score(COLUMNS - 1,  pac_man_y);		
							}
						} else {
							new_x = pac_man_x - 1;
							new_y = pac_man_y;
							if(!check_map_collision(new_x, new_y)){
								update_pac_man_position(new_x, new_y);
								update_score(new_x,  new_y);		
							}
						}
            break;
        case RIGHT:
						if(pac_man_x == COLUMNS - 1){
							if(check_teleport(0, pac_man_y)){
								update_pac_man_position(0, pac_man_y);
								update_score(0,  pac_man_y);		
							}
						} else {
							new_x = pac_man_x+1;
							new_y = pac_man_y;
							if(!check_map_collision(new_x, new_y)){
								update_pac_man_position(new_x, new_y);
								update_score(new_x,  new_y);										
							}
						}
            break;
        case UP:	//UP
            new_x = pac_man_x;
						new_y = pac_man_y - 1;
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

void draw_pac_man(){
	uint8_t dx, dy; 
	for (dy = 0; dy < SQUARE_SIZE; dy++) {
   for (dx = 0; dx < SQUARE_SIZE; dx++) {
		 if(pacman_circle[dy][dx] == 1){
					LCD_SetPoint(SQUARE_SIZE * pac_man_x + dx + INITIAL_X, SQUARE_SIZE * pac_man_y + dy + INITIAL_Y, Yellow);
		 }
   }
  }
}

void clear_pac_man(){
	uint8_t dx, dy; 
	for (dy = 0; dy < SQUARE_SIZE; dy++) {
   for (dx = 0; dx < SQUARE_SIZE; dx++) {
		 if(pacman_circle[dy][dx] == 1){
					LCD_SetPoint(SQUARE_SIZE * pac_man_x + dx + INITIAL_X, SQUARE_SIZE * pac_man_y + dy + INITIAL_Y, Black);
		 }
   }
  }
}

void update_pac_man_position(int8_t newX, int8_t newY){
	clear_pac_man();
	pac_man_x = newX;
	pac_man_y = newY; 
	draw_pac_man(); 
}
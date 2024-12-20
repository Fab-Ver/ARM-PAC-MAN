#include "game.h"
#include <stdio.h>

#include "config.h"
#include "shared.h"
#include "stdbool.h"

extern uint32_t seed;

uint32_t random_number();

void draw_in_square(uint8_t x, uint8_t y, uint8_t square_size, uint16_t color, uint8_t filter[square_size][square_size], bool scale, bool deleteOld, uint8_t old_x, uint8_t old_y);

void draw_map(){
	uint8_t x,y;
	LCD_Clear(Black);
	GUI_Text(10, 10, (uint8_t *) "Game Over in", White, Black);
	GUI_Text(180, 10, (uint8_t *) "SCORE", White, Black);
	GUI_Text(10, 305, (uint8_t *) "LIVES:", White, Black);
	for (y = 0; y < ROWS; y++) {
		for (x = 0; x < COLUMNS; x++) { 
			if(map[y][x] == 1){
				draw_in_square(x, y, SQUARE_SIZE, Blue, square, true, false, NULL, NULL);
			} 
    }
  }
}

void draw_pac_man(uint8_t x_new, uint8_t y_new, uint8_t x_old, uint8_t y_old){
	if(x_new != x_old && y_new != y_old){
		return;
	}
	draw_in_square(x_new, y_new, SQUARE_SIZE, Yellow, pac_man_circle, true, true, x_old, y_old);
}

void generate_power_pills(){
	uint8_t count = 0;
  Position pills_position[MAX_PILLS];
	uint8_t xt, yt;
  for (yt = 0; yt < ROWS; yt++) {
   for (xt = 0; xt < COLUMNS; xt++) {
    if (pills[yt][xt] == 1) {
     if (count < MAX_PILLS) {
      pills_position[count].x = xt;
      pills_position[count].y = yt;
      count++;
     }
    }
   }
  }
	int special_pills_count = 0;
    while (special_pills_count < 6) {
        if (special_pills_count < count) {
            int index = random_number() % count;
            int pill_x = pills_position[index].x;
            int pill_y = pills_position[index].y;
            if (pills[pill_y][pill_x] == 1) {
                pills[pill_y][pill_x]++;
                special_pills_count++;
            }
        }
    }
}

void draw_pills(){
		uint8_t x, y;
    for (y = 0; y < ROWS; y++) {
        for (x = 0; x < COLUMNS; x++) {
            if (pills[y][x] == 1 || pills[y][x] == 2) {
								draw_in_square(x, y, SQUARE_SIZE, pills[y][x] == 1 ? Red : Green, pill_circle, true, false, NULL, NULL);
            }
        }
    }
}

void draw_lives(){
	int i;
	uint8_t dx, dy; 
	uint16_t x = 60;
	uint16_t y = 307;
	/*for(i=0; i < lives + 1; i++){
		for (dy = 0; dy < SQUARE_SIZE; dy++) {
			for (dx = 0; dx < SQUARE_SIZE; dx++) {
				int index = dy * SQUARE_SIZE + dx; 
				LCD_SetPoint(x + dx,y + dy, Black);
				if(PacMan[index] == 1 && i < lives){
					LCD_SetPoint( x + dx, y+ dy, Yellow);
				}
			}
		}
		x+=15;
	}*/
}

uint32_t random_number() {
    seed = (MULTIPLIER * seed + INCREMENT) % MODULUS;
    return seed;
}

void draw_in_square(uint8_t x, uint8_t y, uint8_t square_size, uint16_t color, uint8_t filter[square_size][square_size], bool scale, bool deleteOld, uint8_t old_x, uint8_t old_y){
	uint8_t dx, dy; 
	for (dy = 0; dy < square_size; dy++) {
   for (dx = 0; dx < square_size; dx++) {
		 if(deleteOld){
			LCD_SetPoint(square_size * old_x + dx + INITIAL_X, square_size * old_y + dy + INITIAL_Y, Black);
		 }
		 if(filter[dy][dx] == 1){
				if(scale){
					LCD_SetPoint(square_size * x + dx + INITIAL_X, square_size * y + dy + INITIAL_Y, color);
				} else {
					LCD_SetPoint(x + dx, y+ dy, color);
				}
				
		 }
   }
  }
}
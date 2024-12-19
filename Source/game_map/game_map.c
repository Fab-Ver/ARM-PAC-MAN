#include "game_map.h"
#include <stdio.h>

#define MULTIPLIER 1664525
#define INCREMENT 1013904223
#define MODULUS 0xFFFFFFFF

extern uint32_t seed;

uint32_t random_number();

void drawMap(void){
	LCD_Clear(Black);
	GUI_Text(10, 10, (uint8_t *) "Game Over in", White, Black);
	GUI_Text(180, 10, (uint8_t *) "SCORE", White, Black);
	uint8_t x,y;
	GUI_Text(10, 305, (uint8_t *) "LIVES:", White, Black);
		for (y = 0; y < ROWS; y++) {
        for (x = 0; x < COLUMNS; x++) {
					  int index = y * COLUMNS + x; 
            if(Map[index] == 1){
							uint8_t dx, dy; 
							for (dy = 0; dy < BLOCK_SIZE; dy++) {
                    for (dx = 0; dx < BLOCK_SIZE; dx++) {
                        LCD_SetPoint(BLOCK_SIZE * x + dx + INITIAL_X, BLOCK_SIZE * y + dy + INITIAL_Y, Blue);
                    }
              }
						} 
        }
    }
}

void drawPacMan(uint8_t x_new, uint8_t y_new, uint8_t x_old, uint8_t y_old){
	if(x_new != x_old && y_new != y_old){
		return;
	}
	uint8_t dx, dy; 
	for (dy = 0; dy < BLOCK_SIZE; dy++) {
		for (dx = 0; dx < BLOCK_SIZE; dx++) {
			int index = dy * BLOCK_SIZE + dx; 
			LCD_SetPoint(BLOCK_SIZE * x_old + dx + INITIAL_X, BLOCK_SIZE * y_old + dy + INITIAL_Y, Black);
			if(PacMan[index] == 1){
				LCD_SetPoint(BLOCK_SIZE * x_new + dx + INITIAL_X, BLOCK_SIZE * y_new + dy + INITIAL_Y, Yellow);
			}
		}
  }
}

void drawPills(){
		uint8_t xt, yt, dy, dx;
    int count = 0;
    Position pill_positions[MAX_PILLS];
		
    for (yt = 0; yt < ROWS; yt++) {
        for (xt = 0; xt < COLUMNS; xt++) {
            int index = yt * COLUMNS + xt;
            if (Pills_Map[index] == 1) {
                current_Pills[yt][xt] = Pills_Map[index];
                if (count < MAX_PILLS) {
                    pill_positions[count].x = xt;
                    pill_positions[count].y = yt;
                    count++;
                }
                for (dy = 0; dy < BLOCK_SIZE; dy++) {
                    for (dx = 0; dx < BLOCK_SIZE; dx++) {
                        int i = dy * BLOCK_SIZE + dx;
                        if (Pill[i] == 1) {
                            LCD_SetPoint(BLOCK_SIZE * xt + dx + INITIAL_X, BLOCK_SIZE * yt + dy + INITIAL_Y, Red);
                        }
                    }
                }
            }
        }
    }
    
    int special_pills_count = 0;
    while (special_pills_count < 6) {
        if (special_pills_count < count) {
            int index = random_number() % count;
            int pill_x = pill_positions[index].x;
            int pill_y = pill_positions[index].y;
            if (current_Pills[pill_y][pill_x] == 1) {
                current_Pills[pill_y][pill_x]++;
                special_pills_count++;
                for (dy = 0; dy < BLOCK_SIZE; dy++) {
                    for (dx = 0; dx < BLOCK_SIZE; dx++) {
                        int i = dy * BLOCK_SIZE + dx;
                        if (Pill[i] == 1) {
                            LCD_SetPoint(BLOCK_SIZE * pill_x + dx + INITIAL_X, BLOCK_SIZE * pill_y + dy + INITIAL_Y, Green);
                        }
                    }
                }
            }
        }
    }
}

void drawLives(){
	int i;
	uint8_t dx, dy; 
	uint16_t x = 60;
	uint16_t y = 307;
	for(i=0; i < lives + 1; i++){
		for (dy = 0; dy < BLOCK_SIZE; dy++) {
			for (dx = 0; dx < BLOCK_SIZE; dx++) {
				int index = dy * BLOCK_SIZE + dx; 
				LCD_SetPoint(x + dx,y + dy, Black);
				if(PacMan[index] == 1 && i < lives){
					LCD_SetPoint( x + dx, y+ dy, Yellow);
				}
			}
		}
		x+=15;
	}
}

uint32_t random_number() {
    seed = (MULTIPLIER * seed + INCREMENT) % MODULUS;
    return seed;
}
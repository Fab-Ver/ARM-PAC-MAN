#include "game_map.h"

void drawMap(void){
	LCD_Clear(Black);
	GUI_Text(10, 10, (uint8_t *) "Game Over in", White, Black);
	GUI_Text(185, 10, (uint8_t *) "SCORE", White, Black);
	GUI_Text(10, 30, (uint8_t *) "60s", White, Black); // -->	move to other location
	GUI_Text(185, 30, (uint8_t *) "00", White, Black); // --> move to other location
	int x,y;
		for (y = 0; y < ROWS; y++) {
        for (x = 0; x < COLUMNS; x++) {
					  int index = y * COLUMNS + x; 
            if(Map[index] == 1){
							int dx, dy; 
							for (dy = 0; dy < BLOCK_SIZE; dy++) {
                    for (dx = 0; dx < BLOCK_SIZE; dx++) {
                        LCD_SetPoint(BLOCK_SIZE * x + dx, BLOCK_SIZE * y + dy + INITIAL_Y, Blue);
                    }
                }
						} 
        }
    }
}
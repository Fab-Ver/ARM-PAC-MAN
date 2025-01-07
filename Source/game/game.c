#include "game.h"
#include <stdio.h>


#include "stdbool.h"

uint32_t seed;
uint16_t next_life = 0;

/*
	Generates a seed for pseudo-random number generation using TIMER 1 and ADC 
*/
uint32_t seed_with_adc_and_timer();

/**
 * Generates a random number given the seed (using Linear Congruential Generator). 
 */
uint32_t random_number();

/**
 * Draws the figure specified by filter inside a square_size*square_size square. 
 */
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

void generate_power_pill(){
	uint8_t count = 0;
  Position pills_position[MAX_PILLS];
	uint8_t xt, yt;
	uint16_t power_pills_count = 0;
	
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
	
	uint16_t position;
	uint8_t pill_x, pill_y;
	
	do{
		position = random_number() % count;
    pill_x = pills_position[position].x;
    pill_y = pills_position[position].y;
	} while(pills[pill_y][pill_x] != 1);
	
	pills[pill_y][pill_x] = 2;
	draw_in_square(pill_x, pill_y, SQUARE_SIZE, Green, pill_circle, true, false, NULL, NULL);
}

void draw_pills(){
		uint8_t x, y;
    for (y = 0; y < ROWS; y++) {
        for (x = 0; x < COLUMNS; x++) {
            if (pills[y][x] == 1) {
								draw_in_square(x, y, SQUARE_SIZE, Red, pill_circle, true, false, NULL, NULL);
            }
        }
    }
}

void draw_lives(){
	if(prev_lives != lives){
		uint8_t i;
		uint16_t x = LIVES_X;
		uint16_t y = 305;
		uint8_t dx,dy;
		for (i = 0; i < lives + 1; i++) {
        for (dy = 0; dy < SQUARE_SIZE; dy++) {
            for (dx = 0; dx < SQUARE_SIZE; dx++) {
                // Ensure we don't draw out of bounds
                if ((x + dx) < 240 && (y + dy) < 320) {
                    // Default square color
                    LCD_SetPoint(x + dx, y + dy, Black);
                    // Draw pac-man circle if it's part of a life
                    if (pac_man_circle[dy][dx] == 1 && i < lives) {
                        LCD_SetPoint(x + dx, y + dy, Yellow);
                    }
                }
            }
        }
        // Move to the next life position
        x += LIVES_X_DIST;
    }
	}
}

void init_game(){
	draw_map();
	draw_lives();
	draw_pac_man(pac_man_x, pac_man_y, prev_pac_man_x, prev_pac_man_y);
	update_stats();
	seed = seed_with_adc_and_timer(); 
	draw_pills();
}

void victory(){
	GUI_Text(VICTORY_X, VICTORY_Y, (uint8_t *) "VICTORY!", Yellow, Blue);
	disable_RIT();
	disable_timer(0);
	NVIC_DisableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 20);
}

void game_over(){
	GUI_Text(GAME_OVER_X, GAME_OVER_Y, (uint8_t *) "GAME OVER", Yellow, Blue);
	disable_RIT();
	disable_timer(0);
	NVIC_DisableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 20);
}

void pause(){
	GUI_Text(PAUSE_X, PAUSE_Y, (uint8_t *) "PAUSE", Yellow, Blue);
	disable_RIT();
	disable_timer(0);
	reset_timer(0);
	reset_RIT();
}

void start(){
	GUI_Text(PAUSE_X, PAUSE_Y, (uint8_t *) "     ", Yellow, Blue);
	enable_RIT();
	enable_timer(0);
}

void move(joystick_position curr_joystick_position){
	
	switch (curr_joystick_position) {
				uint8_t new_x;
				uint8_t new_y;
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
							if(check_teleport(0, pac_man_x)){
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

bool check_map_collision(uint8_t x, uint8_t y){
	return map[y][x] == 1 ? true : false; 
}

void update_pac_man_position(uint8_t new_x, uint8_t new_y){
	prev_pac_man_x = pac_man_x;
	prev_pac_man_y = pac_man_y; 
	pac_man_x = new_x;
	pac_man_y = new_y; 
}

bool check_teleport(uint8_t x, uint8_t y){
	return map[y][x] == 1 ? false : true; 
}

void update_score(uint8_t new_x, uint8_t new_y){
	if(pills[new_y][new_x] == 1){
		prev_score = current_score;
		current_score+=STD_SCORE;
		next_life+=STD_SCORE;
		pills[new_y][new_x] = 0;
	} else if (pills[new_y][new_x] == 2){
		prev_score = current_score;
		current_score+=POWER_SCORE;
		next_life+=POWER_SCORE;
		pills[new_y][new_x] = 0;
	}
	if(next_life >= 1000){
		prev_lives = lives;
		lives++;
		next_life-=1000;
	}
	if(count_remaining_pills() == 0){
		current_game_state = VICTORY;     
	}
}

void update_stats(){
	if(prev_score != current_score){
		uint8_t str[20];
		snprintf((char *)str, sizeof(str), "%d", current_score);
		GUI_Text(SCORE_X, SCORE_Y, (uint8_t *) "    ", Black, Black);
		GUI_Text(SCORE_X, SCORE_Y, (uint8_t *) str, White, Black);
	}
}

uint32_t random_number() {
    seed = (MULTIPLIER * seed + INCREMENT) % MODULUS;
    return seed;
}

uint32_t seed_with_adc_and_timer() {
    // Initialize ADC (assuming it's on channel 0, ADC0)
    LPC_SC->PCONP |= (1 << 12); // Power up ADC
    LPC_ADC->ADCR = (1 << 0) | (4 << 8) | (1 << 21); // Select AD0.0, clock div, enable ADC

    // Start ADC conversion
    LPC_ADC->ADCR |= (1 << 24);
    while (!(LPC_ADC->ADSTAT & (1 << 0))); // Wait for conversion
    uint32_t adc_value = (LPC_ADC->ADDR0 >> 4) & 0xFFF; // Read ADC value

    // Use the timer value
    uint32_t timer_value = LPC_TIM1->TC;

    // Combine ADC and timer values as the seed
    return adc_value ^ timer_value;
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
					LCD_SetPoint(x + dx, y + dy, color);
				}
		 }
   }
  }
}

uint8_t count_remaining_pills(){
	uint8_t x, y;
	uint8_t remaining = 0;
  for (y = 0; y < ROWS; y++) {
   for (x = 0; x < COLUMNS; x++) {
    if (pills[y][x] == 1 || pills[y][x] == 2) {
      remaining++;
    }
   }
  }
	return remaining;
}

void disable_interrupts() {
    NVIC_DisableIRQ(TIMER2_IRQn);
    NVIC_DisableIRQ(TIMER0_IRQn);
    NVIC_DisableIRQ(EINT0_IRQn);
}

void enable_interrupts() {
    NVIC_EnableIRQ(TIMER2_IRQn);
    NVIC_EnableIRQ(TIMER0_IRQn);
    NVIC_EnableIRQ(EINT0_IRQn);
}
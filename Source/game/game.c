#include "game.h"
#include <stdio.h>
#include "stdbool.h"
#include "ghost/ghost.h"
#include "pacman/pacman.h"
#include "CAN/CAN.h"

uint32_t seed;
uint16_t next_life = 0;

void draw_map(){
	uint8_t x,y;
	LCD_Clear(Black);
	GUI_Text(10, 10, (uint8_t *) "Game Over in", White, Black);
	GUI_Text(180, 10, (uint8_t *) "SCORE", White, Black);
	GUI_Text(10, 305, (uint8_t *) "LIVES:", White, Black);
	for (y = 0; y < ROWS; y++) {
		for (x = 0; x < COLUMNS; x++) { 
			if(map[y][x] == 1){
				draw_in_square(x, y, SQUARE_SIZE, Blue, square);
			} else if (map[y][x] == 2){
				draw_in_square(x, y, SQUARE_SIZE, Magenta, pill_circle);
			}
    }
  }
}

void generate_power_pill(){
	uint8_t count = 0;
  Position pills_position[MAX_PILLS];
	uint8_t xt, yt;
	uint16_t power_pills_count = 0;
	
  for (yt = 0; yt < ROWS; yt++) {
   for (xt = 0; xt < COLUMNS; xt++) {
    if (map[yt][xt] == 2) {
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
	} while(map[pill_y][pill_x] != 2);
	
	disable_interrupts();
	map[pill_y][pill_x] = 3;
	enable_interrupts(); 
	draw_in_square(pill_x, pill_y, SQUARE_SIZE, Green, pill_circle);
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
								LCD_SetPoint(x + dx, y + dy, Black);
								if (pacman_circle[dy][dx] == 1 && i < lives) {
									LCD_SetPoint(x + dx, y + dy, Yellow);
								}
            }
        }
        x += LIVES_X_DIST;
    }
	}
}

void init_game(){
	draw_map();
	draw_lives();
	draw_pac_man();
	draw_ghost();
	update_stats();
	seed = seed_with_adc_and_timer(); 
}

void victory(){
	GUI_Text(VICTORY_X, VICTORY_Y, (uint8_t *) "VICTORY!", Yellow, Blue);
	disable_RIT();
	disable_timer(0);
	NVIC_DisableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 20);
}

void game_over(){
	draw_lives();
	GUI_Text(GAME_OVER_X, GAME_OVER_Y, (uint8_t *) "GAME OVER", Yellow, Blue);
	disable_RIT();
	disable_timer(0);
	NVIC_DisableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 20);
}

void pause_game(){
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

bool check_map_collision(uint8_t x, uint8_t y){
	return map[y][x] == 1 ? true : false; 
}

bool check_teleport(uint8_t x, uint8_t y){
	return map[y][x] == 1 ? false : true; 
}

void update_score(uint8_t new_x, uint8_t new_y){
	if(map[new_y][new_x] == 2){
		disable_interrupts();
		//prev_score = score;
		score += (uint16_t) STD_SCORE;
		next_life+=STD_SCORE;
		map[new_y][new_x] = 0;
		enable_interrupts();
	} else if (map[new_y][new_x] == 3){
		disable_interrupts();
		//prev_score = score;
		score += (uint16_t) POWER_SCORE;
		next_life+=POWER_SCORE;
		map[new_y][new_x] = 0;
		power_pill_active = true;
    ghost_state = FRIGHTENED;
		enable_interrupts();
	}
	if(next_life >= 1000){
		disable_interrupts();
		//prev_lives = lives;
		lives++;
		next_life-=1000;
		enable_interrupts();
	}
	if(count_remaining_pills() == 0){
		disable_interrupts();
		current_game_state = VICTORY;     
		enable_interrupts(); 
	}
}

void update_stats(){
	if(prev_score != score){
		char str_stats[6];
		sprintf(str_stats, "%d", score);
		GUI_Text(SCORE_X, SCORE_Y, (uint8_t *) "     ", Black, Black);
		GUI_Text(SCORE_X, SCORE_Y, (uint8_t *) str_stats, White, Black);
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

void draw_in_square(uint8_t x, uint8_t y, uint8_t square_size, uint16_t color, uint8_t filter[square_size][square_size]){
	uint8_t dx, dy; 
	for (dy = 0; dy < square_size; dy++) {
   for (dx = 0; dx < square_size; dx++) {
		 if(filter[dy][dx] == 1){
				LCD_SetPoint(square_size * x + dx + INITIAL_X, square_size * y + dy + INITIAL_Y, color);
		 }
   }
  }
}

uint8_t count_remaining_pills(){
	uint8_t x, y;
	uint8_t remaining = 0;
	disable_interrupts();
  for (y = 0; y < ROWS; y++) {
   for (x = 0; x < COLUMNS; x++) {
    if (map[y][x] == 2 || map[y][x] == 3) {
      remaining++;
    }
   }
  }
	enable_interrupts(); 
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

void draw_countdown(){
	char str[6] = "     ";
	sprintf((char *)str, "%d", countdown);
  GUI_Text(10, 30, (uint8_t *) "     ", Black, Black);
	GUI_Text(10, 30, (uint8_t *) strcat(str,"s"), White, Black);
}

void check_game_status(){
	disable_interrupts(); 
	if(lives == 0){
		if(count_remaining_pills() == 0){
			current_game_state = VICTORY;      
		} else {
			current_game_state = GAME_OVER; 
		}
	}
	enable_interrupts();
}

void CAN_update_stats(uint8_t countdown, uint8_t lives, uint16_t score){
		CAN_TxMsg.data[0] = (char) score;
		CAN_TxMsg.data[1] = (char) (score & 0xFF00) >> 8;
		CAN_TxMsg.data[2] = (char)lives;
		CAN_TxMsg.data[3] = (char) countdown;
		CAN_TxMsg.len = 4;
		CAN_TxMsg.id = 2;
		CAN_TxMsg.format = STANDARD_FORMAT;
		CAN_TxMsg.type = DATA_FRAME;
		CAN_wrMsg(1, &CAN_TxMsg);
}

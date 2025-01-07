/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "../GLCD/GLCD.h" 
#include <stdio.h> /*for sprintf*/
#include <string.h>
#include "game/game.h"
#include "game/config.h"
#include "game/shared.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint8_t second = 60;
uint16_t elapsed_time = 0;			// Tracks total elapsed seconds
uint8_t power_pill_count = 0;      // Counter for power pills
uint16_t next_pill_time = 0;        // Time for the next power pill generation         

void TIMER0_IRQHandler (void)
{
	second--;
	elapsed_time++;
	
	char str[20];
  snprintf((char *)str, sizeof(str), "%d", second);
	GUI_Text(10, 30, (uint8_t *) "   ", Black, Black);
	GUI_Text(10, 30, (uint8_t *) strcat(str, "s"), White, Black);
	
	disable_interrupts();
	update_stats();
	draw_lives();
	enable_interrupts();
	
	if (elapsed_time >= next_pill_time && power_pill_count < POWER_PILLS) {
		disable_interrupts();
        generate_power_pill(); // Generate a power pill
		enable_interrupts();
		power_pill_count++;
        next_pill_time = elapsed_time + (random_number() % 10 + 1); // Random interval: 1-10 seconds
  }
	
	if(second == 0){
		disable_interrupts();
		prev_lives = lives;
		lives--;
		enable_interrupts();
		second = 60;
		if(lives == 0){
			if(count_remaining_pills() == 0){
				current_game_state = VICTORY;      
			} else {
				current_game_state = GAME_OVER; 
			}
		}
	}

  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler (void)
{
	uint8_t local_lives; 
	game_state local_current_game_state;
	joystick_position local_curr_joystick_position;
	uint8_t local_pac_man_x; 
	uint8_t local_pac_man_y; 
	uint8_t local_prev_pac_man_x; 
	uint8_t local_prev_pac_man_y; 
	uint8_t local_current_score;  
	
	disable_interrupts();
	local_lives = lives; 
	local_current_game_state = current_game_state; 
	local_curr_joystick_position = curr_joystick_position; 
	enable_interrupts();
		
	
	switch(local_current_game_state){
			case GAME_OVER:
				game_over();
				break;
			case VICTORY: 
				victory();
				break;
			case START:
				start();
				disable_interrupts();
				current_game_state = PLAYING; 
				enable_interrupts();
				break;
			case PAUSE:
				pause();
				break;
			case PLAYING:
				disable_interrupts();
				move(local_curr_joystick_position);
				local_pac_man_x = pac_man_x; 
				local_pac_man_y = pac_man_y; 
				local_prev_pac_man_x = prev_pac_man_x; 
				local_prev_pac_man_y = prev_pac_man_y; 
				local_current_score = current_score; 
				enable_interrupts();
				draw_pac_man(local_pac_man_x, local_pac_man_y, local_prev_pac_man_x, local_prev_pac_man_y);
				break;
		}
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

void TIMER3_IRQHandler (void)
{
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

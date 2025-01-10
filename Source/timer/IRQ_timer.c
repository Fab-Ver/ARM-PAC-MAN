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

bool local_power_pill_active;
bool local_blinky_alive; 
uint8_t power_pill_timer = 0;
uint8_t blinky_respawn_timer = 0;

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
	local_power_pill_active = power_pill_active;
	local_blinky_alive = blinky.isAlive;
	enable_interrupts();
	
	if (elapsed_time >= next_pill_time && power_pill_count < POWER_PILLS) {
		disable_interrupts();
    generate_power_pill(); // Generate a power pill
		enable_interrupts();
		power_pill_count++;
    next_pill_time = elapsed_time + (random_number() % 10 + 1); // Random interval: 1-10 seconds
  }
	
	if(local_power_pill_active){
		power_pill_timer++;
		if(power_pill_timer > POWER_PILL_DURATION){
			disable_interrupts();
			power_pill_active = false;
			power_pill_timer = 0;
			blinky.state = CHASE;
			enable_interrupts();
		}
	}
	
	if(!local_blinky_alive){
		blinky_respawn_timer++;
		if(blinky_respawn_timer > GHOST_RESPAWN_TIME){
			disable_interrupts();
			blinky.isAlive = true;
			blinky.x = BLINKY_INITIAL_X;
			blinky.y = BLINKY_INITIAL_Y;
			blinky.prev_x = 10;
			blinky.prev_y = 11;
			blinky.state = CHASE;
			blinky_respawn_timer = 0;
			enable_interrupts();
		}
	}
	
	if(second == 0){
		disable_interrupts();
		pac_man.prev_lives = pac_man.lives;
		pac_man.lives--;
		second = 60;
		check_game_status();
		enable_interrupts();
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
	
	disable_interrupts();
	local_current_game_state = current_game_state; 
	local_curr_joystick_position = curr_joystick_position; 
	local_blinky_alive = blinky.isAlive;
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
				move_pac_man(local_curr_joystick_position);
				if(local_blinky_alive){
					move_ghost();
					disable_interrupts();
					draw_blinky(blinky.x, blinky.y, blinky.prev_x, blinky.prev_y);
					enable_interrupts();
				}
				disable_interrupts();
				draw_pac_man(pac_man.x, pac_man.y, pac_man.prev_x, pac_man.prev_y);
				enable_interrupts();
				check_collision();
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

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
#include "ghost/ghost.h"
#include "pacman/pacman.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint16_t elapsed_time = 0;			// Tracks total elapsed seconds
volatile uint16_t next_pill_time = 0;        // Time for the next power pill generation  

volatile uint8_t power_pill_timer = 0;
volatile uint8_t ghost_respawn_timer = 0;

volatile uint8_t ghost_speed = GHOST_INITIAL_SPEED;
volatile uint8_t cycle_counter = 0; 
volatile uint16_t freq_counter = 0; 

void TIMER0_IRQHandler (void)
{
	static uint8_t power_pill_generated = 0;  
	countdown--;
	elapsed_time++;
	
	disable_interrupts();
	draw_lives();
	draw_countdown();
	update_stats();
	enable_interrupts();
	
	if (elapsed_time >= next_pill_time && power_pill_generated < POWER_PILLS) {
		disable_interrupts();
    generate_power_pill(); // Generate a power pill
		power_pill_generated++;
		enable_interrupts();
		next_pill_time = elapsed_time + (random_number() % 10 + 1); // Random interval: 1-10 seconds
  }
	
	if(power_pill_active){
		power_pill_timer++;
		if(power_pill_timer > POWER_PILL_DURATION){
			disable_interrupts();
			power_pill_active = false;
			power_pill_timer = 0;
			ghost_state = CHASE;
			enable_interrupts();
		}
	}
	
	if(!ghost_is_alive){
		ghost_respawn_timer++;
		if(ghost_respawn_timer > GHOST_RESPAWN_TIME){
			disable_interrupts();
			ghost_is_alive = true;
			ghost_x = GHOST_INITIAL_X;
			ghost_y = GHOST_INITIAL_Y;
			draw_ghost();
			ghost_state = CHASE;
			ghost_respawn_timer = 0;
			enable_interrupts();
		}
	}
	
	if(countdown == 0){
		disable_interrupts();
		prev_lives = lives;
		lives--;
		countdown = 60;
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
	switch(current_game_state){
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
				pause_game();
				break;
			case PLAYING:
				cycle_counter++;
				
				move_pac_man(curr_joystick_position);
				if(ghost_is_alive && cycle_counter >= ghost_speed){
					freq_counter++;
					move_ghost();
					cycle_counter = 0; 
					if(freq_counter >= SPEED_INCREASE_FREQ){
						ghost_speed = ghost_speed == 0 ? ((uint8_t) GHOST_INITIAL_SPEED / 3 ) : (ghost_speed - 1); 
					}
				}
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

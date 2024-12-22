#include "button.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include <stdbool.h>
#include "RIT/RIT.h"
#include "timer/timer.h"

#include "game/shared.h"
#include "game/game.h"

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	if(current_game_state == PAUSE){
		disable_interrupts();
		current_game_state = START;
		enable_interrupts();
	} else {
		disable_interrupts();
		current_game_state = PAUSE;
		enable_interrupts();
	}
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}



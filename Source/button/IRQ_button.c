#include "button.h"
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include <stdbool.h>
#include "RIT/RIT.h"
#include "timer/timer.h"

extern bool pause; 

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	if(pause == true){
		GUI_Text(100, 190, (uint8_t *) "     ", Yellow, Blue);
		enable_RIT();
		enable_timer(0);
		pause = false; 
	} else {
		GUI_Text(100, 190, (uint8_t *) "PAUSE", Yellow, Blue);
		disable_RIT();
		disable_timer(0);
		reset_timer(0);
		reset_RIT();
		pause = true; 
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



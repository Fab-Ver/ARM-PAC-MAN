/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "game/shared.h"
#include "game/game.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void){	
	uint32_t joystick_state = LPC_GPIO1->FIOPIN & JOYSTICK_MASK;
	joystick_state = ~joystick_state & JOYSTICK_MASK;
	
	if(joystick_state == 0){
		curr_joystick_position = curr_joystick_position; // If no action selected perform the previos action
	} else if (!(joystick_state & (joystick_state - 1))) {
		switch (joystick_state) {
        case (1 << 25): //SEL
            //No action
            break;
        case (1 << 26): //DOWN
						curr_joystick_position = DOWN;
            break;
        case (1 << 27): //LEFT
						curr_joystick_position = LEFT;
            break;
        case (1 << 28):	//RIGHT
						curr_joystick_position = RIGHT;
            break;
        case (1 << 29):	//UP
            curr_joystick_position = UP;
            break;
        default:
            //No action
            break;
    }
	} else {
		curr_joystick_position = NONE; //If more than one position is selected, ignore it. 
	} 

  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

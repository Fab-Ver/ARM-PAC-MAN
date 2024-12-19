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
#include "../led/led.h"
#include <stdio.h>
#include "GLCD.h"
#include "joystick/funct_joystick.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{	
	static uint32_t prev_joystick_state = (1 << 25);
	uint32_t joystick_state = LPC_GPIO1->FIOPIN & JOYSTICK_MASK;
	
	joystick_state = ~joystick_state & JOYSTICK_MASK;
	/*
		If no action selected perform the previos action
	*/
	if(joystick_state == 0){
		perform_action(prev_joystick_state);
	} else if (!(joystick_state & (joystick_state - 1))) {
		perform_action(joystick_state);
		prev_joystick_state = joystick_state;
	} 

  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

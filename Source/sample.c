/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "game/game.h"
#include "button/button.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"
#include "game/shared.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

int main(void){
	
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
	/*TIMER 1: Random number generation */
	init_timer(1, 0x1312D0 );	/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	NVIC_SetPriority(TIMER1_IRQn, TIMER1_PRIORITY);
	enable_timer(1);
	
	/* RIT: Joystick sampling*/ 
	init_RIT(0x1312D0);
	NVIC_SetPriority(RIT_IRQn, RIT_PRIORITY);
	
	/* TIMER 0 game countdown*/
	init_timer(0, 0x17D7840); 								// 1s 
	NVIC_SetPriority(TIMER0_IRQn, TIMER0_PRIORITY);
	
	/* TIMER 2: Game updates */ 
	init_timer(2, 0x2625A0);
	NVIC_SetPriority(TIMER2_IRQn, TIMER2_PRIORITY);
	
	
  LCD_Initialization();
	joystick_init();
	
	init_game();
	
	enable_timer(2);
	BUTTON_init();
	
	disable_timer(1); //After seed generation, no more needed. 	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);
	
  while (1){	
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include <stdio.h> /*for sprintf*/
#include <string.h>
#include "game/game.h"

#include "game/config.h"

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
extern uint8_t lives;
extern uint16_t current_score;

void TIMER0_IRQHandler (void)
{
	second--;
	char str[20];
  snprintf((char *)str, sizeof(str), "%d", second);
	GUI_Text(10, 30, (uint8_t *) "   ", Black, Black);
	GUI_Text(10, 30, (uint8_t *) strcat(str, "s"), White, Black);
	if(second == 0){
		lives--;
		second = 60;
		draw_lives();
		if(lives == 0){
			if(current_score == (STD_PILLS*STD_SCORE)+(POWER_PILLS*POWER_SCORE)){
				GUI_Text(90, 190, (uint8_t *) "VICTORY!", Yellow, Blue);
				disable_RIT();
				disable_timer(0);
				NVIC_DisableIRQ(EINT1_IRQn);
				LPC_PINCON->PINSEL4    &= ~(1 << 20);     
			} else {
				GUI_Text(85, 190, (uint8_t *) "GAME OVER", Yellow, Blue);
				disable_RIT();
				disable_timer(0);
				NVIC_DisableIRQ(EINT1_IRQn);
				LPC_PINCON->PINSEL4    &= ~(1 << 20);
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

/******************************************************************************
**                            End Of File
******************************************************************************/

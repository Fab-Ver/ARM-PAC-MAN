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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "game/game.h"
#include "button/button.h"
#include "RIT/RIT.h"
#include "joystick/joystick.h"


#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

bool pause = true; 
uint8_t PacMan_X = 11; 
uint8_t PacMan_Y = 18;
uint8_t current_Pills[25][23] = {0};
uint16_t current_score = 0;
uint8_t lives = 1; 

uint32_t seed;

uint32_t seed_with_adc_and_timer();

int main(void)
{
	
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	init_timer(1, 0x1312D0 );										/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	enable_timer(1);
	
	BUTTON_init();
  LCD_Initialization();
	draw_map();
	draw_lives();
	draw_pac_man(PacMan_X, PacMan_Y, 11, 11);
	GUI_Text(100, 190, (uint8_t *) "PAUSE", Yellow, Blue);
	joystick_init();
	update_graphic_score(current_score);
	seed = seed_with_adc_and_timer(); 
	disable_timer(1);
	generate_power_pills();
	draw_pills();
	

	init_RIT(0x98968);
	init_timer(0, 0x17D7840); 								// 1s 
	
	
	      
	//init_timer(0, 0x6108 ); 						      /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						        /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	//init_timer(0, 0xC8 ); 						        /* 8us * 25MHz = 200 ~= 0xC8 */
	
	
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
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

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

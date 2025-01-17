/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include "../GLCD/GLCD.h"
#include "game/game.h"
#include "game/shared.h"

extern uint8_t icr ; 										//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg       CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg       CAN_RxMsg;    /* CAN message for receiving */

volatile uint16_t msg_score;
volatile uint8_t msg_countdown;
volatile uint8_t msg_lives;

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {

  /* check CAN controller 1 */
	//icr = 0;
  //icr = (LPC_CAN1->ICR | icr) & 0xFF;               /* clear interrupts */
	
  //if (icr & (1 << 0)) {                          		/* CAN Controller #1 meassage is received */
		//CAN_rdMsg (1, &CAN_RxMsg);	                		/* Read the message */
    //LPC_CAN1->CMR = (1 << 2);                    		/* Release receive buffer */
		/*Do nothing in this case CAN #1 only transmits*/
  //}
	
	//if (icr & (1 << 1)) {                         /* CAN Controller #1 meassage is transmitted */
		/* Do nothing in this example */
	//}
		
	/* check CAN controller 2 */
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF;             /* clear interrupts */

	if (icr & (1 << 0)) {                          	/* CAN Controller #2 meassage is received */
		CAN_rdMsg (2, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN2->CMR = (1 << 2);                    		/* Release receive buffer */
		
		msg_score = (CAN_RxMsg.data[1] << 8) | CAN_RxMsg.data[0];
		msg_lives = CAN_RxMsg.data[2];
		msg_countdown = CAN_RxMsg.data[3]; 
		
		if (msg_score != prev_score){
			update_stats();
			prev_score = msg_score;
		}
		
		if(msg_lives != prev_lives){
			draw_lives();
			prev_lives = msg_lives;
		}
		
		if(msg_countdown != prev_countdown){
			draw_countdown();
			prev_countdown = msg_countdown;
		}
	}
	
	if (icr & (1 << 1)) {                         /* CAN Controller #2 meassage is transmitted */
		/* Do nothing in this example */
	}
}

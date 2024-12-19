																			 /*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_joystick, funct_joystick .c files
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

/* lib_joystick */
#ifndef __JOYSTICK_H
#define __JOYSTICK_H

#define JOYSTICK_MASK ((1 << 29) | (1 << 28) | (1 << 27) | (1 << 26) | (1 << 25))

void joystick_init(void);

#endif
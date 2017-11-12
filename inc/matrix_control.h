/****************************************

File name     : matrix_control.h
Author        : Simon Benoit
Creation date : 25/02/2017

****************************************/

#ifndef __MATRIX_CONTROL_H
#define __MATRIX_CONTROL_H

//****************************************INCLUDE******************************************//
#include "stm32f4xx_hal.h"

//****************************************DEFINES******************************************//

//****************************************Global Constants*********************************//

//****************************************Global Variables*********************************//

//****************************************Global Function Prototype************************//

void    MATRIX_Update       (void);
void    MATRIX_SetPixel     (uint8_t x, uint8_t y, uint32_t color);
void    MATRIX_FillMatrix   (uint32_t color);
void    MATRIX_Clear        (void);

#endif // __MATRIX_CONTROL_H

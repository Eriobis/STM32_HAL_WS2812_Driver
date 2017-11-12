/****************************************
File name     : matrix_control.c
Author        : Simon Benoit
Creation date : 25/02/2017

****************************************/

//****************************************INCLUDE******************************************//

#include <stdlib.h>
#include <string.h>
#include "ws2812_dma.h"
#include "matrix_control.h"

//****************************************DEFINES******************************************//

#define MATRIX_WIDTH    16
#define MATRIX_LENGTH   16
#define MATRIX_TOTAL_SIZE (MATRIX_WIDTH * MATRIX_LENGTH)
#define NUMBER_OF_COLOR 3

typedef struct _ledStruct_t
{
    uint8_t green;
    uint8_t red;
    uint8_t blue;
}ledStruct_t;

ledStruct_t matrixBuffer[MATRIX_WIDTH][MATRIX_LENGTH];

//****************************************Global Constants*********************************//

//****************************************Global Variables*********************************//

//****************************************Function Prototypes******************************//

//****************************************Local Function***********************************//

//****************************************Global Function**********************************//

void MATRIX_SetPixel(uint8_t x, uint8_t y, uint32_t color)
{
    matrixBuffer[x][y].blue = (color & 0x000000FFF);
    matrixBuffer[x][y].green = (color & 0x0000FF00) >> 8;
    matrixBuffer[x][y].red = (color & 0x00FF0000) >> 16;
}

void MATRIX_FillMatrix(uint32_t color)
{
    int i, j;

    for (i = 0; i < MATRIX_WIDTH; i++)
    {
        for (j = 0; j < MATRIX_LENGTH; j++)
        {
            MATRIX_SetPixel(i, j, color);
        }
    }
}

void MATRIX_Clear()
{
    memset(matrixBuffer, 0, MATRIX_TOTAL_SIZE);
}

void MATRIX_Update()
{
    int i, j, k;
    uint16_t *data;
    uint8_t *matrixPtr = (uint8_t *)matrixBuffer;

    data = WS2812_GetBufferAddr();

    for (i = 0; i < (MATRIX_TOTAL_SIZE); i++)
    {
        for (j = 0; j < NUMBER_OF_COLOR; j++)
        {
            //MSb first
            for (k = 7; k >= 0; k--)
            {
                *data = (*matrixPtr & (1 << k)) ? 72 : 34;
                data++;
            }
            matrixPtr++;
        }
    }
}

//****************************************Interrupts Handler*******************************//

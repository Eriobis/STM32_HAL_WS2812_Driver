/**********************************************************************************************************************
 * @file    ws2812_dma.c
 * @author  Simon Benoit
 * @date    12-11-2017
 * @brief
 *********************************************************************************************************************/

/* Includes ---------------------------------------------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "ws2812_dma.h"
#include "tim.h"
#include "dma.h"

/* Local Defines ----------------------------------------------------------------------------------------------------*/

#define WS2812_BIT_PER_COLOR    8
#define WS2812_PIXEL_LENGTH     (WS2812_BIT_PER_COLOR * WS2812_NUMBER_OF_COLOR)
#define WS2812_PWM_BUFF_SIZE    ((WS2812_NUMBER_OF_PIXEL * WS2812_PIXEL_LENGTH) + WS2812_LATCH_CYCLE)

/* Local Typedefs ---------------------------------------------------------------------------------------------------*/

/* Forward Declarations ---------------------------------------------------------------------------------------------*/

static void WS2812_GPIOInit(TIM_HandleTypeDef *timHandle);

/* Local Constants --------------------------------------------------------------------------------------------------*/

/* Local Variables --------------------------------------------------------------------------------------------------*/

static uint16_t     PWMRatioBuffer[WS2812_PWM_BUFF_SIZE];
TIM_HandleTypeDef   htim3;
DMA_HandleTypeDef   hdma_tim3_ch1_trig;

/* Local Functions --------------------------------------------------------------------------------------------------*/

static void WS2812_GPIOInit(TIM_HandleTypeDef *timHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if (timHandle->Instance == TIM3)
    {
        /**TIM3 GPIO Configuration -  PA6 ------> TIM3_CH1 */
        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_TIM3_CLK_DISABLE();

        /* TIM3 DMA DeInit */
        HAL_DMA_DeInit(tim_baseHandle->hdma[TIM_DMA_ID_CC1]);
    }
}

/* Global Functions -------------------------------------------------------------------------------------------------*/

/**
  *--------------------------------------------------------------------------------------------------------------------
  * @brief  Initialise WS2812 with TIM3 and its DMA stream
  *
  * @param  none
  *
  * @retval none
  *
  *--------------------------------------------------------------------------------------------------------------------
  */
void WS2812_Init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig;
    TIM_MasterConfigTypeDef sMasterConfig;
    TIM_OC_InitTypeDef sConfigOC;

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 0;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 106;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    /* TIM3 DMA Init */
    /* TIM3_CH1_TRIG Init */
    hdma_tim3_ch1_trig.Instance = DMA1_Stream4;
    hdma_tim3_ch1_trig.Init.Channel = DMA_CHANNEL_5;
    hdma_tim3_ch1_trig.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim3_ch1_trig.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim3_ch1_trig.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim3_ch1_trig.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_tim3_ch1_trig.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_tim3_ch1_trig.Init.Mode = DMA_CIRCULAR;
    hdma_tim3_ch1_trig.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_tim3_ch1_trig.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_tim3_ch1_trig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one stream to perform all the requested DMAs. */
    __HAL_LINKDMA(&htim3, hdma[TIM_DMA_ID_CC1], hdma_tim3_ch1_trig);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }

    WS2812_GPIOInit(&htim3);
}

/**
  *--------------------------------------------------------------------------------------------------------------------
  * @brief  Start the DMA stream in circular mode
  *
  * @param  none
  *
  * @retval none
  *
  *--------------------------------------------------------------------------------------------------------------------
  */
void WS2812_StartStream()
{
    memset(PWMRatioBuffer, 0, WS2812_PWM_BUFF_SIZE);
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t *)PWMRatioBuffer, WS2812_PWM_BUFF_SIZE);
}

/**
  *--------------------------------------------------------------------------------------------------------------------
  * @brief  Get the PWM buffer address ( this is where the DMA reads its data )
  *
  * @param  none
  *
  * @retval PWMbuffer address
  *
  *--------------------------------------------------------------------------------------------------------------------
  */
uint16_t *WS2812_GetBufferAddr()
{
    return PWMRatioBuffer;
}

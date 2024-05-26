/**
  ******************************************************************************
  * @file           : clock.c
  * @brief          : Clock system setup and HAL timebase
  * @author         : svcguy
  * @details        :
  * @copyright      : Copyright svcguy 05/2024
  *                   Permission is hereby granted, free of charge, to any
  *                   person obtaining a copy of this software and associated
  *                   documentation files (the “Software”), to deal in the
  *                   Software without restriction, including without
  *                   limitation the rights to use, copy, modify, merge,
  *                   publish, distribute, sublicense, and/or sell copies of
  *                   the Software, and to permit persons to whom the Software
  *                   is furnished to do so, subject to the following
  *                   conditions:
  *
  *                   The above copyright notice and this permission notice 
  *                   shall be included in all copies or substantial portions 
  *                   of the Software. 
  *
  *                   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY
  *                   KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
  *                   WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
  *                   PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
  *                   OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  *                   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
  *                   OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
  *                   THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  ******************************************************************************
  */

/* Includes-----------------------------------------------------------------*/
#include "clock.h"
/* Private typedef----------------------------------------------------------*/
/* Private define-----------------------------------------------------------*/
/* Private macro------------------------------------------------------------*/
/* Private variables--------------------------------------------------------*/
TIM_HandleTypeDef htim14;
/* Private function prototypes----------------------------------------------*/
/* Private functions--------------------------------------------------------*/

/**
 * @brief   This function initalizes the clock tree
 * @param   None
 * @returns HAL_OK on success, error code on failure
*/
HAL_StatusTypeDef Clock_Init(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  RCC_OscInitTypeDef oscInit = {0};
  RCC_ClkInitTypeDef clkInit = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  // Setup clock tree to run everything we need at full bore
  oscInit.OscillatorType =  RCC_OSCILLATORTYPE_HSE;
  oscInit.HSEState =        RCC_HSE_BYPASS;
  oscInit.PLL.PLLState =    RCC_PLL_ON;
  oscInit.PLL.PLLSource =   RCC_PLLSOURCE_HSE;
  oscInit.PLL.PLLM =        4;
  oscInit.PLL.PLLN =        216;
  oscInit.PLL.PLLP =        RCC_PLLP_DIV2;
  oscInit.PLL.PLLQ =        4;
  oscInit.PLL.PLLR =        2;

  ret = HAL_RCC_OscConfig(&oscInit);
  if(ret != HAL_OK)
  {
    return ret;
  }

  clkInit.ClockType =       RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                            RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  clkInit.SYSCLKSource =    RCC_SYSCLKSOURCE_PLLCLK;
  clkInit.AHBCLKDivider =   RCC_SYSCLK_DIV1;
  clkInit.APB1CLKDivider =  RCC_HCLK_DIV4;
  clkInit.APB2CLKDivider =  RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&clkInit, FLASH_LATENCY_7);

  return ret;
}

/**
  * @brief  This function configures the TIM14 as a time base source.
  *         The time source is configured  to have 1ms time base with a dedicated
  *         Tick interrupt priority.
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
  * @param  TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  RCC_ClkInitTypeDef clkConfig;
  uint32_t uwTimClock, pFlashLatency, uwAPB1Prescaler = 0, uwPrescalerValue = 0;
  HAL_StatusTypeDef ret = HAL_OK;

  // Get current clock info
  HAL_RCC_GetClockConfig(&clkConfig, &pFlashLatency);

  uwAPB1Prescaler = clkConfig.APB1CLKDivider;
  if(uwAPB1Prescaler == RCC_HCLK_DIV1)
  {
    uwTimClock = HAL_RCC_GetPCLK1Freq();
  }
  else
  {
    uwTimClock = HAL_RCC_GetPCLK1Freq() * 2;
  }

  uwPrescalerValue = (uint32_t)((uwTimClock / 1000000U) - 1U);

  // Setup TIM14 for 1ms tick freq
  htim14.Instance =                 TIM14;
  htim14.Init.Period =              (1000000U / 1000U) - 1U;
  htim14.Init.Prescaler =           uwPrescalerValue;
  htim14.Init.ClockDivision =       0;
  htim14.Init.CounterMode =         TIM_COUNTERMODE_UP;
  htim14.Init.AutoReloadPreload =   TIM_AUTORELOAD_PRELOAD_DISABLE;

  __HAL_RCC_TIM14_CLK_ENABLE();
  ret = HAL_TIM_Base_Init(&htim14);

  if(ret != HAL_OK)
  {
    return ret;
  }

  ret = HAL_TIM_Base_Start_IT(&htim14);
  if(ret != HAL_OK)
  {
    return ret;
  }

  HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  if(!(TickPriority < (1UL << __NVIC_PRIO_BITS)))
  {
    return HAL_ERROR;
  }

  HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, TickPriority, 0);
  uwTickPrio = TickPriority;

  return ret;
}

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling TIM6 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_SuspendTick(void)
{
  /* Disable TIM6 update Interrupt */
  __HAL_TIM_DISABLE_IT(&htim14, TIM_IT_UPDATE);
}

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling TIM6 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_ResumeTick(void)
{
  /* Enable TIM6 Update interrupt */
  __HAL_TIM_ENABLE_IT(&htim14, TIM_IT_UPDATE);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called when TIM14 interrupt took place, inside
  *         HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  *         a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM14)
  {
    HAL_IncTick();
  }
}

/**
  ******************************************************************************
  * @file           : gpio.c
  * @brief          :
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
#include "gpio.h"
/* Private typedef----------------------------------------------------------*/
/* Private define-----------------------------------------------------------*/
/* Private macro------------------------------------------------------------*/
/* Private variables--------------------------------------------------------*/
/* Private function prototypes----------------------------------------------*/
/* Private functions--------------------------------------------------------*/

/**
 * @brief   This function initalizes the GPIO pins in use
 * @param   None
 * @returns HAL_OK on success, error code on failure
*/
HAL_StatusTypeDef
GPIO_Init(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  GPIO_InitTypeDef gpio = {0};

  // Onboard LEDs
  //  LD1 - Green led, PB0
  //  LD2 - Blue led, PB7
  //  LD3 - Red led, PB14
  gpio.Mode       = GPIO_MODE_OUTPUT_PP;
  gpio.Speed      = GPIO_SPEED_FREQ_LOW;
  gpio.Pull       = GPIO_NOPULL;
  gpio.Pin        = GPIO_PIN_0 | GPIO_PIN_7 | GPIO_PIN_14;

  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_Init(GPIOB, &gpio);

  // Onboard push button - PC13
  gpio.Mode       = GPIO_MODE_IT_RISING;
  gpio.Speed      = GPIO_SPEED_FREQ_LOW;
  gpio.Pull       = GPIO_NOPULL;
  gpio.Pin        = GPIO_PIN_13;

  __HAL_RCC_GPIOC_CLK_ENABLE();
  HAL_GPIO_Init(GPIOC, &gpio);

  // Setup interrupt for button
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  // UART pins
  //  PD8 - UART3 TX
  //  PD9 - UART3 RX
  gpio.Mode       = GPIO_MODE_AF_PP;
  gpio.Speed      = GPIO_SPEED_FREQ_HIGH;
  gpio.Pull       = GPIO_NOPULL;
  gpio.Alternate  = GPIO_AF7_USART3;
  gpio.Pin        = GPIO_PIN_8 | GPIO_PIN_9;

  __HAL_RCC_GPIOD_CLK_ENABLE();
  HAL_GPIO_Init(GPIOD, &gpio);

  return ret;
}

/**
 * @brief   The EXTI interrupt callback
 * @param   GPIO_Pin the pin number responsible for the interrupt
 * @returns None
*/
void
HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_13)
  {

  }
}
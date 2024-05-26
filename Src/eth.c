/**
  ******************************************************************************
  * @file           : eth.c
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
#include "eth.h"
/* Private typedef----------------------------------------------------------*/
/* Private define-----------------------------------------------------------*/
/* Private macro------------------------------------------------------------*/
/* Private variables--------------------------------------------------------*/
/* Private function prototypes----------------------------------------------*/
/* Private functions--------------------------------------------------------*/

/**
 * @brief   This function initalizes the GPIO and clock for the ethernet
 *          hardware.  All other initialization is handled by
 *          FreeRTOS-PLus-TCP
 * @param   heth A pointer to the ethernet interface handle
 * @returns None
*/
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
  // Enable Ethernet Interface Clocks
  __HAL_RCC_ETH_CLK_ENABLE();
  
  // Setup GPIO
  //  RMII_TXD0     = PG13
  //  RMII_TXD1     = PB13
  //  RMII_TX_EN    = PG11
  //  RMII_RXD0     = PC4
  //  RMII_RXD1     = PC5
  //  RMII_CRS_DV   = PA7
  //  RMII_REF_CLK  = PA1
  //  RMII_MDIO     = PA2
  //  RMII_MDC      = PC1
  GPIO_InitTypeDef eth_pin = {0};

  // PORTA
  eth_pin.Mode      = GPIO_MODE_AF_PP;
  eth_pin.Pull      = GPIO_NOPULL;
  eth_pin.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  eth_pin.Alternate = GPIO_AF11_ETH;
  eth_pin.Pin       = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_7;
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  HAL_GPIO_Init(GPIOA, &eth_pin);

  // PORTB
  eth_pin.Pin       = GPIO_PIN_13;

  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_Init(GPIOB, &eth_pin);

  // PORTC
  eth_pin.Pin       = GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;

  __HAL_RCC_GPIOC_CLK_ENABLE();
  HAL_GPIO_Init(GPIOC, &eth_pin);

  // PORTG
  eth_pin.Pin       = GPIO_PIN_11 | GPIO_PIN_13;

  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_GPIO_Init(GPIOG, &eth_pin);

  // Configure ETH NVIC
  HAL_NVIC_SetPriority(ETH_IRQn, ipconfigMAC_INTERRUPT_PRIORITY, 0);
  HAL_NVIC_EnableIRQ(ETH_IRQn);
}

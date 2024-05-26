/**
  ******************************************************************************
  * @file           : rng.c
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
#include "rng.h"
/* Private typedef----------------------------------------------------------*/
/* Private define-----------------------------------------------------------*/
/* Private macro------------------------------------------------------------*/
/* Private variables--------------------------------------------------------*/
RNG_HandleTypeDef hrng;
/* Private function prototypes----------------------------------------------*/
/* Private functions--------------------------------------------------------*/

/**
 * @brief   This function initalizes the hardware random number generator
 * @param   None
 * @returns HAL_OK on success, error code on failure
*/
HAL_StatusTypeDef 
RNG_Init(void)
{
  HAL_StatusTypeDef ret = HAL_OK;

  hrng.Instance = RNG;

  __HAL_RCC_RNG_CLK_ENABLE();
  ret = HAL_RNG_Init(&hrng);

  if(ret != HAL_OK)
  {
    // Handle error
  }
  
  return ret;
}
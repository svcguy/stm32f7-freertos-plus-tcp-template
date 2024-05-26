/**
  ******************************************************************************
  * @file           : rtos.c
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
#include "rtos.h"
/* Private typedef----------------------------------------------------------*/
/* Private define-----------------------------------------------------------*/
/* Private macro------------------------------------------------------------*/
/* Private variables--------------------------------------------------------*/
TaskHandle_t heartbeatTaskHandle;
/* Private function prototypes----------------------------------------------*/
static void heartbeat_task_entry(void *pvParameters);
/* Private functions--------------------------------------------------------*/

/**
 * @brief   This function will create all the tasks and RTOS primatives
 *          the application will use
 * @param   None
 * @returns HAL_OK on success, an error code on failure
*/
HAL_StatusTypeDef
RTOS_Init(void)
{
  HAL_StatusTypeDef ret = HAL_OK;
  BaseType_t freertosRet = pdPASS;

  // Create the heartbeat task
  freertosRet = xTaskCreate(heartbeat_task_entry, 
                            "heartbeat_task",
                            configMINIMAL_STACK_SIZE,
                            NULL,
                            configMAX_PRIORITIES-1,
                            NULL);
  if(freertosRet != pdPASS)
  {
    return HAL_ERROR;
  }

  return ret;
}

/**
 * @brief   The default 'heartbeat' task.  It just blinks an LED to prove the
 *          application is running
 * @param   pvParameters - not used
 * @returns ...it doesn't
*/
void
heartbeat_task_entry(void *pvParameters)
{
  UNUSED(pvParameters);

  while(1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}
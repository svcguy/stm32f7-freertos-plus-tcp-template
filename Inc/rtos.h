/**
  ******************************************************************************
  * @file           : rtos.h
  * @brief          : Header for rtos.c file.
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

/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __RTOS_H
#define __RTOS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes-----------------------------------------------------------------*/
#include "main.h"
/* Exported types-----------------------------------------------------------*/
/* Exported constants-------------------------------------------------------*/
/* Exported macro-----------------------------------------------------------*/
/* Exported variables-------------------------------------------------------*/
extern TaskHandle_t heartbeatTaskHandle;
/* Exported function prototypes---------------------------------------------*/
HAL_StatusTypeDef RTOS_Init(void);
/* Exported defines---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __RTOS_H */

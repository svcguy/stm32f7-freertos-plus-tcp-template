/**
 * stm32f7xx_it.c
 * 
 * STM32F7 Interrupt Handler File
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim14;

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  // If debugger is attached, trigger breakpoint
  if(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
  {
    __BKPT(0);
  }
  __disable_irq();
  while (1)
  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /// If debugger is attached, trigger breakpoint
  if(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
  {
    __BKPT(0);
  }
  __disable_irq();
  while (1)
  {
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  // If debugger is attached, trigger breakpoint
  if(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
  {
    __BKPT(0);
  }
  __disable_irq();
  while (1)
  {
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  // If debugger is attached, trigger breakpoint
  if(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
  {
    __BKPT(0);
  }
  __disable_irq();
  while (1)
  {
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  // If debugger is attached, trigger breakpoint
  if(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
  {
    __BKPT(0);
  }
  __disable_irq();
  while (1)
  {
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM8 trigger and commutation interrupts and TIM14 global interrupt.
  */
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim14);
}

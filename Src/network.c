/**
  ******************************************************************************
  * @file           : network.c
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
#include "network.h"
/* Private typedef----------------------------------------------------------*/
/* Private define-----------------------------------------------------------*/
/* Private macro------------------------------------------------------------*/
/* Private variables--------------------------------------------------------*/
// Use MAC address defined in stm32f7xx_hal_conf.h
static uint8_t ucMACAddress[6] = {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};
// Using DHCP, but we need to supply addresses to use in case DHCP does not get
//  a reservation
static const uint8_t ucIPAddress[4] = {169, 254, 0, 3};
static const uint8_t ucNetMask[4] = {255, 255, 0, 0};
static const uint8_t ucGatewayAddress[4] = {0, 0, 0, 0};
static const uint8_t ucDNSServerAddress[4] = {8, 8, 8, 8};
// Only one physical interface
static NetworkInterface_t xInterfaces[ 1 ];
// Only one end point
static NetworkEndPoint_t xEndPoints[ 1 ];
/* Private function prototypes----------------------------------------------*/
// There's no header for the port FillInterfaceDescriptor that I could find
//  so declare it here to avoid compiler warning TODO: FIX THIS
NetworkInterface_t * pxSTM32Fxx_FillInterfaceDescriptor( BaseType_t xEMACIndex,
                                                         NetworkInterface_t * pxInterface );
/* Private functions--------------------------------------------------------*/

/**
 * @brief   This function handles the callbacks from the TCP/IP stack
 * @param   eNetworkEvent the callback event type
 * @param   pxEndPoint the endpoint the event is for
 * @returns None
*/
void
vApplicationIPNetworkEventHook_Multi(eIPCallbackEvent_t eNetworkEvent,
                                     struct xNetworkEndPoint * pxEndPoint)
{
  static BaseType_t xTasksAlreadyCreated = pdFALSE;
  uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
  char cBuffer[ 16 ];

  /* Both eNetworkUp and eNetworkDown events can be processed here. */
  if( eNetworkEvent == eNetworkUp )
  {
    /* Create the tasks that use the TCP/IP stack if they have not already
    been created. */
    if( xTasksAlreadyCreated == pdFALSE )
    {
      /*
        * For convenience, tasks that use FreeRTOS-Plus-TCP can be created here
        * to ensure they are not created before the network is usable.
        */

      xTasksAlreadyCreated = pdTRUE;
    }
  }
  
  FreeRTOS_GetEndPointConfiguration( &ulIPAddress, &ulNetMask, &ulGatewayAddress, &ulDNSServerAddress, pxNetworkEndPoints );
        
  FreeRTOS_inet_ntoa(ulIPAddress, cBuffer);
  printf("\r\n\r\nIP Address: %s\r\n", cBuffer);

  FreeRTOS_inet_ntoa(ulNetMask, cBuffer);
  printf("Subnet Mask: %s\r\n", cBuffer);

  FreeRTOS_inet_ntoa(ulGatewayAddress, cBuffer);
  printf("Gateway Address: %s\r\n", cBuffer);

  FreeRTOS_inet_ntoa(ulDNSServerAddress, cBuffer);
  printf("DNS Server Address: %s\r\n\r\n\r\n", cBuffer);
}

/**
 * @brief   This function handles DHCP callbacks
 * @param   eDHCPPhase
 * @param   pxEndPoint the endpoint that the event belongs to
 * @param   pxIPAddress
 * @returns eDHCPCallbackAnswer_t - 
*/
eDHCPCallbackAnswer_t 
xApplicationDHCPHook_Multi(eDHCPCallbackPhase_t eDHCPPhase,
                            struct xNetworkEndPoint *pxEndPoint,
                            IP_Address_t * pxIPAddress)
{
  return eDHCPContinue;
}

/**
 * @brief   This function handles DNS query callbacks
 * @param   pxEndPoint the endpoint that the event belongs to
 * @param   pcName
 * @returns BaseType_t - 
*/
BaseType_t
xApplicationDNSQueryHook_Multi(struct xNetworkEndPoint *pxEndPoint,
                                const char *pcName )
{
  BaseType_t xReturn = pdPASS;

  return xReturn;
}

/**
 * @brief   This function gets random numbers for the TCP/IP stack.  It
 *          uses the MCU's hardware random number generator
 * @param   pulNumber a pointer to a 32-bit random number
 * @returns pdPASS on success, pdFAIL on failure
*/
BaseType_t 
xApplicationGetRandomNumber(uint32_t *pulNumber)
{
  HAL_StatusTypeDef ret = HAL_RNG_GenerateRandomNumber(&hrng, pulNumber);

  if(ret != HAL_OK)
  {
    return pdFAIL;
  }
  else
  {
    return pdPASS;
  }
}

/**
 * @brief   This function generates hard to guess inital sequence numbers
 *          for TCP.  It should be RFC6128 compliant.  This implementation
 *          is not, it just generates a random number using the MCU's hardware
 *          random number generator
 * @param   ulSourceAddress
 * @param   usSourcePort
 * @param   ulDestinationAddress
 * @param   usDestinationPort
 * @returns A 32-bit unsigned int for the inital sequence number
*/
uint32_t
ulApplicationGetNextSequenceNumber(uint32_t ulSourceAddress,
                                    uint16_t usSourcePort,
                                    uint32_t ulDestinationAddress,
                                    uint16_t usDestinationPort)
{
    UNUSED(ulSourceAddress);
    UNUSED(usSourcePort);
    UNUSED(ulDestinationAddress);
    UNUSED(usDestinationPort);

    uint32_t randomNumber;

    HAL_RNG_GenerateRandomNumber(&hrng, &randomNumber);

    return randomNumber;
}

/**
 * @brief   This function initalizes the TCP/IP stack
 * @param   None
 * @returns HAL_OK on success, an error code on failure
*/
HAL_StatusTypeDef Network_Init(void)
{
  HAL_StatusTypeDef ret = HAL_OK;

  // Setup interfaces and endpoints
  pxSTM32Fxx_FillInterfaceDescriptor(0, &(xInterfaces[0]));
  FreeRTOS_FillEndPoint(&(xInterfaces[0]), &(xEndPoints[0]), ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);
  xEndPoints[0].bits.bWantDHCP = pdTRUE;

  // Start FreeRTOS-Plus-TCP
  if(FreeRTOS_IPInit_Multi() != pdPASS)
  {
    return HAL_ERROR;
  }

  return ret;
}
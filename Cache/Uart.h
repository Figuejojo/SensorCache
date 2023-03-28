/** @file Uart.h
 *  @brief Header file for UART functions and tasks.
 *					Two RTOS tasks:
 *						- Sensing information to the user.
 *						- Receiving, appending and interpresting incoming user data.
 *
 *  @author Y3913624
 */
 
#ifndef _UART_H_
#define _UART_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "CacheDTypes.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/** @name 	vTxUart Task
	*	@brief 	Send the information received from the UartTxQueue
  *
  * @param 	Void
  */
portTASK_FUNCTION_PROTO(vTxUart, pvParameters);

/** @name 	vRxUart Task
	*	@brief 	Collect information from UART append the characters as well as 
	*						interpresting and validating the input message ... (TODO)
  *
  * @param 	Void
  */
portTASK_FUNCTION_PROTO(vRxUart, pvParameters);

#endif //_UART_H_

/** @file Uart.c
 *  @brief Implementation file for UART functions and tasks.
 *					Two RTOS tasks:
 *						- Sensing information to the user.
 *						- Receiving, appending and interpresting incoming user data.
 *
 *  @author Y3913624
 */

/*******************************************************************************
* Includes
*******************************************************************************/
#include "Uart.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/
xTaskHandle hPrintTask;		/*!< RTOS handler for USART Tx or Print Task   */
xTaskHandle hRxUart;			/*!< RTOS handler for USART Rx Task				     */

xQueueHandle UartTxQueue;	/*!< RTOS queue message, USART output  		     */
xQueueHandle UartRxQueue;	/*!< RTOS queue message, USART input (From INT)*/

/*******************************************************************************
* Function Implementation
*******************************************************************************/
	
/**
	*	@name vAnalog
	* @Type	Task
*/
portTASK_FUNCTION(vRxUart, pvParameters)
{
	int8_t bp_command[20] = {0}; // For appending the message
	int8_t b_character = 0;
	int8_t b_ChCounter = 0;
	
	while(1)
	{
		xQueueReceive( UartRxQueue, &b_character, portMAX_DELAY); 
		
		if(b_character == '\n' || b_character == '\r')
		{
			bp_command[b_ChCounter++] = '\r';
			bp_command[b_ChCounter] = '\n';
			xQueueSendToBack(UartTxQueue, bp_command, portMAX_DELAY);
			memset(bp_command, 0, sizeof(bp_command)); 
			b_ChCounter = 0;
		}
		else
		{
			bp_command[b_ChCounter] = b_character;
			b_ChCounter++;
		}
		
		if(b_ChCounter >= 17)
		{
			xQueueSendToBack(UartTxQueue, bp_command, portMAX_DELAY);
			memset(bp_command, 0, sizeof(bp_command));
			snprintf((char *) bp_command,20," - Size Exeeded! \r\n");
			xQueueSendToBack(UartTxQueue, bp_command, portMAX_DELAY);
			memset(bp_command, 0, sizeof(bp_command));
			b_ChCounter = 0;
		}
	}
}

/**
	*	@name vTxUart
	* @Type	Task
*/ 
portTASK_FUNCTION(vTxUart, pvParameters) {
	
	int8_t b_message[100];
	
	while(1) 
	{
		// *** Receive a message from the queue and print it HERE
		xQueueReceive( UartTxQueue, b_message, portMAX_DELAY);
		printf("%s",b_message);
	}
}

/**
	*	@name USART2_IRQHandler
	* @Type	Interrupt Handler
*/ 
void USART2_IRQHandler()
{
	char c = USART_ReceiveData(USART2);
	xQueueSendToBackFromISR(UartRxQueue, &c, NULL);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
}

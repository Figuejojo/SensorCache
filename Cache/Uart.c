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
extern xQueueHandle CacheQueue;	/*!< RTOS queue message, Cache 					 */

/*******************************************************************************
* Function Implementation
*******************************************************************************/
	
/**
	*	@name vRxUart
	* @Type	Task
*/
portTASK_FUNCTION(vRxUart, pvParameters)
{
	int8_t bp_command[CMDMAXSZ] = {0}; // For appending the message
	uint8_t bp_msg[22] = {0};
	int8_t b_character = 0;
	int8_t b_ChCounter = 0;
	
	QCacheMsg_t Msg_t;
	Msg_t.task = NONE_e;
	
	while(1)
	{
		//Retreive character sent from Interrupt.
		xQueueReceive( UartRxQueue, &b_character, portMAX_DELAY); 
		
		// If the received character is '\n' then, the message validation starts. 
		if(b_character == '\n')
		{
			
			if(strcmp((char *)bp_command,"<AN0>\r") == 0) //Retreive Analog0 data from cache
			{
				Msg_t.task = EOutANALOG;
				xQueueSendToBack(CacheQueue, &Msg_t, portMAX_DELAY);
			}
			else if(strcmp((char *)bp_command,"<AN1>\r") == 0) //Retreive Analog1 data from cache
			{
				Msg_t.task = EOutANALOG1;
				xQueueSendToBack(CacheQueue, &Msg_t, portMAX_DELAY);
			}
			else if(strcmp((char *)bp_command,"<PAT>\r") == 0) //Retreive LEDs pattern from cache
			{
				Msg_t.task = EOutPATTRN;
				xQueueSendToBack(CacheQueue, &Msg_t, portMAX_DELAY);
			}
			else if(strcmp((char *)bp_command,"<ACC>\r") == 0) //Retreive LEDs pattern from cache
			{
				Msg_t.task = EOutACC;
				xQueueSendToBack(CacheQueue, &Msg_t, portMAX_DELAY);
			}
			else	//Error: Invalid command
			{
				bp_command[b_ChCounter-1] = '\0';
				snprintf((char*)bp_msg, INVMSGSZ,"<ERR|CMD>-Invalid:%s",bp_command);
				xQueueSendToBack(UartTxQueue, bp_msg, portMAX_DELAY);
			}
			memset(bp_command, 0, sizeof(bp_command)); 
			memset(bp_msg, 0, sizeof(bp_msg)); 
			b_ChCounter = 0;
		}
		else //Append incomming character.
		{
			bp_command[b_ChCounter] = b_character;
			b_ChCounter++;
		}
		
		if(b_ChCounter >= CMDMAXSZ-1) //Error: Message size exceeded
		{
			snprintf((char *) bp_msg,INVMSGSZ,"<ERR|CMD>-Size Exceed:%s",bp_command);
			xQueueSendToBack(UartTxQueue, bp_msg, portMAX_DELAY);
			
			memset(bp_command, 0, sizeof(bp_command));
			memset(bp_msg, 0, sizeof(bp_msg));
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
		printf("%s\r\n",b_message);
	}
}

/**
	*	@name USART2_IRQHandler
	* @Type	Interrupt Handler
*/ 
void USART2_IRQHandler()
{
	//Retreive character from USART register and sent it to task.
	char c = USART_ReceiveData(USART2);
	xQueueSendToBackFromISR(UartRxQueue, &c, NULL);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
}

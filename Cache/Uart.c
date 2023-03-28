#include "Uart.h"

xTaskHandle hPrintTask;
xTaskHandle hRxUart;

xQueueHandle UartTxQueue;
xQueueHandle UartRxQueue;

// This task should run whenever a message is waiting in the queue.
// ---------------------------------------------------------------------------- 
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

// This task should run whenever a message is waiting in the queue.
// ---------------------------------------------------------------------------- 
portTASK_FUNCTION(vTxUart, pvParameters) {
	
	int8_t b_message[100];
	
	while(1) 
	{
		// *** Receive a message from the queue and print it HERE
		xQueueReceive( UartTxQueue, b_message, portMAX_DELAY);
		printf("%s",b_message);
	}
}


void USART2_IRQHandler()
{
	char c = USART_ReceiveData(USART2);
	xQueueSendToBackFromISR(UartRxQueue, &c, NULL);
	USART_ClearFlag(USART2, USART_FLAG_RXNE);
}

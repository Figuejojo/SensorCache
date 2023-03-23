#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

#include "stm32f4xx.h"
#include "utils.h"

xTaskHandle hTask1;
xTaskHandle hPrintTask;
xTaskHandle hRxUart;

static xQueueHandle UartTxQueue;
static xQueueHandle UartRxQueue;
static xQueueHandle Cache;
//portTASK_FUNCTION_PROTO(vTask1, pvParameters);

portTASK_FUNCTION_PROTO(vTxUart, pvParameters);
portTASK_FUNCTION_PROTO(vRxUart,pvParameters);

// *** Declare a queue HERE

// ============================================================================
int main(void) {

	// Initialise all of the STM32F4DISCOVERY hardware (including the serial port)
	// Initialise all of the STM32F4DISCOVERY hardware (including the serial port)
	
	vUSART2_Init();
	vUSART2_RX_IRQ_Init();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	// *** Initialise the queue HERE
	UartRxQueue = xQueueCreate(20 ,sizeof(int8_t));
	UartTxQueue = xQueueCreate(10 ,100);

	// *** Initialise the queue HERE
	
	// Welcome message
	printf("\r\nFreeRTOS Test\r\n");

	// Tasks get started here.
	// Arguments to xTaskCreate are:
	// 1- The function to execute as a task (make sure it never exits!)
	// 2- The task name (keep it short)
	// 3- The stack size for the new task (configMINIMAL_STACK_SIZE == 130)
	// 4- Parameter for the task (we won't be using this, so set it to NULL)
	// 5- The task priority; higher numbers are higher priority, and the idle task has priority tskIDLE_PRIORITY
	// 6- A pointer to an xTaskHandle variable where the TCB will be stored
	//xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &hTask1);
	
	xTaskCreate(vRxUart, "RxUart",configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &hRxUart);
	xTaskCreate(vTxUart, "TxUart", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &hPrintTask);

	vTaskStartScheduler(); // This should never return.

	// Will only get here if there was insufficient memory to create
	// the idle task.
	while(1);  
}

#if 0
// This task should run every 500ms and send a message to the print task.
// ---------------------------------------------------------------------------- 
portTASK_FUNCTION(vTask1, pvParameters) {
	//portTickType xLastWakeTime = xTaskGetTickCount();
	
	while(1) 
	{
		// *** Insert a message into the queue HERE
		xQueueReceive( UartRxQueue, b_message, portMAX_DELAY);
		//vTaskDelayUntil(&xLastWakeTime, (500/portTICK_RATE_MS));
	}
}
#endif


// This task should run whenever a message is waiting in the queue.
// ---------------------------------------------------------------------------- 
portTASK_FUNCTION(vRxUart, pvParameters)
{
	int8_t bp_command[100]; // For appending the message
	int8_t b_character = 0;
	while(1)
	{
		
		xQueueReceive( UartRxQueue, &b_character, portMAX_DELAY); 
		
		//<PRS> - <ACC> - <MIC> - < > --> Process Message
		//<SNR><R/W>/r/n
		snprintf((char *) bp_command,100,"Character: %c \r\n",b_character);
		
		xQueueSendToBack(UartTxQueue, &b_character, portMAX_DELAY);

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


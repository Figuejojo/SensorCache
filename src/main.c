

#include "CacheDTypes.h"
#include "Cache.h"
#include "Pressure.h"

extern xTaskHandle hAnalog;
xTaskHandle hPrintTask;
xTaskHandle hRxUart;
extern xTaskHandle hCache;

xQueueHandle UartTxQueue;
static xQueueHandle UartRxQueue;
extern xQueueHandle CacheQueue;

portTASK_FUNCTION_PROTO(vTxUart, pvParameters);
portTASK_FUNCTION_PROTO(vRxUart, pvParameters);


// ============================================================================
int main(void) {

	// Initialise all of the STM32F4DISCOVERY hardware (including the serial port)
	// Initialise all of the STM32F4DISCOVERY hardware (including the serial port)
	
	vUSART2_Init();
	vUSART2_RX_IRQ_Init();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	initPRES();
	
	// *** Initialise the queue HERE
	UartRxQueue = xQueueCreate(20 ,sizeof(int8_t));
	UartTxQueue = xQueueCreate(10 ,20);
	CacheQueue = xQueueCreate(10,sizeof(QCacheMsg_t)); 
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
	
	xTaskCreate(vRxUart, "RxUart",configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1, &hRxUart);
	xTaskCreate(vTxUart, "TxUart", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, 	&hPrintTask);
	xTaskCreate(vAnalog, "Analog", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,&hAnalog);
	xTaskCreate(vCache, 	"Cache", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,&hCache);


	vTaskStartScheduler(); // This should never return.

	// Will only get here if there was insufficient memory to create
	// the idle task.
	while(1);  
}

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


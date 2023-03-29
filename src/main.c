
#include "CacheDTypes.h"
#include "Cache.h"
#include "Analog.h"
#include "Uart.h"
#include "ButLed.h"

extern xTaskHandle hAnalog;
extern xTaskHandle hPrintTask;
extern xTaskHandle hRxUart;
extern xTaskHandle hCache;
extern xTaskHandle hGPIO;

extern xQueueHandle UartTxQueue;
extern xQueueHandle UartRxQueue;
extern xQueueHandle CacheQueue;

// ============================================================================
int main(void) {

	// Initialise all of the STM32F4DISCOVERY hardware
	vUSART2_Init();
	vUSART2_RX_IRQ_Init();
	initLEDs();
	initBUTs();
	initAnalog();
	
	
	// *** Initialise the queue HERE
	UartRxQueue = xQueueCreate(20 ,sizeof(int8_t));
	UartTxQueue = xQueueCreate(10 ,22);
	CacheQueue  = xQueueCreate(10 ,sizeof(QCacheMsg_t)); 
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
	
	xTaskCreate(vRxUart, "RxUart", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+1,&hRxUart);
	xTaskCreate(vTxUart, "TxUart", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY  ,&hPrintTask);
	xTaskCreate(vAnalog, "Analog", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2,&hAnalog);
	xTaskCreate(vCache,  "Cache" , configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3,&hCache);
	xTaskCreate(vGPIO,   "GPIOs" , configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY  ,&hCache);

	vTaskStartScheduler(); // This should never return.

	// Will only get here if there was insufficient memory to create
	// the idle task.
	while(1);  
}

/** @file Cache.c
 *  @brief Implementation file dedicated to storing or save=ing sensor data into 
 *						the a cache. 
 *
 *  @author Y3913624
 */

/*******************************************************************************
* Includes
*******************************************************************************/
#include "Cache.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/
xTaskHandle hCache;								/*!< Cache RTOS handler for the Cache Task  */
xQueueHandle CacheQueue;					/*!< Cache RTOS Queue Message for the Cache	*/
extern xQueueHandle UartTxQueue;	/*!< Cache RTOS Queue Message for UART Tx		*/

/*******************************************************************************
* Function Implementation
*******************************************************************************/
	
/**
	*	@name vCache
	* @Type	Task
*/
portTASK_FUNCTION(vCache, pvParameters)
{
	QCacheMsg_t inMsg_t;
	int8_t outMsg_t[20] = {0};
	float cache_analog = 0;
	while(1)
	{
		xQueueReceive( CacheQueue, &inMsg_t, portMAX_DELAY);
		switch(inMsg_t.task)
		{
			case EInANALOG:
				cache_analog = inMsg_t.Value.voltage;
				break;
			
			case EOutANALOG:
				snprintf((char *)outMsg_t,20,"Volt Value: %0.2f\r\n",cache_analog);
				xQueueSendToBack(UartTxQueue, &outMsg_t, portMAX_DELAY);
				break;
				
			default:
				snprintf((char *)outMsg_t,20,"Error in Cache");
				xQueueSendToBack(UartTxQueue, &outMsg_t, portMAX_DELAY);
				break;
		};		
		memset(outMsg_t, 0, sizeof(outMsg_t));
		memset(&inMsg_t, 0, sizeof(inMsg_t));
	}
}

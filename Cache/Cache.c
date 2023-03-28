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
	uint8_t cache_patt = 0;
	while(1)
	{
		xQueueReceive( CacheQueue, &inMsg_t, portMAX_DELAY);
		switch(inMsg_t.task)
		{
			case EInANALOG:
				cache_analog = inMsg_t.Value.voltage;
				break;
			
			case EInPATTRN:
				cache_patt = inMsg_t.Value.pattern;
				break;
			
			case EOutANALOG:
				snprintf((char *)outMsg_t,20,"<AN0>-Value: %0.2f V",cache_analog);
				xQueueSendToBack(UartTxQueue, &outMsg_t, portMAX_DELAY);
				break;
			
			case EOutPATTRN:
				snprintf((char *)outMsg_t,20,"<PAT>-Hex: %x",cache_patt);
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

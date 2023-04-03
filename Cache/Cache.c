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
	float cache_analog, cache_analog1 = 0;
	float cache_ACC[3] = {0};
	uint8_t cache_patt = 0;
	while(1)
	{
		// Retreive command message from RTOS queue
		xQueueReceive( CacheQueue, &inMsg_t, portMAX_DELAY);
		
		// Identify the command task
		switch(inMsg_t.task)
		{
			/**	EInXXXX are for saving data in cache						**/
			case EInANALOG: /*!< Store Analog 0 data		*/
				cache_analog = inMsg_t.Value.voltage;
				break;
			
		  case EInANALOG1:/*!< Store Analog 1 data		*/	
				cache_analog1 = inMsg_t.Value.voltage;
				break;
			
			case EInPATTRN:/*!< Store GPIO/Pattern data	*/
				cache_patt = inMsg_t.Value.pattern;
				break;
			
			case EInACC:	/*!< Store Accelerometer data	*/
				cache_ACC[0] = inMsg_t.Value.acc[0];
				cache_ACC[1] = inMsg_t.Value.acc[1];
				cache_ACC[2] = inMsg_t.Value.acc[2];
				break;
			
			/**	EOutXXXX request information from cache						**/
			case EOutANALOG:	/*!< Reply to Analog1 command			*/
				snprintf((char *)outMsg_t,20,"<AN0>-Value: %0.2f V",cache_analog);
				xQueueSendToBack(UartTxQueue, &outMsg_t, portMAX_DELAY);
				break;
			
			case EOutANALOG1:	/*!< Reply to Analog0 command			*/
				snprintf((char *)outMsg_t,20,"<AN1>-Value: %0.2f V",cache_analog1);
				xQueueSendToBack(UartTxQueue, &outMsg_t, portMAX_DELAY);
				break;
			
			case EOutPATTRN: /*!< Reply to GPIO/PATTERN command	*/
				snprintf((char *)outMsg_t,20,"<PAT>-Hex: %x",cache_patt);
				xQueueSendToBack(UartTxQueue, &outMsg_t, portMAX_DELAY);
				break;
			
		  case EOutACC:		/*!< Reply to Accelerometer command	*/
				snprintf((char *)outMsg_t,40,"<ACC>-XYZ: %0.2f %0.2f %0.2f",cache_ACC[0],cache_ACC[1],cache_ACC[2]);
				xQueueSendToBack(UartTxQueue, &outMsg_t, portMAX_DELAY);
				break;
			
			/**	If no command is identified.												**/
			default:
				snprintf((char *)outMsg_t,20,"Error in Cache");
				xQueueSendToBack(UartTxQueue, &outMsg_t, portMAX_DELAY);
				break;
		};		
		memset(outMsg_t, 0, sizeof(outMsg_t));
		memset(&inMsg_t, 0, sizeof(inMsg_t));
	}
}

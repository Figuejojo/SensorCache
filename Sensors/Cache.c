#include "Cache.h"

xTaskHandle hCache;
xQueueHandle CacheQueue;

extern xQueueHandle UartTxQueue;

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
			case ANALOG_e:
				cache_analog = inMsg_t.Value.voltage;
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

/** @file GPIO.c
 *  @brief Implementation file for GPIO functions and tasks.
 *
 *  @author Y3913624
 */

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ButLed.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/
xTaskHandle hGPIO;							/*!< RTOS handler for GPIO Task   */

extern xQueueHandle CacheQueue;	/*!< RTOS queue message, Cache 					 */

/*******************************************************************************
* Function Implementation
*******************************************************************************/

/**
	*	@name vCache
	* @Type	Task
*/
portTASK_FUNCTION(vGPIO, pvParameters)
{
	
	portTickType xLastWakeTime = xTaskGetTickCount();
	uint8_t ub_sequence = 0;
	uint8_t buttons;
	uint8_t Flag = 0;
	
	QCacheMsg_t Msg_t = 
	{
		.task = EInPATTRN,
		.Value.pattern = 0,
	};

	while(1)
	{
		
		GPIO_ResetBits(GPIOD,0xFF00);
		buttons = GPIO_ReadInputData(GPIOE)>>8;
		
		if(buttons!=0)
		{
			if((buttons&0x01)==1)
			{
				Flag = 0;
				ub_sequence = 0;
			}
			else if(buttons > 1)
			{
				Flag = 0xFE;
				ub_sequence = buttons;
			}
			Msg_t.Value.pattern = ub_sequence;
			xQueueSendToBack(CacheQueue, &Msg_t, portMAX_DELAY);
		}
		GPIO_SetBits(GPIOD, ub_sequence<<8);
		ub_sequence = Flag^ub_sequence;
		
		vTaskDelayUntil(&xLastWakeTime, (CYCLETIME_GPIO/portTICK_RATE_MS));
	}
}

void initLEDs(void)
{
	//GPIOD are the LEDs
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = 0xFF00;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void initBUTs(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = 0xFF00;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

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
xTaskHandle hGPIO;							/*!< RTOS handler for GPIO Task   			 */

extern xQueueHandle CacheQueue;	/*!< RTOS queue message, Cache 					 */

static volatile uint8_t svgFlag = 1;/*!< Retrieve change in Interrupt			 */
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
	uint8_t ub_sequence = 1;
	uint8_t buttons;
	
	QCacheMsg_t Msg_t = 
	{
		.task = EInPATTRN,
		.Value.pattern = 0,
	};

	while(1)
	{
		
		GPIO_ResetBits(GPIOD,0xFF00);
		buttons = GPIO_ReadInputData(GPIOE)>>8;
	
		if(svgFlag == 1 || buttons > 1)
		{
			if(svgFlag == 1)
			{
				svgFlag = 0;
				ub_sequence = 1;
			}
			else if(buttons > 1)
			{
				svgFlag = 0xFE;
				ub_sequence = buttons;			
			}
			Msg_t.Value.pattern = ub_sequence;
			xQueueSendToBack(CacheQueue, &Msg_t, portMAX_DELAY);
		}
		
		GPIO_SetBits(GPIOD, ub_sequence<<8);
		ub_sequence = svgFlag^ub_sequence;
		
		vTaskDelayUntil(&xLastWakeTime, (CYCLETIME_GPIO/portTICK_RATE_MS));
	}
}

/**
	*	@name initLEDs
	* @Type	Function
*/
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

/**
	*	@name initBUTs
	* @Type	Function
*/
void initBUTs(void)
{
	// Enable GPIOE for Buttons and SYSCFG for External INTs
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = 0xFF00; //Set all GPIO pins
	GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	// Set PE8 as an external interrupt
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource8);
	
	// Configure external interrupt
	EXTI_InitTypeDef EXTINT_t;
	EXTINT_t.EXTI_Line = EXTI_Line8;
	EXTINT_t.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTINT_t.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTINT_t.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTINT_t);
		
	// Configure the NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
}

/**
	*	@name USART2_IRQHandler
	* @Type	Interrupt Handler for PE8 (Button 1)
*/ 
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line8) != RESET) 
	{
		svgFlag = 1;	// Global Volatile and static variable
		EXTI_ClearITPendingBit(EXTI_Line8);
  }
}

/** @file Analog.c
 *  @brief Implementation file dedicated to getting, processing and sending 
 *					the Analog value from PC1.
 *
 *  @author Y3913624
 */

/*******************************************************************************
* Includes
*******************************************************************************/
#include "Analog.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
xTaskHandle hAnalog;
extern xQueueHandle CacheQueue;
	
/*******************************************************************************
* Function Implementation
*******************************************************************************/
	
/**
	*	@name vAnalog
	* @Type	Task
*/
portTASK_FUNCTION(vAnalog, pvParameters) 
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	QCacheMsg_t Msg_t;
	Msg_t.task = EInANALOG;
	while(1) 
	{
		Msg_t.task = EInANALOG;
		Msg_t.Value.voltage = ADC2VOLTS(getAnalog(EAN0));
		xQueueSendToBack( CacheQueue, &Msg_t, portMAX_DELAY);
		vTaskDelayUntil(&xLastWakeTime, (CYCLETIME_AN/portTICK_RATE_MS));
		
		Msg_t.task = EInANALOG1;
		Msg_t.Value.voltage = ADC2VOLTS(getAnalog(EAN1));
		xQueueSendToBack( CacheQueue, &Msg_t, portMAX_DELAY);
		vTaskDelayUntil(&xLastWakeTime, (CYCLETIME_AN/portTICK_RATE_MS));
		
	}
}

/**
	*	@name initAnalog
	* @Type	Function
*/
void initAnalog(void)
{
	// Analog for PC1 (GPIOC Pin 1 & GPIOC Pin 2 
	
	// GPIO Configuration and Initialization
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	//GPIO Configuration for GPIO C Pin1
	GPIO_InitTypeDef GpioPot_t = 
	{
		.GPIO_Mode = GPIO_Mode_AN,
		.GPIO_OType= GPIO_OType_PP,
		.GPIO_Pin  = GPIO_Pin_1,
		.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.GPIO_Speed= GPIO_Speed_100MHz
	};
	GPIO_Init(GPIOC, &GpioPot_t);
	
	//GPIO Configuration for GPIO C Pin 2
	GpioPot_t.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOC, &GpioPot_t);
	
	//ADC Configuration
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_CommonInitTypeDef AdcCommon_t = 
	{
		.ADC_Mode = ADC_Mode_Independent,
		.ADC_Prescaler = ADC_Prescaler_Div4,
	}; 
	ADC_CommonInit(&AdcCommon_t);
		
	ADC_InitTypeDef AdcPotConfig_t =
	{
		.ADC_Resolution = ADC_Resolution_8b
	};
	
	ADC_Init(ADC1, &AdcPotConfig_t);
	ADC_Cmd(ADC1,ENABLE);
	
}

/**
	*	@name getAnalog
	* @Type	Function
*/
uint16_t getAnalog(Analog_e Analog)
{
	if(Analog == EAN0)
	{
		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_56Cycles);
	}
	else if (Analog == EAN1)
	{
		ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_56Cycles);
	}
	else
	{
		return (uint16_t)EANERR;
	}
	
	ADC_SoftwareStartConv(ADC1);
	
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	
	return ADC_GetConversionValue(ADC1);
}

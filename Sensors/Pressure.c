#include "Pressure.h"

xTaskHandle hAnalog;
extern xQueueHandle CacheQueue;
	
// This task should run every 500ms and send a message to the print task.
// ---------------------------------------------------------------------------- 
portTASK_FUNCTION(vAnalog, pvParameters) 
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	QCacheMsg_t Msg_t;
	Msg_t.task = ANALOG_e;
	while(1) 
	{
		// *** Insert a message into the queue HERE
		Msg_t.Value.voltage = (3.3f * getPRES())/(255);
		xQueueSendToBack( CacheQueue, &Msg_t, portMAX_DELAY);
		vTaskDelayUntil(&xLastWakeTime, (1000/portTICK_RATE_MS));
	}
}

void initPRES(void)
{
	// Analog for PC1 (GPIOC Pin 1) 
	
	// GPIO Configuration and Initialization
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GpioPot1_t = 
	{
		.GPIO_Mode = GPIO_Mode_AN,
		.GPIO_OType= GPIO_OType_PP,
		.GPIO_Pin  = GPIO_Pin_1,
		.GPIO_PuPd = GPIO_PuPd_NOPULL,
		.GPIO_Speed= GPIO_Speed_100MHz
	};
	GPIO_Init(GPIOC, &GpioPot1_t);
	
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

uint16_t getPRES(void)
{
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_56Cycles);
	ADC_SoftwareStartConv(ADC1);
	
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	
	return ADC_GetConversionValue(ADC1);
}

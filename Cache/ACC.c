/** @file Uart.c
 *  @brief Implementation file for the accelerometer functions.
 *
 *  @author Y3913624
 */

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ACC.h"
#include "stm32f4_discovery_lis302dl.h"
/*******************************************************************************
* Static Global Variables
*******************************************************************************/
xTaskHandle hACC;									/*!< RTOS handler for Accelerometer		   */

extern xQueueHandle CacheQueue;	/*!< RTOS queue message, USART output    */

/*******************************************************************************
* Function Implementation
*******************************************************************************/

/**
	*	@name vACC
	* @Type	Task
*/
portTASK_FUNCTION(vACC, pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	QCacheMsg_t msg;
	msg.task = EInACC;
	axis_t accVals = {0};
	while(1)
	{
		getACC(&accVals);
		msg.Value.acc[0] = accVals.X;
		msg.Value.acc[1] = accVals.Y;
		msg.Value.acc[2] = accVals.Z;
		xQueueSendToBack( CacheQueue, &msg, portMAX_DELAY);
		vTaskDelayUntil(&xLastWakeTime, (CYCLETIMEACC/portTICK_RATE_MS));
	}	
}

/**
	*	@name  initPriphACC
	* @Type	 static function
	* @brief Function Initialize all the SPI GPIO required for Accelerometer
*/ 
static void initPriphACC(void)
{
	// PE0 & PE1 - INT1 & INT2
	// PE3 - CS, PA5 - SCK, PA7 - MOSI, PA6 - MISO 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);	// PA5 - SCK
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);	// PA6 - MISO
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);	// PA7 - MOSI
	
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI MISO pin configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	/* SPI configuration -------------------------------------------------------*/
  SPI_InitTypeDef  SPI_InitStructure;	
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_Init(SPI1, &SPI_InitStructure);
  SPI_Cmd(SPI1, ENABLE);	 /* Enable SPI1  */
	
	/* Configure GPIO PIN for Lis Chip select */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;					
	GPIO_Init(GPIOE, &GPIO_InitStructure);							//PE3 - CS
  GPIO_SetBits(GPIOE, GPIO_Pin_3);										/* Deselect : Chip Select high */
								
	/* Configure GPIO PINs to detect Interrupts */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);							//PE0 - INT1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_InitStructure);						 	//PE1 - INT2
}

/**
	*	@name initACC
	* @Type	Function
*/ 
void initACC(void)
{
	initPriphACC();
								// 	ODR 		 LPen   	 X				Y					Z
	uint8_t ctrl =  (2<<4) | (0<<3) | (1<<2) | (1<<1) | (1<<0);
	
	/* Write value to MEMS CTRL_REG1 regsister */
  LIS302DL_Write(&ctrl, LIS3DH_CTRL_REG1, 1);
	LIS302DL_Read(&ctrl,  LIS3DH_CTRL_REG4, 1);
	
}

/**
	*	@name getACC
	* @Type	Function
*/ 
void getACC(axis_t * ACCx) //Default is +/-2g, Normal Mode (No test),  
{
	uint8_t axisLow, axisHigh = 0;
	LIS302DL_Read(&axisLow,  LIS3DH_OUTX_L,1);
	LIS302DL_Read(&axisHigh, LIS3DH_OUTX_H,1);
	ACCx->X = ((axisLow)+(axisHigh<<8)*SENS);
	
	axisLow = 0;
	axisHigh = 0;
	LIS302DL_Read(&axisLow,  LIS3DH_OUTY_L,1);
	LIS302DL_Read(&axisHigh, LIS3DH_OUTY_H,1);
	ACCx->Y = ((axisLow)+(axisHigh<<8)*SENS);
	
	axisLow = 0;
	axisHigh = 0;
	LIS302DL_Read(&axisLow,  LIS3DH_OUTZ_L,1);
	LIS302DL_Read(&axisHigh, LIS3DH_OUTZ_H,1);
	ACCx->Z = ((axisLow)+(axisHigh<<8)*SENS);
}

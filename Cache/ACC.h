/** @file ACC.h
 *  @brief Header file for the accelerometer
 *
 *  @author Y3913624
 */
 
#ifndef _ACC_H_
#define _ACC_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "CacheDTypes.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define CYCLETIMEACC  (1000)			/*!< Amount of time for the Task delay    	 */

//LIS3DH Commands (Datasheet)
#define LIS3DH_WHO_AM_I  (0x0F)		
#define LIS3DH_CTRL_REG0 (0x1E)
#define LIS3DH_CTRL_REG1 (0x20)
#define LIS3DH_CTRL_REG2 (0x21)
#define LIS3DH_CTRL_REG3 (0x22)
#define LIS3DH_CTRL_REG4 (0x23)
#define LIS3DH_CTRL_REG5 (0x24)
#define LIS3DH_CTRL_REG6 (0x25)

#define LIS3DH_OUTX_L  (0x28) 
#define LIS3DH_OUTX_H  (0x29) 
#define LIS3DH_OUTY_L  (0x2A) 
#define LIS3DH_OUTY_H  (0x2B) 
#define LIS3DH_OUTZ_L  (0x2C) 
#define LIS3DH_OUTZ_H  (0x2D) 

//LIS3DH CS Line Control
#define LIS3DH_CS_LOW()		GPIO_ResetBits(GPIOE, GPIO_Pin_3)
#define LIS3DH_CS_HIGH()	GPIO_SetBits(GPIOE, GPIO_Pin_3)

//LIS3DH Sensitivity
#define SENS	 (0.061f)

/*******************************************************************************
* Type definitions
*******************************************************************************/

/**
 * @name TASK_e
 * @Type struct
 * @brief This structure is used to collect the new values from the SPI line.
 */
typedef struct
{
/*@{*/
	float	X;	/*!< For X axis value */
	float Y;	/*!< For Y axis value */
	float Z;	/*!< For Z axis value */
/*@}*/
}axis_t;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/** @name 	vACC Task
	*	@brief 	Get and send information gather from the Accelerometer to the cache.
  *						
  * @param 	Void
  */
portTASK_FUNCTION_PROTO(vACC, pvParameters);

/** @name 	initACC
	*	@brief 	Initializr GPIO and SPI peripherals and configure the Accelerometer
  *
  * @param 	Void
  */
uint8_t initACC(void);

/** @name 	getACC
	*	@brief 	Get the X,Y and Z raw data from the accelerometer. 
	*						- Unify the LSB and MSB
  *
  * @param 	axis_t pointer @see axis_t
  */
void getACC(axis_t * ACCx);

#endif	//_ACC_H_

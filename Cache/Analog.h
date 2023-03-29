/** @file Analog.h
 *  @brief Header file for the Analog functions.
 *					Dedicated to getting, processing and sending the Analog value from PC1.
 *
 *  @author Y3913624
 */

#ifndef _PRES_H_
#define _PRES_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "CacheDTypes.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define CYCLETIME_AN (1000)	/*!< Amount of time for the Task delay              	 */
#define VREF			   (3.3f)	/*!< Reference voltage set as a 3.3 float value     	 */
#define RES			      (255)	/*!< Analog Resolution value (2^n) currently 8 bit  	 */

#define ADC2VOLTS(x) ((VREF*x)/RES)	/*!< Macro function to converting adc to volts */

/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * @name Analog_e
 * @Type enum
 * @brief This enumerator is to distinguished from the Analog to be read.
 */
typedef enum
{
	EANERR = -1,	/*!< Error / No Supported 								*/
	EAN0	 =  0,	/*!< Analog Zero or Potentiometer 1			  */
	EAN1,					/*!< Analog One or Potentiometer 2			  */
	
	EANDEND,			/*!< END of the enumerator value				  */
}Analog_e;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/** @name 	vAnalog Task
	*	@brief 	Collect data from PC1, process it and send the value to the cache, every @see CYCLETIME 
  *
  * @param 	Void
  */
portTASK_FUNCTION_PROTO(vAnalog, pvParameters);

/** @name		initAnalog
	*	@brief 	Initialize and configure the GPIO and Analog so that PC1 is ready to be use.
	*						8bit resolution... (Todo) 
  *
  * @param 	Void.
  */
void initAnalog(void);

/** @name		getAnalog
	*	@brief 	Retreive the analog value by starting the conversion and waiting for it to be ready.
	*						It is static so it can only be used by the Analog.c file.
  *
  * @param 	Analog - To chose which Analog should be used. @see Analog_e
	* @return uint16_t value (Must be converted to voltage)
  */
static uint16_t getAnalog(Analog_e Analog);

#endif //_PRES_H_

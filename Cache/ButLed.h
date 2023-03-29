#ifndef _BUTLED_H_
#define _BUTLED_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "CacheDTypes.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define CYCLETIME_GPIO (2000)	/*!< Amount of time for the Task delay        	*/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/** @name 	vGPIO Task
	*	@brief  Save the value of the pressed buttons, alternate the LED value depending 
	*						on the saved value. This goes from LED 2 to 9 (PD9 to PD15) and Buttons
	*						2 to 9 (PE9 to PE15).
	*					Button 1 (PD8) is for resetting the pattern and turn the first led on (PD8)
  *					Whenever the pattern is changed the new pattern is send to cache with the queue.
	*
  * @param 	Void
  */
portTASK_FUNCTION_PROTO(vGPIO , pvParameters);

/** @name		initLEDs
	*	@brief 	Initialize the Buttons and set Interrupt for Button 1 (pin8)
	*						- Turn on RCC for GPIOE and SYSCFG
  *						- Set GPIO for pins 8 to 15
	*						- Set interrupt service for PE8.
  * @param 	Void
  */
void initLEDs(void);

/** @name		initBUTs
	*	@brief 	Initialize the LEDs
	*						- Turn on RCC for GPIOD
  *						- Set GPIO for pins 8 to 15
  * @param 	Void
  */
void initBUTs(void);

#endif //_BUTLED_H_

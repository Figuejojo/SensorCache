#ifndef _BUTLED_H_
#define _BUTLED_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "CacheDTypes.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define CYCLETIME (5000)

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/** @name 	vGPIO Task
	*	@brief  Eeach Button has a different led sequence. 
  *
  * @param 	Void
  */
portTASK_FUNCTION_PROTO(vGPIO , pvParameters);

void initLEDs(void);

void initBUTs(void);

#endif //_BUTLED_H_

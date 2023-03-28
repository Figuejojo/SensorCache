/** @file Cache.h
 *  @brief Header file for the Cache functions and tasks.
 *					This cache wait for an incomming queue message which may:
 *						- Save value from a specific sensor into the cache
 *						- Send value for the request sensor.
 *
 *  @author Y3913624
 */
#ifndef _CACHE_H_
#define _CACHE_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "CacheDTypes.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/


/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/** @name 	vCache Task
	*	@brief 	Collect information from incoming message queue and store or send 
	*						sensor information.
  *
  * @param 	Void
  */
portTASK_FUNCTION_PROTO(vCache , pvParameters);

#endif //_CACHE_H_

/** @file CacheDTypes.h
 *  @brief Common Header file for the message communication with the Cache. 
 *
 *  @author Y3913624
 */
 
#ifndef _CACHEDTYPES_H_
#define _CACHEDTYPES_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stm32f4xx.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*******************************************************************************
* Type definitions
*******************************************************************************/

/**
 * @name TASK_e
 * @Type enum
 * @brief This enumerator is to distinguished the incomming message:
 *					- Sensor are inputs (save data)
 *					- Rquests are ouputs(retreive data)
 */
typedef enum
{
/*@{*/
	NONE_e 			= -1,		/*!< Error State */

	EOutANALOG  =  0,		/*!< Requested Analog0 value 					  */
	EOutANALOG1,				/*!< Requested Analog1 value 					  */
	EOutPATTRN,					/*!< Requested LEDPattern	   					  */
	EOutACC,						/*!< Requested Accelerometer value		  */
	
	EInANALOG,					/*!< Incoming Analog0 value to be stored*/
	EInANALOG1,					/*!< Incoming Analog1 value to be stored*/
	EInPATTRN,					/*!< Incoming LEDPattern to be stored	  */
	EInACC,							/*!< Incoming Accelerometer to be stored*/
	
	ETASKEND,						/*!< End of TASK enum values						*/
/*@}*/
}TASK_e;

/**
 * @name MsgCache_t
 * @brief Queue message for cache to collect different datatypes
 */
typedef union
{
/*@{*/
	float voltage;		/*!< Voltage for analog conversion value  */
	uint8_t pattern;	/*!< Current LED Pattern (HEX value)		  */
	float acc[3];			/*!< Values for ACC X Y and Z						  */
/*@{*/	
}MsgCache_t;

/**
 * @name StateMachine
 * @brief State Machine for the Game Engine.
 */
typedef struct
{
/*@{*/
	TASK_e task;			/*!< Where is the incomming message @see TASK_e */
	MsgCache_t Value; /*!< Message Value @see MsgCache_t							*/
/*@{*/
}QCacheMsg_t;

#endif // _CACHEDTYPES_H_

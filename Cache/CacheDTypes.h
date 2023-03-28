#ifndef _CACHEDTYPES_H_
#define _CACHEDTYPES_H_

#include "stm32f4xx.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef enum
{
	NONE_e = -1,
	inANALOG_e = 0,
	outANALOG_e,
}TASK_e;

typedef union
{
	char msg[10];
	float voltage;	
}MsgCache_t;

typedef struct
{
	TASK_e task;
	MsgCache_t Value;
}QCacheMsg_t;

#endif

#ifndef _PRES_H_
#define _PRES_H_

#include "CacheDTypes.h"

portTASK_FUNCTION_PROTO(vAnalog, pvParameters);

void initPRES(void);

uint16_t getPRES(void);

#endif

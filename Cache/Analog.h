#ifndef _PRES_H_
#define _PRES_H_

#include "CacheDTypes.h"

portTASK_FUNCTION_PROTO(vAnalog, pvParameters);

void initAnalog(void);

static uint16_t getAnalog(void);

#endif

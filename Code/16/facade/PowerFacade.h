#pragma once

#include "../common/PowerTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

void PowerFacade_Init(void);
void PowerFacade_SetMode(PowerMode mode);
void PowerFacade_ShowStatus(void);
void PowerFacade_RunTask(const char *name, int workload);
void PowerFacade_Shutdown(void);

#ifdef __cplusplus
}
#endif

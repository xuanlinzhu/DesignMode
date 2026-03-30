#include "facade/PowerFacade.h"

extern int puts(const char *str);

int main(void)
{
    PowerFacade_Init();
    PowerFacade_ShowStatus();
    PowerFacade_RunTask("Boot Task", 60);

    puts("\n-> PERFORMANCE");
    PowerFacade_SetMode(POWER_MODE_PERFORMANCE);
    PowerFacade_RunTask("AI Task", 100);

    puts("\n-> LOW_POWER");
    PowerFacade_SetMode(POWER_MODE_LOW_POWER);
    PowerFacade_RunTask("Sync Task", 100);

    puts("\n-> STANDARD");
    PowerFacade_SetMode(POWER_MODE_STANDARD);
    PowerFacade_ShowStatus();

    puts("\n-> OFF");
    PowerFacade_SetMode(POWER_MODE_OFF);
    PowerFacade_RunTask("Should Fail", 20);

    PowerFacade_Shutdown();
    return 0;
}

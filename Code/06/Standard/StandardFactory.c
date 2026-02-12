#include "StandardFactory.h"
#include "SevenSegDisplay.h"
#include "DigitalTempSensor.h"
#include "UartComm.h"
#include "PwmFan.h"

ModuleCreators StandardModuleCreators = {
    .versionName = "Standard",
    .createDisplay = createSevenSegDisplay,
    .createSensor = createDigitalTempSensor,
    .createComm = createUartComm,
    .createActuator = createPwmFan
};

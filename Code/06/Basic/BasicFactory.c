#include "BasicFactory.h"
#include "LedDisplay.h"
#include "AnalogTempSensor.h"
#include "ButtonControl.h"
#include "FixedFan.h"

ModuleCreators BasicModuleCreators = {
    .versionName = "Basic",
    .createDisplay = createLedDisplay,
    .createSensor = createAnalogTempSensor,
    .createComm = createButtonControl,
    .createActuator = createFixedFan
};

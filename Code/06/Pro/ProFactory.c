#include "ProFactory.h"
#include "LcdDisplay.h"
#include "HighPrecTempSensor.h"
#include "WifiComm.h"
#include "HighPerfFan.h"

ModuleCreators ProModuleCreators = {
    .versionName = "Pro",
    .createDisplay = createLcdDisplay,
    .createSensor = createHighPrecTempSensor,
    .createComm = createWifiComm,
    .createActuator = createHighPerfFan
};

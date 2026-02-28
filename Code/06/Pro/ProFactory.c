#include "ProFactory.h"
#include "LcdDisplay.h"
#include "HighPrecTempSensor.h"
#include "WifiComm.h"
#include "HighPerfFan.h"

/* Pro 版本对象族：绑定高配模块创建函数 */
ModuleCreators ProModuleCreators = {
    .versionName = "Pro",
    .createDisplay = createLcdDisplay,
    .createSensor = createHighPrecTempSensor,
    .createComm = createWifiComm,
    .createActuator = createHighPerfFan
};

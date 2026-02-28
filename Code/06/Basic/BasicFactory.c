#include "BasicFactory.h"
#include "LedDisplay.h"
#include "AnalogTempSensor.h"
#include "ButtonControl.h"
#include "FixedFan.h"

/* Basic 版本对象族：绑定显示/传感/通信/执行器的具体创建函数 */
ModuleCreators BasicModuleCreators = {
    .versionName = "Basic",
    .createDisplay = createLedDisplay,
    .createSensor = createAnalogTempSensor,
    .createComm = createButtonControl,
    .createActuator = createFixedFan
};

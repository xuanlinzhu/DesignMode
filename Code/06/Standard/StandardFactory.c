#include "StandardFactory.h"
#include "SevenSegDisplay.h"
#include "DigitalTempSensor.h"
#include "UartComm.h"
#include "PwmFan.h"

/* Standard 版本对象族：绑定该版本全部模块创建函数 */
ModuleCreators StandardModuleCreators = {
    .versionName = "Standard",
    .createDisplay = createSevenSegDisplay,
    .createSensor = createDigitalTempSensor,
    .createComm = createUartComm,
    .createActuator = createPwmFan
};

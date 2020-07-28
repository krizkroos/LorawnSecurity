#include "batterydepletion.h"
#include "Utils/logger.h"

BatteryDepletion::BatteryDepletion()
{

}

Lorawan_result BatteryDepletion::stop()
{

}

Lorawan_result BatteryDepletion::launch()
{
    writeLog(Logger::BatteryDepletion, "Launching battery attack");

    return Lorawan_result::Success;
}

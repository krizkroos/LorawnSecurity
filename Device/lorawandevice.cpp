#include "lorawandevice.h"

LorawanDevice::LorawanDevice(lorawanVersion ver)
{
    version = ver;
}

void LorawanDevice::setDevEUI(const bytes &value)
{
    devEUI = value;
}

void LorawanDevice::setAppEUI(const bytes &value)
{
    appEUI = value;
}

bytes LorawanDevice::getEui64() const
{
    return eui64;
}

void LorawanDevice::setEui64(const bytes &value)
{
    eui64 = value;
}

Lorawan_result LorawanDevice::isValid()
{
    if(devClass != DeviceClass::A)
        return Lorawan_result::Error;
    
    if((version.major != 1) && (version.minor != 0)
            && (version.patch != 2))
    {
        return Lorawan_result::Error;
    }

    return Lorawan_result::Success;
}

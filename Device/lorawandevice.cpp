#include "lorawandevice.h"

LorawanDevice::LorawanDevice()
{

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

LorawanDevice::LorawanDevice(lorawanVersion ver)
{
    version = ver;
}

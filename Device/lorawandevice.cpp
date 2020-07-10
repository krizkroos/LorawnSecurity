#include "lorawandevice.h"

bytes LorawanDevice::getDevEUI() const
{
    return devEUI;
}

void LorawanDevice::setDevEUI(const bytes &value)
{
    devEUI = value;
}

bytes LorawanDevice::getAppEUI() const
{
    return appEUI;
}

void LorawanDevice::setAppEUI(const bytes &value)
{
    appEUI = value;
}

bytes LorawanDevice::getMagicFour() const
{
    return magicFour;
}

void LorawanDevice::setMagicFour(const bytes &value)
{
    magicFour = value;
}

bytes LorawanDevice::getEui64() const
{
    return eui64;
}

void LorawanDevice::setEui64(const bytes &value)
{
    eui64 = value;
}

Tins::IP LorawanDevice::getDestinationAddress() const
{
    return destinationAddress;
}

void LorawanDevice::setDestinationAddress(const Tins::IP &value)
{
    destinationAddress = value;
}

uint16_t LorawanDevice::getDstPort() const
{
    return dstPort;
}

void LorawanDevice::setDstPort(const uint16_t &value)
{
    dstPort = value;
}

uint16_t LorawanDevice::getSrcPort() const
{
    return srcPort;
}

void LorawanDevice::setSrcPort(const uint16_t &value)
{
    srcPort = value;
}

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

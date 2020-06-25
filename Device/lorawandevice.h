#ifndef LORAWANDEVICE_H
#define LORAWANDEVICE_H

#include "defineLorawan.h"


enum class DeviceClass
{
    A,
    B,
    C
};

typedef struct lorawanVersion
{
    byte major;
    byte minor;
    byte patch;

} lorawanVersion;

class LorawanDevice
{
private:
    bytes devEUI;
    bytes appEUI;
    DeviceClass devClass = DeviceClass::A;
    lorawanVersion version;
    int frameCounter = 0;
public:
    LorawanDevice();
    Lorawan_result isValid();
    LorawanDevice(lorawanVersion ver);
};

#endif // LORAWANDEVICE_H


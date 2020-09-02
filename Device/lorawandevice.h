#ifndef LORAWANDEVICE_H
#define LORAWANDEVICE_H

#include "defineLorawan.h"
#include "tins/tins.h"

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
protected:
    bytes devEUI;
    bytes appEUI;

    DeviceClass devClass = DeviceClass::A;
    lorawanVersion version;
    int frameCounter = 0;

    bytes magicFour;
    bytes eui64;
    Tins::IP destinationAddress;
    uint16_t dstPort;
    uint16_t srcPort;
public:
    LorawanDevice() = default;
    LorawanDevice(lorawanVersion ver);
    Lorawan_result isValid();

    void setDevEUI(const bytes &value);
    void setAppEUI(const bytes &value);
    bytes getEui64() const;
    void setEui64(const bytes &value);

};

#endif // LORAWANDEVICE_H


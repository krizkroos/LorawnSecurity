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
    LorawanDevice();
    Lorawan_result isValid();
    LorawanDevice(lorawanVersion ver);
    bytes getDevEUI() const;
    void setDevEUI(const bytes &value);
    bytes getAppEUI() const;
    void setAppEUI(const bytes &value);
    bytes getMagicFour() const;
    void setMagicFour(const bytes &value);
    bytes getEui64() const;
    void setEui64(const bytes &value);
    Tins::IP getDestinationAddress() const;
    void setDestinationAddress(const Tins::IP &value);
    uint16_t getDstPort() const;
    void setDstPort(const uint16_t &value);
    uint16_t getSrcPort() const;
    void setSrcPort(const uint16_t &value);
};

#endif // LORAWANDEVICE_H


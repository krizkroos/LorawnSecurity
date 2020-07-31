#ifndef BATTERYDEPLETION_H
#define BATTERYDEPLETION_H

#include "lorawantest.h"
#include "packetstorage.h"

class BatteryDepletion  : public LorawanTest
{
private:
    SendController downlink;
    Lorawan_result setUpSending(std::shared_ptr<LorawanPacket> packet);
    Lorawan_result sendExtraPacket(std::shared_ptr<DataPacket> downlinkPacket);
    Lorawan_result send(bytes magicFour, bytes eui64, std::string json);
public:
    BatteryDepletion();
    virtual Lorawan_result stop() override;
    virtual Lorawan_result launch() override;
};

#endif // BATTERYDEPLETION_H

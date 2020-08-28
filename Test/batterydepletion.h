#ifndef BATTERYDEPLETION_H
#define BATTERYDEPLETION_H

#include "lorawantest.h"
#include "packetstorage.h"

class BatteryDepletion  : public LorawanTest
{
private:
    SendController downlink;
    int _downlinkCounter;
    Lorawan_result setUpSending(std::shared_ptr<LorawanPacket> packet);
    Lorawan_result sendExtraDownlinkPacket(DataPacket &copyPacket);
    Lorawan_result send(bytes magicFour, std::string json);
public:
    BatteryDepletion() = default;
    BatteryDepletion(int downlinkCounter);
    virtual Lorawan_result launch() override;
};

#endif // BATTERYDEPLETION_H

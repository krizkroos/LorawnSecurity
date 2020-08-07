#ifndef BRUTEFORCINGMIC_H
#define BRUTEFORCINGMIC_H

#include "lorawantest.h"
#include "packetstorage.h"

#include <vector>

class BruteforcingMIC : public LorawanTest
{
public:
    BruteforcingMIC();
    std::vector<std::vector<unsigned char>> micValues;
    virtual Lorawan_result launch() override;

private:
    SendController uplink;
    Lorawan_result setUpSending(std::shared_ptr<LorawanPacket> packet);
    Lorawan_result sendGuardPacket(std::shared_ptr<DataPacket> dataPkt);
    Lorawan_result send(bytes magicFour, bytes eui64, std::string json);
    Lorawan_result sendDeathPacket(std::shared_ptr<DataPacket> dataPkt);
};

#endif // BRUTEFORCINGMIC_H

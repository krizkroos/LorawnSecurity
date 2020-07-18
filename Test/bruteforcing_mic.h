#ifndef BRUTEFORCINGMIC_H
#define BRUTEFORCINGMIC_H

#include "lorawantest.h"
#include "packetstorage.h"


class BruteforcingMIC : public LorawanTest
{
public:
    BruteforcingMIC();
    const unsigned long int MAX_FCNT_GAP = 400;
    virtual Lorawan_result stop() override;
    virtual Lorawan_result launch() override;


private:
    Lorawan_result printPackets();
    Lorawan_result setUpSending(std::shared_ptr<LorawanPacket> requestPkt);
    Lorawan_result sendGuardPacket(std::shared_ptr<DataPacket> dataPkt);
    Lorawan_result send(bytes magicFour, bytes eui64, std::string json);
    Lorawan_result createJsonToSend(bytes rawPacket, std::string refJson, std::string &jsonToSend);
    Lorawan_result calculateMIC(DataPacket &dataPkt, bool changeFCnt = false);
    Lorawan_result sendDeathPacket(std::shared_ptr<DataPacket> dataPkt);
};

#endif // BRUTEFORCINGMIC_H

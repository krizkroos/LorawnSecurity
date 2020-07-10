#ifndef BRUTEFORCINGMIC_H
#define BRUTEFORCINGMIC_H

#include "lorawantest.h"


class BruteforcingMIC : public LorawanTest
{
public:
    BruteforcingMIC();
    virtual Lorawan_result stop() override;
    virtual Lorawan_result launch() override;

private:
    Lorawan_result printPackets();
    Lorawan_result send(bytes magicFour, bytes eui64, std::string json);
    Lorawan_result createJsonToSend(bytes rawPacket, std::string refJson, std::string &jsonToSend);
};

#endif // BRUTEFORCINGMIC_H

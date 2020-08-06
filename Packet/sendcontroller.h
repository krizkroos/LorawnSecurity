#ifndef UPLINKCONTROLLER_H
#define UPLINKCONTROLLER_H

#include "tins/tins.h"
#include "defineLorawan.h"

class SendController
{
private:
    Tins::PacketSender sender;
    Tins::IP packetIP;
    bytes magicFour;
    uint16_t dstPort;
    uint16_t srcPort;
    bytes eui64;
public:
    SendController();
    Lorawan_result send(std::string rawData);

    void setIP(const Tins::IP &value);
    void setMagicFour(const bytes &value);
    void setDstPort(const uint16_t &value);
    void setSrcPort(const uint16_t &value);
    void setEui64(const bytes &value);
};

#endif // UPLINKCONTROLLER_H

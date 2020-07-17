#ifndef JOINREQUESTPACKET_H
#define JOINREQUESTPACKET_H

#include "lorawanpacket.h"
#include "tins/tins.h"

class JoinRequestPacket : public LorawanPacket
{
private:
    bytes appEUI;
    bytes devEUI;
    bytes devNonce;


    bytes eui64;
    uint16_t dstPort;
    uint16_t srcPort;

public:
    JoinRequestPacket();
    Lorawan_result deserialize();


    bytes getEui64() const;
    void setEui64(const bytes &value);
    uint16_t getDstPort() const;
    void setDstPort(const uint16_t &value);
    uint16_t getSrcPort() const;
    void setSrcPort(const uint16_t &value);
    bytes getAppEUI() const;
    void setAppEUI(const bytes &value);
    bytes getDevEUI() const;
    void setDevEUI(const bytes &value);
};

#endif // JOINREQUESTPACKET_H

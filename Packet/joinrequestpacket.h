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

public:
    JoinRequestPacket();
    Lorawan_result deserialize();


    bytes getAppEUI() const;
    void setAppEUI(const bytes &value);
    bytes getDevEUI() const;
    void setDevEUI(const bytes &value);
};

#endif // JOINREQUESTPACKET_H

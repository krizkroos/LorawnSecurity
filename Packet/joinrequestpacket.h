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
    JoinRequestPacket(const std::shared_ptr<JoinRequestPacket> packet);
    Lorawan_result deserialize();
    Lorawan_result serialize();

    bytes getAppEUI() const;
    void setAppEUI(const bytes &value);
    bytes getDevEUI() const;
    void setDevEUI(const bytes &value);

    bytes getDevNonce() const;
    void setDevNonce(const bytes &value);
};

#endif // JOINREQUESTPACKET_H

#ifndef JOINREQUESTPACKET_H
#define JOINREQUESTPACKET_H

#include "lorawanpacket.h"

class JoinRequestPacket : public LorawanPacket
{
private:
    bytes appEUI;
    bytes devEUI;
    bytes devNonce;

public:
    JoinRequestPacket();
};

#endif // JOINREQUESTPACKET_H

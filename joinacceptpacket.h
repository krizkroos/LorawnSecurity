#ifndef JOINACCEPTPACKET_H
#define JOINACCEPTPACKET_H

#include "lorawanpacket.h"

class JoinAcceptPacket : public LorawanPacket
{
private:
    bytes devAddr;
    bytes netID;
    bytes appNonce;
public:
    JoinAcceptPacket();
};

#endif // JOINACCEPTPACKET_H

#ifndef LORAWANPACKET_H
#define LORAWANPACKET_H

#include "defineLorawan.h"

enum class MsgType
{
    JoinRequest,
    JoinAccept,
    MACPayload
};

class LorawanPacket
{
private:
    MsgType _type;

public:
    LorawanPacket();
    LorawanPacket(MsgType type);
    LorawanPacket deserializePacket(bytes data);
    bytes serializePacket();
};

#endif // LORAWANPACKET_H

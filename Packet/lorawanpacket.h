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
    bytes rawData;

public:
    LorawanPacket();
    LorawanPacket(MsgType type);
    bytes serializePacket();
    bytes getRawData() const;
    void setRawData(const bytes &value);
    MsgType getType() const;
};

#endif // LORAWANPACKET_H

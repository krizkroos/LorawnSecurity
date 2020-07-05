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
protected:
    MsgType _type;
    bytes rawData;
    bytes MIC;

public:
    LorawanPacket();
    LorawanPacket(MsgType type);

    bytes serializePacket();
    bytes getRawData() const;
    void setRawData(const bytes &value);
    MsgType getType() const;
};

#endif // LORAWANPACKET_H

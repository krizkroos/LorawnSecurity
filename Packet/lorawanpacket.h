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
    bytes rawPacket;
    std::string jsonString;
    bytes MIC;
    bytes magicFour;

public:
    LorawanPacket();
    LorawanPacket(MsgType type);
    LorawanPacket(const std::shared_ptr<LorawanPacket> packet);

    bytes serializePacket();
    bytes getRawData() const;
    void setRawData(const bytes &value);
    MsgType getType() const;

    std::string getJsonString() const;
    void setJsonString(const std::string &value);
    void setMagicFour(const bytes &value);
    bytes getMagicFour() const;
    bytes getMIC() const;
    void setMIC(const bytes &value);
};

#endif // LORAWANPACKET_H

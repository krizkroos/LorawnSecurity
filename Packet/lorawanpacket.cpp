#include "lorawanpacket.h"

MsgType LorawanPacket::getType() const
{
    return _type;
}

LorawanPacket::LorawanPacket()
{

}

LorawanPacket::LorawanPacket(MsgType type)
{
    _type = type;
}

bytes LorawanPacket::getRawData() const
{
    return rawData;
}

void LorawanPacket::setRawData(const bytes &value)
{
    rawData = value;
}



bytes LorawanPacket::serializePacket()
{
    return bytes();
}

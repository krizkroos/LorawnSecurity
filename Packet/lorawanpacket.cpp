#include "lorawanpacket.h"

MsgType LorawanPacket::getType() const
{
    return _type;
}

std::string LorawanPacket::getJsonString() const
{
    return jsonString;
}

void LorawanPacket::setJsonString(const std::string &value)
{
    jsonString = value;
}

void LorawanPacket::setMagicFour(const bytes &value)
{
    magicFour = value;
}

bytes LorawanPacket::getMagicFour() const
{
    return magicFour;
}

LorawanPacket::LorawanPacket()
{
    
}

LorawanPacket::LorawanPacket(MsgType type)
{
    _type = type;
}

LorawanPacket::LorawanPacket(const std::shared_ptr<LorawanPacket> packet)
{
     _type = packet->_type;
    rawPacket= packet->rawPacket;
    jsonString= packet->jsonString;
    MIC= packet->MIC;
    magicFour= packet->magicFour;

}

bytes LorawanPacket::getRawData() const
{
    return rawPacket;
}

void LorawanPacket::setRawData(const bytes &value)
{
    rawPacket = value;
}



bytes LorawanPacket::serializePacket()
{
    return bytes();
}

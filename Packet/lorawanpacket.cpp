#include "lorawanpacket.h"
#include "Utils/logger.h"

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

bytes LorawanPacket::getMIC() const
{
    return MIC;
}

void LorawanPacket::setMIC(const bytes &value)
{
    MIC = value;
}

Tins::IP LorawanPacket::getPacketIP() const
{
    return packetIP;
}

bytes LorawanPacket::getEui64() const
{
    return eui64;
}

void LorawanPacket::setEui64(const bytes &value)
{
    eui64 = value;
}

uint16_t LorawanPacket::getSrcPort() const
{
    return srcPort;
}

void LorawanPacket::setSrcPort(const uint16_t &value)
{
    srcPort = value;
}

uint16_t LorawanPacket::getDstPort() const
{
    return dstPort;
}

void LorawanPacket::setDstPort(const uint16_t &value)
{
    dstPort = value;
}

LorawanPacket::LorawanPacket()
{
    
}

LorawanPacket::LorawanPacket(MsgType type)
{
    _type = type;
}

Lorawan_result LorawanPacket::setIP(std::string dstAddr, Tins::IP::Flags flags, uint16_t id, uint8_t tos, uint8_t ttl, bool convertToBigEndian)
{

    called(Logger::Packet);
    if(convertToBigEndian)
    {

    }
    else
    {
        packetIP = Tins::IP(dstAddr);
        packetIP.id(id);
        packetIP.tos(tos);
        packetIP.ttl(ttl);
        packetIP.flags(flags);
    }



    return Lorawan_result::Success;
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

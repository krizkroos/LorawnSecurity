#include "joinrequestpacket.h"
#include "Utils/common.h"

#include <iostream>





bytes JoinRequestPacket::getEui64() const
{
    return eui64;
}

void JoinRequestPacket::setEui64(const bytes &value)
{
    eui64 = value;
}

Tins::IP JoinRequestPacket::getDestinationAddress() const
{
    return destinationAddress;
}

void JoinRequestPacket::setDestinationAddress(std::string value, bool convertToBigEndian)
{
    std::string ipAddress{};
    if(convertToBigEndian)
    {

        //TODO Common::convertIPAddress(const bytes address, bytes &convertedAddr)
    }
    else
    {
        ipAddress =value;
    }


    destinationAddress = Tins::IP(ipAddress);
}

uint16_t JoinRequestPacket::getDstPort() const
{
    return dstPort;
}

void JoinRequestPacket::setDstPort(const uint16_t &value)
{
    dstPort = value;
}

uint16_t JoinRequestPacket::getSrcPort() const
{
    return srcPort;
}

void JoinRequestPacket::setSrcPort(const uint16_t &value)
{
    srcPort = value;
}

bytes JoinRequestPacket::getAppEUI() const
{
    return appEUI;
}

void JoinRequestPacket::setAppEUI(const bytes &value)
{
    appEUI = value;
}

bytes JoinRequestPacket::getDevEUI() const
{
    return devEUI;
}

void JoinRequestPacket::setDevEUI(const bytes &value)
{
    devEUI = value;
}

JoinRequestPacket::JoinRequestPacket() : LorawanPacket(MsgType::JoinRequest)
{

}

Lorawan_result JoinRequestPacket::deserialize()
{

    const byte joinRequestType = 0x00;

    if(rawPacket.empty())
        return Lorawan_result::InputSizeZero;

    if(rawPacket.front() != joinRequestType)
        return Lorawan_result::Error;

    bytes::iterator nextByte = rawPacket.begin() +1;

    bytes littleEndianAppEUI = bytes(nextByte, nextByte + 8);

    if(Common::convertToBigEndian(littleEndianAppEUI,appEUI) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2BigEndian;
    }

    nextByte += 8;

    bytes littleEndianDevEUI = bytes(nextByte, nextByte + 8);

    if(Common::convertToBigEndian(littleEndianDevEUI,devEUI) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2BigEndian;
    }

    return Lorawan_result::Success;


}

#include "joinrequestpacket.h"
#include "Utils/common.h"
#include "Utils/logger.h"

#include <iostream>


JoinRequestPacket::JoinRequestPacket() : LorawanPacket(MsgType::JoinRequest)
{

}

JoinRequestPacket::JoinRequestPacket(const std::shared_ptr<JoinRequestPacket> packet)  : LorawanPacket(packet)
{
    called(Logger::Packet);

    appEUI = packet->getAppEUI();
    devEUI = packet->getDevEUI();
    devNonce = packet->getDevNonce();
}

Lorawan_result JoinRequestPacket::deserialize()
{

    const byte joinRequestType = 0x00;

    called(Logger::Packet);

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

    nextByte += 8;

    bytes littleEndiandevNonce  = bytes(nextByte, nextByte + 2);

    if(Common::convertToBigEndian(littleEndiandevNonce,devNonce) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2BigEndian;
    }


    return Lorawan_result::Success;


}


Lorawan_result JoinRequestPacket::serialize()
{
    byte mhdr{0x60};
    if(rawPacket.empty())
    {
        writeLog(Logger::Packet,"raw packet is empty, default mhdr");
    }
    else
        mhdr = rawPacket.front();

    rawPacket.clear();

    rawPacket.emplace_back(mhdr);

    bytes littleEndianAppEUI{};
    if(Common::convertToLittleEndian(appEUI,littleEndianAppEUI) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2LittleEndian;
    }

    rawPacket.insert(rawPacket.begin()+1, littleEndianAppEUI.begin(), littleEndianAppEUI.end());

    bytes littleEndianDevEUI{};
    if(Common::convertToLittleEndian(devEUI,littleEndianDevEUI) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2LittleEndian;
    }

    rawPacket.insert(rawPacket.end(), littleEndianDevEUI.begin(), littleEndianDevEUI.end());

    bytes littleEndianDevNonce{};
    if(Common::convertToLittleEndian(devNonce,littleEndianDevNonce) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2LittleEndian;
    }

    rawPacket.insert(rawPacket.end(), littleEndianDevNonce.begin(), littleEndianDevNonce.end());


    return Lorawan_result::Success;
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

bytes JoinRequestPacket::getDevNonce() const
{
    return devNonce;
}

void JoinRequestPacket::setDevNonce(const bytes &value)
{
    devNonce = value;
    writeLog(Logger::Packet, "set up value of DevNonce");
    writeHexLog(Logger::Packet, devNonce);
}

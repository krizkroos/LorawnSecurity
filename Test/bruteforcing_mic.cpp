#include "bruteforcing_mic.h"
#include "packetstorage.h"
#include "Utils/common.h"
#include "Utils/jsonparser.h"

#include <iostream>

BruteforcingMIC::BruteforcingMIC()
{


}

Lorawan_result BruteforcingMIC::launch()
{
    PacketStorage *storage = PacketStorage::getInstance();

    std::vector<std::shared_ptr<JoinRequestPacket> > requests = storage->getRequestPacket();


    if(requests.size() > 0)
    {
        std::shared_ptr<JoinRequestPacket> firstReq = requests.front();
        testDevice->setAppEUI(firstReq->getAppEUI());
        testDevice->setDevEUI(firstReq->getDevEUI());
        testDevice->setEui64(firstReq->getEui64());

        uplink.setDstPort(firstReq->getDstPort());
        uplink.setSrcPort(firstReq->getSrcPort());

        uplink.setDestinationAddress(firstReq->getDestinationAddress());

        std::vector<std::shared_ptr<DataPacket> > macPayload = storage->getMacPayloadPacket();

        if(macPayload.size() > 0)
        {

            std::shared_ptr<DataPacket> firstData = macPayload.front();

            std::string jsonToSend{};
            DataPacket copyPacket(firstData);

            if(createJsonToSend(copyPacket.getRawData(),copyPacket.getJsonString(),jsonToSend)
                    != Lorawan_result::Success)
                return Lorawan_result::ErrorCreatingPacket;

            std::cout << "sending packet " << std::endl;

            if(send(copyPacket.getMagicFour(),testDevice->getEui64(), jsonToSend) != Lorawan_result::Success)
            {
                std:: cout << "error sending packet!" << std::endl;
            }


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

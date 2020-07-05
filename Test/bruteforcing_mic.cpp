#include "bruteforcing_mic.h"
#include "packetstorage.h"
#include "Utils/common.h"

#include <iostream>

BruteforcingMIC::BruteforcingMIC()
{

}

Lorawan_result BruteforcingMIC::launch()
{

    printPackets();

    return Lorawan_result::Success;
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

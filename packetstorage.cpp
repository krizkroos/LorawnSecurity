#include "packetstorage.h"
#include <iostream>

/* Null, because instance will be initialized on demand. */
PacketStorage* PacketStorage::instance = nullptr;

PacketStorage* PacketStorage::getInstance()
{
    if (instance == nullptr)
    {
        instance = new PacketStorage();
    }

    return instance;
}

PacketStorage::PacketStorage()
{}

Lorawan_result PacketStorage::addPacket(std::shared_ptr<JoinRequestPacket> packet)
{
    Lorawan_result rv = Lorawan_result::Success;

    if(packet->getType() == MsgType::JoinRequest)
    {
        request.emplace_back(packet);
        std::cout << "added request packet" << std::endl;
    }
    else
    {
        std::cout << "unknown packet type" << std::endl;
        rv = Lorawan_result::ErrorAddPacket;

    }

    return rv;
}

Lorawan_result PacketStorage::addPacket(std::shared_ptr<JoinAcceptPacket> packet)
{
    Lorawan_result rv = Lorawan_result::Success;

    if(packet->getType() == MsgType::JoinAccept)
    {
        accept.emplace_back(packet);
        std::cout << "added accept packet" << std::endl;
    }
    else
    {
        std::cout << "unknown packet type" << std::endl;
        rv = Lorawan_result::ErrorAddPacket;

    }

    return rv;
}

Lorawan_result PacketStorage::addPacket(std::shared_ptr<DataPacket> packet)
{
    Lorawan_result rv = Lorawan_result::Success;

    if(packet->getType() == MsgType::MACPayload)
    {
        macPayload.emplace_back(packet);
        std::cout << "added data packet" << std::endl;
    }
    else
    {
        std::cout << "unknown packet type" << std::endl;
        rv = Lorawan_result::ErrorAddPacket;

    }

    return rv;
}

std::vector<std::shared_ptr<JoinAcceptPacket> > PacketStorage::getAcceptPacket() const
{
    return accept;
}

std::vector<std::shared_ptr<DataPacket> > PacketStorage::getMacPayloadPacket() const
{
    return macPayload;
}

std::vector<std::shared_ptr<JoinRequestPacket> > PacketStorage::getRequestPacket() const
{
    return request;
}



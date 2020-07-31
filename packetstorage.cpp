#include "packetstorage.h"
#include "Utils/logger.h"

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
        writeLog(Logger::PacketData,"added request packet");
    }
    else
    {
        writeLog(Logger::PacketData,"unknown packet type");
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
        writeLog(Logger::PacketData,"added accept packet");
    }
    else
    {
        writeLog(Logger::PacketData,"unknown packet type");
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
        writeLog(Logger::PacketData,"added data packet");
    }
    else
    {
        writeLog(Logger::PacketData,"unknown packet type");
        rv = Lorawan_result::ErrorAddPacket;

    }

    return rv;
}

Lorawan_result PacketStorage::findFirstDownlink(std::shared_ptr<DataPacket> &packet)
{
    for(auto &pkt : macPayload)
    {
        if(pkt->getDirection() == "downlink")
        {
            packet = pkt;
            return Lorawan_result::Success;
        }
    }
   return Lorawan_result::NoValueAvailable;
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



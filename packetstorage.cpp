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
    called(Logger::PacketData);

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
    called(Logger::PacketData);

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
    called(Logger::PacketData);

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

Lorawan_result PacketStorage::findFirstDownlink(std::shared_ptr<DataPacket> &packet, bytes withDevAddr)
{
    called(Logger::PacketData);
    for(auto &pkt : macPayload)
    {
        if(pkt->getDirection() == "downlink")
        {
            if(withDevAddr.size() > 0)
            {
                if(pkt->isDevAddrSame(withDevAddr))
                {
                    packet = pkt;
                    writeLog(Logger::PacketData, "found packet with correct devAddr");
                    return Lorawan_result::Success;
                }
                else
                {
                    writeLog(Logger::PacketData, "Diffrent downlink devAddr");
                    return Lorawan_result::NoValueAvailable;
                }
            }
            else
            {
                packet = pkt;
                writeLog(Logger::PacketData, "found packet with correct devAddr");
                return Lorawan_result::Success;
            }

        }
    }
   return Lorawan_result::NoValueAvailable;
}

Lorawan_result PacketStorage::findLastUplink(std::shared_ptr<DataPacket> &packet, bytes withDevAddr)
{
    called(Logger::PacketData);
    std::vector<std::shared_ptr<DataPacket>>::reverse_iterator lastPkt = macPayload.rbegin();
    for(; lastPkt != macPayload.rend(); ++lastPkt)
    {
        if((*lastPkt)->getDirection() == "uplink")
        {
            if(withDevAddr.size() > 0)
            {
                if((*lastPkt)->isDevAddrSame(withDevAddr))
                {
                    packet = (*lastPkt);
                    return Lorawan_result::Success;
                }
                else
                {
                    writeLog(Logger::PacketData, "Diffrent uplink devAddr");
                    return Lorawan_result::NoValueAvailable;
                }
            }
            else
            {
                packet = (*lastPkt);
                return Lorawan_result::Success;
            }

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



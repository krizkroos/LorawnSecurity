#ifndef PACKETSTORAGE_H
#define PACKETSTORAGE_H

#include "Packet/joinrequestpacket.h"
#include "Packet/joinacceptpacket.h"
#include "Packet/datapacket.h"

class PacketStorage
{
private:
    static PacketStorage* instance;
    PacketStorage() = default;
    std::vector<std::shared_ptr<JoinRequestPacket>> request;
    std::vector<std::shared_ptr<JoinAcceptPacket>> accept;
    std::vector<std::shared_ptr<DataPacket>> macPayload;
public:  
    static PacketStorage* getInstance();
    static void deleteInstance();
    std::vector<std::shared_ptr<JoinRequestPacket> > getRequestPacket() const;
    std::vector<std::shared_ptr<JoinAcceptPacket> > getAcceptPacket() const;
    std::vector<std::shared_ptr<DataPacket> > getMacPayloadPacket() const;
    Lorawan_result addPacket(std::shared_ptr<JoinRequestPacket> packet);
    Lorawan_result addPacket(std::shared_ptr<JoinAcceptPacket> packet);
    Lorawan_result addPacket(std::shared_ptr<DataPacket> packet);
    Lorawan_result findFirstDownlink(std::shared_ptr<DataPacket> &packet, bytes withDevAddr = bytes());
    Lorawan_result findLastUplink(std::shared_ptr<DataPacket> &packet, bytes withDevAddr = bytes());
};

#endif // PACKETSTORAGE_H

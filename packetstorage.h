#ifndef PACKETSTORAGE_H
#define PACKETSTORAGE_H

#include "Packet/joinrequestpacket.h"
#include "Packet/joinacceptpacket.h"
#include "Packet/datapacket.h"

#include <memory>

class PacketStorage
{
private:
    std::vector<std::shared_ptr<JoinRequestPacket>> request;
    std::vector<std::shared_ptr<JoinAcceptPacket>> accept;
    std::vector<std::shared_ptr<DataPacket>> macPayload;
public:
    PacketStorage();
};

#endif // PACKETSTORAGE_H

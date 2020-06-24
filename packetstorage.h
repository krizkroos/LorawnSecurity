#ifndef PACKETSTORAGE_H
#define PACKETSTORAGE_H

#include "joinrequestpacket.h"
#include "joinacceptpacket.h"
#include "datapacket.h"

class PacketStorage
{
private:
    std::vector<JoinRequestPacket> request;
    std::vector<JoinAcceptPacket> accept;
    std::vector<DataPacket> macPayload;
public:
    PacketStorage();
};

#endif // PACKETSTORAGE_H

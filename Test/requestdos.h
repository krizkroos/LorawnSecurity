#ifndef REQUESTDOS_H
#define REQUESTDOS_H

#include "lorawantest.h"
#include "packetstorage.h"

class RequestDoS : public LorawanTest
{
public:
    RequestDoS();
    RequestDoS(int requestCounter);

    virtual Lorawan_result launch() override;


private:
    int _requestCounter = 1;
    SendController uplink;
    Lorawan_result sendNextJoinRequest(std::shared_ptr<JoinRequestPacket> packet);
    Lorawan_result setUpSending(std::shared_ptr<JoinRequestPacket> requestPkt);
    Lorawan_result send(bytes magicFour, bytes eui64, std::string json);
};

#endif // REQUESTDOS_H

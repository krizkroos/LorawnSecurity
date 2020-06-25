#ifndef MITMATTACK_H
#define MITMATTACK_H

#include "testprerequisite.h"
#include "lorawanpacket.h"

#include <tins/ip_address.h>

class MiTMAttack : public TestPrerequisite
{
private:
    Tins::IPv4Address victim;
    Tins::IPv4Address gateway;

public:
    MiTMAttack();
    virtual Lorawan_result start() override;
    virtual Lorawan_result stop() override;
    Lorawan_result arpSpoofing();
    Lorawan_result dnsSpoofing();
    Lorawan_result sendPacket(LorawanPacket packet);

};

#endif // MITMATTACK_H

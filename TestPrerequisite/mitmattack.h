#ifndef MITMATTACK_H
#define MITMATTACK_H

#include "testprerequisite.h"
#include "Packet/lorawanpacket.h"

#include "tins/tins.h"


class MiTMAttack : public TestPrerequisite
{
private:
    static bool deserializePacket(const Tins::Packet &packet);

public:
    MiTMAttack();
    virtual Lorawan_result start() override;
    virtual Lorawan_result stop() override;

    Lorawan_result sniffing(std::string interface, std::string filter);
    Lorawan_result arpSpoofing();
    Lorawan_result dnsSpoofing();
    Lorawan_result sendPacket(LorawanPacket packet);

};

#endif // MITMATTACK_H

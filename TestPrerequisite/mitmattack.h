#ifndef MITMATTACK_H
#define MITMATTACK_H

#include "testprerequisite.h"
#include "Packet/lorawanpacket.h"
#include "packetstorage.h"

#include "tins/tins.h"


enum class SniffingPackets
{
    Data,
    Request,
    Accept
};

class MiTMAttack : public TestPrerequisite
{
private:

    std::string _filter;
    std::string _interface;

    static bool deserializePacket(const Tins::Packet &packet);
    static bool checkSniffedNumber();
    static void incrementSniffed();

    PacketStorage* storage = nullptr;


public:
    MiTMAttack(int wantedPacket, SniffingPackets whichPacketWanted, std::string filter, std::string interface);
    MiTMAttack();
    virtual Lorawan_result start() override;
    virtual Lorawan_result stop() override;

    Lorawan_result arpSpoofing();
    Lorawan_result dnsSpoofing();
    Lorawan_result sendPacket(LorawanPacket packet);
    Lorawan_result sniffing(std::string interface, std::string filter);

    static int _wantedPacket;
    static int _sniffedPacket;
    static SniffingPackets _whichPacketWanted;

};

#endif // MITMATTACK_H

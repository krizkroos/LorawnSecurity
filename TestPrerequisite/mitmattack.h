#ifndef MITMATTACK_H
#define MITMATTACK_H

#include "testprerequisite.h"
#include "Packet/lorawanpacket.h"
#include "packetstorage.h"

#include "tins/tins.h"

#include <map>

enum class SniffingPackets
{
    Downlink,
    Uplink, 
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
    MiTMAttack(std::map<SniffingPackets, int> wantedPacket, std::string filter, std::string interface);
    MiTMAttack();
    virtual Lorawan_result start() override;
    virtual Lorawan_result stop() override;

    Lorawan_result arpSpoofing();
    Lorawan_result dnsSpoofing();
    Lorawan_result sendPacket(LorawanPacket packet);
    Lorawan_result sniffing(std::string interface, std::string filter);

    static int _wantedPacketNum;
    static int _sniffedPacketNum;
    static SniffingPackets _currentWantedPacket;
    static std::map<SniffingPackets, int> _wantedPacket;
    static std::map<SniffingPackets, int>::iterator _nextWantedPacket;

    static void setWhichPacketWanted(const SniffingPackets &whichPacketWanted);
};

#endif // MITMATTACK_H

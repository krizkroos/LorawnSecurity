#include "mitmattack.h"

#include <iostream>

MiTMAttack::MiTMAttack()
{
    Tins::SnifferConfiguration config;
    config.set_filter("ip src 10.0.2.15");
    config.set_promisc_mode(true);
    config.set_snap_len(400);

}

Lorawan_result MiTMAttack::start()
{
    std::cout << "start MiTM prerequisite" << std::endl;
    Lorawan_result result = Lorawan_result::Success;

    result = sniffing("enp0s3", "port 1700");

    return result;

}

Lorawan_result MiTMAttack::stop()
{
    return Lorawan_result::Success;
}

bool MiTMAttack::deserializePacket(const Tins::Packet& packet)
{
    const Tins::IP &ip = packet.pdu()->rfind_pdu<Tins::IP>();
    std::cout << "Destination address: " << ip.dst_addr() << std::endl;

    if(ip.dst_addr() == Tins::IPv4Address("10.0.2.15"))
    {
        return false;
    }
    else
    {
        return true;
    }

}

Lorawan_result MiTMAttack::sniffing(std::string interface, std::string filter)
{
    if(interface.empty() || filter.empty())
        return Lorawan_result::WrongSniffingParameters;

    Tins::SnifferConfiguration config;
    config.set_promisc_mode(true);
    config.set_filter(filter);
    Tins::Sniffer sniffer(interface, config);

    config.set_snap_len(400);

    sniffer.sniff_loop(MiTMAttack::deserializePacket);


    return Lorawan_result::Success;
}

Lorawan_result MiTMAttack::arpSpoofing()
{
    return Lorawan_result::Success;
}

Lorawan_result MiTMAttack::dnsSpoofing()
{
    return Lorawan_result::Success;
}

Lorawan_result MiTMAttack::sendPacket(LorawanPacket packet)
{
    packet.serializePacket();
    return Lorawan_result::Success;
}

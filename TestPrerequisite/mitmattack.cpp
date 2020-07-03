#include "mitmattack.h"

#include <iostream>
#include "Utils/common.h"
#include "Utils/jsonparser.h"
#include "defineLorawan.h"
MiTMAttack::MiTMAttack()
{

}

Lorawan_result MiTMAttack::start()
{
    std::cout << "start MiTM prerequisite" << std::endl;
    Lorawan_result result = Lorawan_result::Success;

    result = sniffing("wlan0", "udp dst port 1700");

    return result;

}

Lorawan_result MiTMAttack::stop()
{
    return Lorawan_result::Success;
}

bool MiTMAttack::deserializePacket(const Tins::Packet& packet)
{
    static int wantedPacket =0;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "------------------------------------"<< std::endl;
    if(packet.pdu()->find_pdu<Tins::IP>())
    {
        const Tins::IP &ip = packet.pdu()->rfind_pdu<Tins::IP>();
        std::cout<<"---------- IP protocol -------------" << std::endl;
        std::cout << ip.dst_addr() << " --> " << ip.src_addr() << std::endl;

        if(ip.inner_pdu()->find_pdu<Tins::UDP>())
        {
            std::cout<<"------------UDP packet--------------" << std::endl;
            const Tins::UDP &udp = ip.inner_pdu()->rfind_pdu<Tins::UDP>();
            std::cout << "src port: " << udp.sport() << "--> dst port: " << udp.dport() << std::endl;
            Tins::PDU* udpData = udp.inner_pdu();
            bytes rawdata = udpData->serialize();
            std::cout << "size of raw data: " << std::to_string(rawdata.size())<< std::endl;
            std::cout << "raw data ----" << Common::bytes2HexStr(rawdata) << std::endl;
            std::cout <<" check if it is valid packet " << std::endl;

            if(rawdata.at(3) == 0x00) //TODO why omitt first three bytes
            {
                bytes rawJson = bytes(rawdata.begin() +12, rawdata.end()); // omit EUI
                std::cout << "received wanted packet " << std::endl;

                std::cout << "raw Json: " << Common::bytes2HexStr(rawJson) << std::endl;

                std::string jsonString = Common::bytes2Str(rawJson);
                JsonParser jParser;
                std::string lorawanData{};
                jParser.parse(jsonString);

                Lorawan_result rv = jParser.getValue(jsonKeys({"rxpk","data"}),lorawanData);

                if(rv == Lorawan_result::Success)
                {
                    std::cout << "lorawan data ----" << lorawanData  << std::endl;
                }

                wantedPacket++;
            }
            else
               std::cout <<"not a valid valid packet" << std::endl;

        }

    }

    if(wantedPacket > 5)
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

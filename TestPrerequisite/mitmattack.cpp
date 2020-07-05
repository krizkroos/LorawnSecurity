#include "mitmattack.h"

#include <iostream>
#include "Utils/common.h"
#include "Utils/jsonparser.h"
#include "defineLorawan.h"


int MiTMAttack::_wantedPacket = 0;
int MiTMAttack::_sniffedPacket = 0;
SniffingPackets MiTMAttack::_whichPacketWanted = SniffingPackets::Data;


MiTMAttack::MiTMAttack(int wantedPacket, SniffingPackets whichPacketWanted)
{
    MiTMAttack::_wantedPacket = wantedPacket;
    MiTMAttack::_whichPacketWanted = whichPacketWanted;

}

MiTMAttack::MiTMAttack()
{

}


Lorawan_result MiTMAttack::start()
{
    std::cout << "start MiTM prerequisite" << std::endl;
    Lorawan_result result = Lorawan_result::Success;

    std::cout << "WantedPacket = " << std::to_string(MiTMAttack::_wantedPacket) << std::endl;
    std::cout << "SniffedPacket = " << std::to_string(MiTMAttack::_sniffedPacket) << std::endl;
    result = sniffing("wlan0", "udp dst port 1700");
    std::cout << "after WantedPacket = " << std::to_string(MiTMAttack::_wantedPacket) << std::endl;
    std::cout << "after SniffedPacket = " << std::to_string(MiTMAttack::_sniffedPacket) << std::endl;
    return result;

}

Lorawan_result MiTMAttack::stop()
{
    return Lorawan_result::Success;
}

bool MiTMAttack::deserializePacket(const Tins::Packet& packet)
{

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

            if(rawdata.at(3) == 0x00) //TODO why omit first three bytes and why 0x00 indicates wanted packet
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
                    bytes b64Data = Common::str2Bytes(lorawanData);
                    bytes rawPacket{};
                    if(Common::decodeBase64(b64Data,rawPacket) == Lorawan_result::Success)
                    {
                        PacketStorage* storage = PacketStorage::getInstance();

                        switch(rawPacket.front())
                        {
                            case 0x00: //Request
                            {
                                std::cout << "Received JoinRequest packet" << std::endl;
                                std::shared_ptr<JoinRequestPacket> reqPacket = std::make_shared<JoinRequestPacket>();
                                reqPacket->setRawData(rawPacket);
                                storage->addPacket(reqPacket);

                                if(_whichPacketWanted == SniffingPackets::Request)
                                    MiTMAttack::incrementSniffed();

                                break;
                            }
                            case 0x20: //Accept
                            {
                                std::cout << "Received JoinAccept packet" << std::endl;
                                std::shared_ptr<JoinAcceptPacket> accPacket = std::make_shared<JoinAcceptPacket>();
                                accPacket->setRawData(rawPacket);
                                storage->addPacket(accPacket);

                                if(_whichPacketWanted == SniffingPackets::Accept)
                                    MiTMAttack::incrementSniffed();

                                break;
                            }
                            case 0x40: //unconfirmed Up
                            case 0x60: //unconfirmed Down
                            case 0x80: //confirmed Up
                            case 0xA0: //confirmed Down
                            {
                                std::cout << "Received Data packet" << std::endl;
                                std::shared_ptr<DataPacket> dataPacket = std::make_shared<DataPacket>();
                                dataPacket->setRawData(rawPacket);
                                storage->addPacket(dataPacket);

                                if(_whichPacketWanted == SniffingPackets::Data)
                                    MiTMAttack::incrementSniffed();

                                break;
                            }
                            default:
                            {
                                std::cout << "unknown packet type" << rawPacket.front()  << std::endl;
                                break;
                            }
                        }
                    }else
                    {
                        std::cout << "Error decoding base64" << std::endl;
                    }
                }


            }
            else
               std::cout <<"not a valid valid packet" << std::endl;

        }

    }

    return MiTMAttack::checkSniffedNumber();
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

bool MiTMAttack::checkSniffedNumber()
{

    std::cout << "(check) WantedPacket = " << std::to_string(MiTMAttack::_wantedPacket) << std::endl;
    std::cout << "(check) SniffedPacket = " << std::to_string(MiTMAttack::_sniffedPacket) << std::endl;
    return !(MiTMAttack::_wantedPacket == MiTMAttack::_sniffedPacket);
}

void MiTMAttack::incrementSniffed()
{
    std::cout << "incremented sniffed and wanted packet counter" <<std::endl;
    MiTMAttack::_sniffedPacket +=1;
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

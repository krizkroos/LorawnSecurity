#include "mitmattack.h"

#include <iostream>
#include "Utils/common.h"
#include "Utils/jsonparser.h"
#include "defineLorawan.h"

#include "Utils/logger.h"
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
    writeLog(Logger::MiTM,"start MiTM prerequisite");
    Lorawan_result result = Lorawan_result::Success;

    result = sniffing("wlan0", "udp port 1700");
    return result;

}

Lorawan_result MiTMAttack::stop()
{
    return Lorawan_result::Success;
}

bool MiTMAttack::deserializePacket(const Tins::Packet& packet)
{

    writeLog(Logger::MiTM, "-------------------------");

    if(packet.pdu()->find_pdu<Tins::IP>())
    {
        const Tins::IP &ip = packet.pdu()->rfind_pdu<Tins::IP>();

        writePacketLog(packet);

        if(ip.inner_pdu()->find_pdu<Tins::UDP>())
        {
            const Tins::UDP &udp = ip.inner_pdu()->rfind_pdu<Tins::UDP>();
            Tins::PDU* udpData = udp.inner_pdu();

            bytes serializedData = udpData->serialize();

            writeLog(Logger::RawData,"UDP data:");
            writeHexLog(Logger::RawData, serializedData);
            writeLog(Logger::MiTM, "looking for type of lorawan data");

             bytes rawJson{};
             std::string jsonString{};
             JsonParser jParser;
             Lorawan_result getLorawanData = Lorawan_result::Success;
             std::string lorawanData{};

            if(serializedData.at(3) == 0x00)
                //TODO why to omit first three bytes and why 0x00 indicates wanted rx packet
            {
              writeLog(Logger::MiTM, "UPLINK lorawan packet");
              rawJson = bytes(serializedData.begin() +12, serializedData.end()); // omit EUI
              jsonString = Common::bytes2Str(rawJson);
              jParser.parse(jsonString);

              getLorawanData= jParser.getValue(jsonKeys({"data"}),lorawanData);
            }
            else if(serializedData.at(1) == 0x00)
            {
              writeLog(Logger::MiTM, "DOWNLINK lorawan packet");
              rawJson = bytes(serializedData.begin() +4, serializedData.end()); // omit EUI
              jsonString = Common::bytes2Str(rawJson);
              jParser.parse(jsonString);

              getLorawanData= jParser.getValue(jsonKeys({"txpk","data"}),lorawanData);

            }
            else
            {
                writeLog(Logger::MiTM, "Not a valid serialized Data");
                return MiTMAttack::checkSniffedNumber();
            }

            if(getLorawanData == Lorawan_result::Success)
            {
                writeLog(Logger::MiTM, "successfully parsed lorawan data");
                writeLog(Logger::MiTM, "Decoding from base64: " + lorawanData);

                bytes b64Data = Common::str2Bytes(lorawanData);

                writeLog(Logger::RawData,"Base64 raw data:");
                writeHexLog(Logger::RawData, b64Data);

                bytes rawPacket{};
                if(Common::decodeBase64(b64Data,rawPacket) == Lorawan_result::Success)
                {
                    writeLog(Logger::RawData,"decoded Raw packet data:");
                    writeHexLog(Logger::RawData, rawPacket);

                    PacketStorage* storage = PacketStorage::getInstance();

                    switch(rawPacket.front())
                    {
                    case 0x00: //Request
                    {
                        writeLog(Logger::MiTM, "received JoinRequest packet");
                        std::shared_ptr<JoinRequestPacket> reqPacket = std::make_shared<JoinRequestPacket>();
                        reqPacket->setRawData(rawPacket);
                        reqPacket->setMagicFour(bytes(serializedData.begin(), serializedData.begin()+4));
                        reqPacket->setEui64(bytes(serializedData.begin() +4, serializedData.begin() + 12));
                        reqPacket->setJsonString(jsonString);
                        reqPacket->setIP(ip.dst_addr().to_string(), ip.flags(), ip.id(), ip.tos(),ip.ttl(),
                                         (ip.endianness == Tins::PDU::endian_type::BE)? false: true);
                        reqPacket->setDstPort(udp.dport());
                        reqPacket->setSrcPort(udp.sport());


                        if(reqPacket->deserialize() != Lorawan_result::Success)
                            writeLog(Logger::MiTM, "Error in deserialization of request packet");

                        storage->addPacket(reqPacket);

                        if(_whichPacketWanted == SniffingPackets::Request)
                            MiTMAttack::incrementSniffed();

                        break;
                    }
                    case 0x20: //Accept
                    {
                        writeLog(Logger::MiTM, "received JoinAccept packet");
                        std::shared_ptr<JoinAcceptPacket> accPacket = std::make_shared<JoinAcceptPacket>();
                        accPacket->setRawData(rawPacket);
                        accPacket->setMagicFour(bytes(serializedData.begin(), serializedData.begin()+4));
                        accPacket->setJsonString(jsonString);

                        accPacket->setSrcPort(udp.sport());
                        accPacket->setDstPort(udp.dport());
                        accPacket->setIP(ip.dst_addr().to_string(), ip.flags(), ip.id(), ip.tos(),ip.ttl(),
                                         (ip.endianness == Tins::PDU::endian_type::BE)? false: true);
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
                        writeLog(Logger::MiTM, "received Data packet");
                        std::shared_ptr<DataPacket> dataPacket = std::make_shared<DataPacket>();
                        if(rawPacket.front() == 0x40)
                        {
                            writeLog(Logger::MiTM, "Message type: UnconfirmedUp");
                            dataPacket->setEui64(bytes(serializedData.begin() +4, serializedData.begin() + 12));
                        }
                        else if(rawPacket.front() == 0x60)
                        {
                            writeLog(Logger::MiTM, "Message type: UnconfirmedDown");
                        }

                        dataPacket->setRawData(rawPacket);
                        dataPacket->setMagicFour(bytes(serializedData.begin(), serializedData.begin()+4));

                        dataPacket->setSrcPort(udp.sport());
                        dataPacket->setDstPort(udp.dport());
                        dataPacket->setIP(ip.dst_addr().to_string(), ip.flags(), ip.id(), ip.tos(),ip.ttl(),
                                          (ip.endianness == Tins::PDU::endian_type::BE)? false: true);
                        dataPacket->setJsonString(jsonString); // check if can be after deserialization

                        dataPacket->deserialize();
                        storage->addPacket(dataPacket);

                        if(_whichPacketWanted == SniffingPackets::Data)
                            MiTMAttack::incrementSniffed();

                        break;
                    }
                    default:
                    {
                        writeLog(Logger::MiTM,"unknown packet type");
                        break;
                    }
                    }

                }else
                {
                    writeLog(Logger::MiTM,"Error decoding base64");
                }
            }

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

    writeLog(Logger::MiTM,"(check) WantedPacket = " + std::to_string(MiTMAttack::_wantedPacket));
    writeLog(Logger::MiTM,"(check) SniffedPacket = " + std::to_string(MiTMAttack::_sniffedPacket));
    return !(MiTMAttack::_wantedPacket == MiTMAttack::_sniffedPacket);
}

void MiTMAttack::incrementSniffed()
{
    writeLog(Logger::MiTM,"incremented sniffed and wanted packet counter");
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

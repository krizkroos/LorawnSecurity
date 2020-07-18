#ifndef LORAWANPACKET_H
#define LORAWANPACKET_H

#include "defineLorawan.h"
#include "tins/tins.h"

enum class MsgType
{
    JoinRequest,
    JoinAccept,
    MACPayload
};

class LorawanPacket
{
protected:
    MsgType _type;
    bytes rawPacket;
    std::string jsonString;
    bytes MIC;
    bytes magicFour;
    Tins::IP packetIP;


    bytes eui64;
    uint16_t dstPort;
    uint16_t srcPort;
public:
    LorawanPacket();
    LorawanPacket(MsgType type);
    LorawanPacket(const std::shared_ptr<LorawanPacket> packet);

    bytes serializePacket();
    bytes getRawData() const;
    void setRawData(const bytes &value);
    MsgType getType() const;
    Lorawan_result setIP(std::string dstAddr, Tins::IP::Flags flags, uint16_t id, uint8_t tos, uint8_t ttl, bool convertToBigEndian = false);
    Tins::IP getPacketIP() const;
    std::string getJsonString() const;
    void setJsonString(const std::string &value);
    void setMagicFour(const bytes &value);
    bytes getMagicFour() const;
    bytes getMIC() const;
    void setMIC(const bytes &value);

    bytes getEui64() const;
    void setEui64(const bytes &value);
    uint16_t getSrcPort() const;
    void setSrcPort(const uint16_t &value);
    uint16_t getDstPort() const;
    void setDstPort(const uint16_t &value);
};

#endif // LORAWANPACKET_H

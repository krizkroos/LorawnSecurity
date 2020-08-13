#include "sendcontroller.h"
#include "Utils/logger.h"

Lorawan_result SendController::send(std::string rawData)
{
    static uint16_t inc = 10;

    inc+= 10;
    called(Logger::Packet);
    packetIP.id(packetIP.id() + inc);

    Tins::IP ipPacket = packetIP / Tins::UDP(dstPort, srcPort) / Tins::RawPDU(rawData);

    writeLog(Logger::Packet,"destination port: " + std::to_string(dstPort));
    writeLog(Logger::Packet,"source port: " + std::to_string(srcPort));
    sender.send(ipPacket, "wlan0");
    writeLog(Logger::Packet,"after sending packet with IP.id = " + std::to_string(packetIP.id()));
    return Lorawan_result::Success;
}

void SendController::setIP(const Tins::IP &value)
{
    packetIP = value;
}

void SendController::setMagicFour(const bytes &value)
{
    magicFour = value;
}

void SendController::setDstPort(const uint16_t &value)
{
    dstPort = value;
}

void SendController::setSrcPort(const uint16_t &value)
{
    srcPort = value;
}

void SendController::setEui64(const bytes &value)
{
    eui64 = value;
}



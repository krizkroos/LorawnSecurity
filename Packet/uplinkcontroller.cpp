#include "uplinkcontroller.h"
#include "Utils/logger.h"


void UplinkController::setIP(const Tins::IP &value)
{
    packetIP = value;
}

void UplinkController::setMagicFour(const bytes &value)
{
    magicFour = value;
}

void UplinkController::setDstPort(const uint16_t &value)
{
    dstPort = value;
}

void UplinkController::setSrcPort(const uint16_t &value)
{
    srcPort = value;
}

void UplinkController::setEui64(const bytes &value)
{
    eui64 = value;
}

UplinkController::UplinkController()
{

}


Lorawan_result UplinkController::send(std::string rawData)
{
    static uint16_t inc = 10;

    inc+= 10;
    packetIP.id(packetIP.id() + inc);

    Tins::IP ipPacket = packetIP / Tins::UDP(dstPort, srcPort) / Tins::RawPDU(rawData);
    sender.send(ipPacket, "wlan0");
    writeLog(Logger::Packet,"after sending packet with IP.id = " + std::to_string(packetIP.id()));
    return Lorawan_result::Success;
}

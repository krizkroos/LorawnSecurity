#include "uplinkcontroller.h"
#include <iostream>


void UplinkController::setDestinationAddress(const Tins::IP &value)
{
    destinationAddress = value;
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
    Tins::IP ipPacket = destinationAddress / Tins::UDP(srcPort, dstPort) / Tins::RawPDU(rawData);

    sender.send(ipPacket, "wlan0");
    std::cout << "after sending packet!" << std::endl;
    return Lorawan_result::Success;
}

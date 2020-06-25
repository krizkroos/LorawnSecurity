#include "mitmattack.h"

#include <iostream>

MiTMAttack::MiTMAttack()
{

}

Lorawan_result MiTMAttack::start()
{
    std::cout << "start MiTM prerequisite" << std::endl;
    return Lorawan_result::Success;
}

Lorawan_result MiTMAttack::stop()
{
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
    return Lorawan_result::Success;
}

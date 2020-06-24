#include "mitmattack.h"


MiTMAttack::MiTMAttack()
{

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

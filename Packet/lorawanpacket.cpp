#include "lorawanpacket.h"

LorawanPacket::LorawanPacket()
{

}

LorawanPacket::LorawanPacket(MsgType type)
{
    _type = type;
}

LorawanPacket LorawanPacket::deserializePacket(bytes data)
{
    return LorawanPacket();
}

bytes LorawanPacket::serializePacket()
{
    return bytes();
}

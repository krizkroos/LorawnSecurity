#include "lorawanpacket.h"

LorawanPacket::LorawanPacket()
{

}

LorawanPacket::LorawanPacket(MsgType type)
{
    _type = type;
}

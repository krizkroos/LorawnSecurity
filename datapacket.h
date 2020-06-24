#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "lorawanpacket.h"

class DataPacket : public LorawanPacket
{
private:
    bytes devAddr;
    byte fCtrl;
    bytes data;
public:
    DataPacket();
};

#endif // DATAPACKET_H

#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "lorawanpacket.h"

class DataPacket : public LorawanPacket
{
private:
    bytes devAddr;
    byte fCtrl;
    bytes frameCounter; //2 bytes
    bytes fOpts;
    byte fPort;
    bytes frmPayload;
public:
    DataPacket();
    Lorawan_result deserialize();
};

#endif // DATAPACKET_H

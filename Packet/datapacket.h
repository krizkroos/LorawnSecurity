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
    DataPacket(const std::shared_ptr<DataPacket> packet);
    Lorawan_result deserialize();
    Lorawan_result serialialize();
    bytes getFrameCounter() const;
    void setFrameCounter(const bytes &value);
};

#endif // DATAPACKET_H

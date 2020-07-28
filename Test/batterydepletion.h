#ifndef BATTERYDEPLETION_H
#define BATTERYDEPLETION_H

#include "lorawantest.h"

class BatteryDepletion  : public LorawanTest
{
public:
    BatteryDepletion();
    virtual Lorawan_result stop() override;
    virtual Lorawan_result launch() override;
};

#endif // BATTERYDEPLETION_H

#ifndef BRUTEFORCINGMIC_H
#define BRUTEFORCINGMIC_H

#include "lorawantest.h"

class BruteforcingMIC : public LorawanTest
{
public:
    BruteforcingMIC();
    virtual Lorawan_result stop() override;
    virtual Lorawan_result launch() override;

};

#endif // BRUTEFORCINGMIC_H

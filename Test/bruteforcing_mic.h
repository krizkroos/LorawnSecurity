#ifndef BRUTEFORCINGMIC_H
#define BRUTEFORCINGMIC_H

#include "lorawantest.h"

class BruteforcingMIC : public LorawanTest
{
public:
    BruteforcingMIC();
    virtual Lorawan_result stop();
    virtual Lorawan_result launch();

};

#endif // BRUTEFORCINGMIC_H

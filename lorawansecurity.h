#ifndef LORAWANSECURITY_H
#define LORAWANSECURITY_H

#include "LorawanSecurity_global.h"
#include "defineLorawan.h"

class LORAWANSECURITY_EXPORT LorawanSecurity
{
public:
    LorawanSecurity();
    LORAWANSECURITY_EXPORT Lorawan_result setUpTestParams();
    LORAWANSECURITY_EXPORT Lorawan_result startTestEnv();
    LORAWANSECURITY_EXPORT Lorawan_result launchTest();
private:

};

#endif // LORAWANSECURITY_H

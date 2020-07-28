#ifndef TESTER_H
#define TESTER_H


#include "lorawansecurity.h"
#include "Test/testparams.h"
#include "Test/bruteforcing_mic.h"
#include "TestPrerequisite/mitmattack.h"
#include "Device/lorawandevice1_0_2.h"
#include "Utils/common.h"
#include "Utils/jsonparser.h"
#include <memory>

class LorawanTester
{
public:
    LorawanTester();
    Lorawan_result testMIC();
    Lorawan_result testBatteryDeplation();
private:
    Lorawan_result printPackets();
};

#endif // TESTER_H

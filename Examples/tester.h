#ifndef TESTER_H
#define TESTER_H

#include "Test/testparams.h"

#include <memory>

class LorawanTester
{
public:
    LorawanTester();
    Lorawan_result testMIC(TestParams params);
    Lorawan_result testBatteryDeplation(TestParams params);
    Lorawan_result testDoSRequest(TestParams params);
private:
    Lorawan_result printPackets();
    Lorawan_result checkTestParams(TestParams params);
};

#endif // TESTER_H

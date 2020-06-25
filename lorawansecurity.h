#ifndef LORAWANSECURITY_H
#define LORAWANSECURITY_H

#include "LorawanSecurity_global.h"
#include "defineLorawan.h"
#include "Test/lorawantest.h"
#include "Test/testparams.h"

#include <memory>

class LORAWANSECURITY_EXPORT LorawanSecurity
{
public:
    LorawanSecurity();
    LORAWANSECURITY_EXPORT Lorawan_result setUpTestParams(TestParams &params);
    LORAWANSECURITY_EXPORT Lorawan_result startPrerequisites();
    LORAWANSECURITY_EXPORT Lorawan_result launchTest();
    LORAWANSECURITY_EXPORT Lorawan_result addTest(std::shared_ptr<LorawanTest> test);
private:

    std::vector<std::shared_ptr<LorawanTest>> _test;
    TestParams _testParams;

};

#endif // LORAWANSECURITY_H

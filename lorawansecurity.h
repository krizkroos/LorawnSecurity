#ifndef LORAWANSECURITY_H
#define LORAWANSECURITY_H

#include "defineLorawan.h"
#include "Test/lorawantest.h"
#include "Test/testparams.h"



class LorawanSecurity
{
public:
    LorawanSecurity();
    Lorawan_result startPrerequisites();
    Lorawan_result launchTest();
    Lorawan_result addTest(std::shared_ptr<LorawanTest> test);
private:

    std::vector<std::shared_ptr<LorawanTest>> _test;
    TestParams _testParams;

};

#endif // LORAWANSECURITY_H

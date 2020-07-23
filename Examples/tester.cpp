#include "tester.h"
#include <iostream>

#include "Utils/logger.h"

LorawanTester::LorawanTester()
{

}

Lorawan_result LorawanTester::testMIC()
{
    LorawanSecurity loraSec;
    TestParams params;

    std::cout << "testing MIC" << std::endl;

    writeLog(Logger::RAW_PACKET, "test 2");

    params.setLogFileName("lorawan-test.log");
    loraSec.setUpTestParams(params);



    std::shared_ptr<LorawanDevice1_0_2> testDevice = std::make_shared<LorawanDevice1_0_2>();
    std::shared_ptr<MiTMAttack> mitm = std::make_shared<MiTMAttack>(10, SniffingPackets::Data);
    mitm->setName("MiTM");
    std::shared_ptr<BruteforcingMIC> testOne = std::make_shared<BruteforcingMIC>();

    testOne->setDescription("Brutforcing MIC Test");
    testOne->setTestDevice(testDevice);

    if(testOne->addPrerequisite(mitm) != Lorawan_result::Success)
    {
        std::cout << "Error adding prerequisite" << std::endl;
        return Lorawan_result::ErrorPrerequisite;
    }

    if(loraSec.addTest(testOne) != Lorawan_result::Success)
    {
        std::cout << "Error adding test" << std::endl;
        return Lorawan_result::ErrorTest;
    }

    if(loraSec.startPrerequisites()  != Lorawan_result::Success)
    {
        std::cout << "Error starting prerequisite" << std::endl;
        return Lorawan_result::ErrorPrerequisite;
    }

    if(loraSec.launchTest()  != Lorawan_result::Success)
    {
        std::cout << "Error launching test" << std::endl;
        return Lorawan_result::ErrorTest;
    }

    return Lorawan_result::Success;
}

Lorawan_result LorawanTester::testADR()
{
   return Lorawan_result::Success;
}



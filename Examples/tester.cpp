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
    params.setLogFileName("lorawan-test.log");

    Logger logger(params.getLogFileName(), Logger::JSON | Logger::RawData | Logger::MiTM | Logger::LorawanTest);
    writeLog(Logger::LorawanTest, "testing MIC");


    loraSec.setUpTestParams(params);

    std::shared_ptr<LorawanDevice1_0_2> testDevice = std::make_shared<LorawanDevice1_0_2>();
    std::shared_ptr<MiTMAttack> mitm = std::make_shared<MiTMAttack>(4, SniffingPackets::Data,"udp dst port 1700","wlan0");
    mitm->setName("MiTM");
    std::shared_ptr<BruteforcingMIC> testOne = std::make_shared<BruteforcingMIC>();

    testOne->setDescription("Brutforcing MIC Test");
    testOne->setTestDevice(testDevice);

    if(testOne->addPrerequisite(mitm) != Lorawan_result::Success)
    {
        writeLog(Logger::LorawanTest,"Error adding prerequisite");
        return Lorawan_result::ErrorPrerequisite;
    }

    if(loraSec.addTest(testOne) != Lorawan_result::Success)
    {
        writeLog(Logger::LorawanTest,"Error adding test");
        return Lorawan_result::ErrorTest;
    }

    if(loraSec.startPrerequisites()  != Lorawan_result::Success)
    {
        writeLog(Logger::LorawanTest,"Error starting prerequisite");
        return Lorawan_result::ErrorPrerequisite;
    }

    if(loraSec.launchTest()  != Lorawan_result::Success)
    {
        writeLog(Logger::LorawanTest,"Error launching test");
        return Lorawan_result::ErrorTest;
    }

    return Lorawan_result::Success;
}

Lorawan_result LorawanTester::testADR()
{
   return Lorawan_result::Success;
}



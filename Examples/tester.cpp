#include "tester.h"
#include <iostream>

#include "Utils/logger.h"
#include "Test/batterydepletion.h"
#include <map>

LorawanTester::LorawanTester()
{

}

Lorawan_result LorawanTester::testMIC()
{
    LorawanSecurity loraSec;
    TestParams params;
    params.setLogFileName("lorawan-MIC-test.log");

    Logger logger(params.getLogFileName(), Logger::JSON | Logger::RawData | Logger::MiTM | Logger::LorawanTest | Logger::Common | Logger::BruteforcingMIC);
    writeLog(Logger::LorawanTest, "testing MIC");


    loraSec.setUpTestParams(params);

    std::shared_ptr<LorawanDevice1_0_2> testDevice = std::make_shared<LorawanDevice1_0_2>();

    std::map<SniffingPackets, int> wantedPacket;

    wantedPacket.insert(std::pair<SniffingPackets, int>( SniffingPackets::Uplink, 2));

    std::shared_ptr<MiTMAttack> mitm = std::make_shared<MiTMAttack>(wantedPacket,"udp dst port 1700","wlan0");
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

    printPackets();

    if(loraSec.launchTest()  != Lorawan_result::Success)
    {
        writeLog(Logger::LorawanTest,"Error launching test");
        return Lorawan_result::ErrorTest;
    }

    return Lorawan_result::Success;
}

Lorawan_result LorawanTester::testBatteryDeplation()
{

    LorawanSecurity loraSec;
    TestParams params;
    params.setLogFileName("lorawan-test-battery.log");

    Logger logger(params.getLogFileName(), Logger::JSON | Logger::RawData | Logger::Common | Logger::LorawanTest | Logger::BatteryDepletion);
    writeLog(Logger::LorawanTest, "testing battery deplation");


    loraSec.setUpTestParams(params);

    std::shared_ptr<LorawanDevice1_0_2> testDevice = std::make_shared<LorawanDevice1_0_2>();

    std::map<SniffingPackets, int> wantedPacket;

    wantedPacket.insert(std::pair<SniffingPackets, int>( SniffingPackets::Downlink, 3));
    wantedPacket.insert(std::pair<SniffingPackets, int>( SniffingPackets::Uplink, 1));

    std::shared_ptr<MiTMAttack> mitm = std::make_shared<MiTMAttack>(wantedPacket,"udp port 1700","wlan0");
    mitm->setName("MiTM");
    std::shared_ptr<BatteryDepletion> testOne = std::make_shared<BatteryDepletion>();

    testOne->setDescription("Battery deplation test");
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

    printPackets();

    if(loraSec.launchTest()  != Lorawan_result::Success)
    {
        writeLog(Logger::LorawanTest,"Error launching test");
        return Lorawan_result::ErrorTest;
    }
   return Lorawan_result::Success;
}

Lorawan_result LorawanTester::printPackets()
{
    PacketStorage* storage = PacketStorage::getInstance();

    writeLog(Logger::LorawanTest,"Able to perform test with below packets:");

    std::vector<std::shared_ptr<JoinRequestPacket> > requestPackets = storage->getRequestPacket();
    std::vector<std::shared_ptr<JoinAcceptPacket> > acceptPackets = storage->getAcceptPacket();
    std::vector<std::shared_ptr<DataPacket> > dataPackets = storage->getMacPayloadPacket();

    writeLog(Logger::LorawanTest,"Available packets:");
    writeLog(Logger::LorawanTest,"Requests:" + std::to_string(requestPackets.size()));
    writeLog(Logger::LorawanTest, "Accepts:" + std::to_string(acceptPackets.size()));
    writeLog(Logger::LorawanTest,"Datas:" + std::to_string(dataPackets.size()));


    for(auto &request: requestPackets)
    {
        writeLog(Logger::LorawanTest,"next request packet:");
        writeLog(Logger::LorawanTest,Common::bytes2HexStr(request->getRawData()));
    }


    for(auto &accept: acceptPackets)
    {
        writeLog(Logger::LorawanTest,"next accept packet:");
        writeLog(Logger::LorawanTest,Common::bytes2HexStr(accept->getRawData()));
    }

    for(auto &data: dataPackets)
    {
        writeLog(Logger::LorawanTest,"next data packet:");
        writeLog(Logger::LorawanTest,Common::bytes2HexStr(data->getRawData()));
    }

    return Lorawan_result::Success;
}



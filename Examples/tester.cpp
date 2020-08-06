#include "tester.h"
#include <iostream>

#include "Utils/logger.h"
#include "Test/batterydepletion.h"
#include <map>

LorawanTester::LorawanTester()
{

}

Lorawan_result LorawanTester::checkTestParams(TestParams params)
{
    std::string logfileName = params.getLogFileName();
    std::string interface = params.getInterfaceName();
    std::string filter = params.getFilter();
    if(logfileName.empty())
    {
        std::cout << "No log file name provided" << std::endl;
        return Lorawan_result::ErrorTest;
    }

    if(params.getLogLevel() == 0)
    {
        std::cout << "No log level set" << std::endl;
        return Lorawan_result::ErrorTest;
    }

    if(interface.empty())
    {
        writeLog(Logger::LorawanTest,"No interface provided");
        return Lorawan_result::ErrorTest;
    }

    if(filter.empty())
    {
        writeLog(Logger::LorawanTest,"No filter provided");
        return Lorawan_result::ErrorTest;
    }

    return Lorawan_result::Success;

}

Lorawan_result LorawanTester::testMIC(TestParams params)
{
    LorawanSecurity loraSec;

    writeLog(Logger::LorawanTest, "testing MIC");

    if(checkTestParams(params) == Lorawan_result::ErrorTest)
    {
        return Lorawan_result::ErrorTest;
    }

    Logger logger(params.getLogFileName(),params.getLogLevel());

    std::shared_ptr<LorawanDevice1_0_2> testDevice = std::make_shared<LorawanDevice1_0_2>();

    std::map<SniffingPackets, int> wantedPacket;

    wantedPacket.insert(std::pair<SniffingPackets, int>( SniffingPackets::Uplink, 2));

    std::shared_ptr<MiTMAttack> mitm = std::make_shared<MiTMAttack>(wantedPacket,params.getFilter(),params.getInterfaceName());
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

Lorawan_result LorawanTester::testBatteryDeplation(TestParams params)
{

    LorawanSecurity loraSec;
    writeLog(Logger::LorawanTest, "testing battery depletion");
    if(checkTestParams(params) == Lorawan_result::ErrorTest)
    {
        return Lorawan_result::ErrorTest;
    }

    Logger logger(params.getLogFileName(), params.getLogLevel());

    std::shared_ptr<LorawanDevice1_0_2> testDevice = std::make_shared<LorawanDevice1_0_2>();

    std::map<SniffingPackets, int> wantedPacket;

    wantedPacket.insert(std::pair<SniffingPackets, int>( SniffingPackets::Downlink, 1));
    wantedPacket.insert(std::pair<SniffingPackets, int>( SniffingPackets::Uplink, 1));

    std::shared_ptr<MiTMAttack> mitmCollectDownlink = std::make_shared<MiTMAttack>(wantedPacket,params.getFilter(),params.getInterfaceName());
    mitmCollectDownlink->setName("MiTM");
    std::shared_ptr<BatteryDepletion> testOne = std::make_shared<BatteryDepletion>();

    testOne->setDescription("Battery deplation test");
    testOne->setTestDevice(testDevice);

    if(testOne->addPrerequisite(mitmCollectDownlink) != Lorawan_result::Success)
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

    wantedPacket.clear();
    wantedPacket.insert(std::pair<SniffingPackets, int>( SniffingPackets::Uplink, 1));
    std::shared_ptr<MiTMAttack> mitmLoopUplink = std::make_shared<MiTMAttack>(wantedPacket,params.getFilter(),params.getInterfaceName());

    testOne->clearPrerequisites();

    if(testOne->addPrerequisite(mitmLoopUplink) != Lorawan_result::Success)
    {
        writeLog(Logger::LorawanTest,"Error adding next prerequisite");
        return Lorawan_result::ErrorPrerequisite;
    }

    int downlinkCounter = 0;
    int paramsCounter = params.getDownlinkCounter();
    if( paramsCounter > 0)
    {
        downlinkCounter = paramsCounter;
        writeLog(Logger::LorawanTest,"downlink counter set up from params = " + std::to_string(downlinkCounter));
    }
    else
    {
        downlinkCounter = 3;
        writeLog(Logger::LorawanTest,"downlink counter set to default = " + std::to_string(downlinkCounter));
    }

    for(int i=0; i < downlinkCounter; i++)
    {
        writeLog(Logger::LorawanTest, "loop no "+ std::to_string(i));
        if(loraSec.startPrerequisites()  != Lorawan_result::Success)
        {
            writeLog(Logger::LorawanTest,"Error starting next prerequisite");
            return Lorawan_result::ErrorPrerequisite;
        }

        printPackets();

        if(loraSec.launchTest()  != Lorawan_result::Success)
        {
            writeLog(Logger::LorawanTest,"Error launching next test");
            return Lorawan_result::ErrorTest;
        }

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





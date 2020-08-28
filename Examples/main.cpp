#include <iostream>

#include "Utils/logger.h"
#include "Test/testparams.h"
#include "Examples/tester.h"
#include "Utils/common.h"

int main()
{
    std::cout <<"Start testing LoRaWAN protocol..."<<std::endl;

    LorawanTester tester;

//    TestParams paramsMIC;
//    paramsMIC.setLogLevel(Logger::JSON | Logger::RawData | Logger::MiTM | Logger::LorawanTest | Logger::Common | Logger::BruteforcingMIC);
//    paramsMIC.setLogFileName("logs-mic/test-MIC-" + Common::getTime() + ".log");
//    //paramsMIC.setMicValuesFileName("micValues.txt");
//    paramsMIC.setInterfaceName("wlan0");
//    paramsMIC.setFilter("udp dst port 1700");
//    paramsMIC.setMax_gap(16384);
//    paramsMIC.setNwkSKey(bytes({0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x32,0xA0}));
//    //paramsMIC.setNwkSKey(bytes({0xD2,0x0F,0xE6,0x0A,0x81,0x02,0x82,0x06,0x16,0x58,0xFB,0x6E,0x24,0xC9,0x15,0xF9}));
//    if(tester.testMIC(paramsMIC) != Lorawan_result::Success)
//    {
//        std::cout << "testing MIC failed" <<std::endl;
//    }

//    TestParams paramsBattery;
//    paramsBattery.setLogLevel(Logger::JSON | Logger::RawData | Logger::Common | Logger::LorawanTest | Logger::BatteryDepletion);
//    paramsBattery.setLogFileName("logs-battery/test-battery-" + Common::getTime() + ".log");
//    paramsBattery.setInterfaceName("wlan0");
//    paramsBattery.setFilter("udp port 1700");
//    paramsBattery.setDownlinkCounter(3);
//    paramsBattery.setNwkSKey(bytes({0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x32,0xA0}));
//    if(tester.testBatteryDeplation(paramsBattery) != Lorawan_result::Success)
//    {
//        std::cout << "testing Battery Depletion failed" <<std::endl;
//    }

    TestParams paramsDoSRequest;
    paramsDoSRequest.setLogLevel(Logger::JSON | Logger::RawData | Logger::MiTM | Logger::LorawanTest | Logger::Common | Logger::RequestDoS);
    paramsDoSRequest.setLogFileName("logs-req/test-request-" + Common::getTime() + ".log");
    paramsDoSRequest.setInterfaceName("wlan0");
    paramsDoSRequest.setFilter("udp dst port 1700");
    paramsDoSRequest.setRequestCounter(240);
    paramsDoSRequest.setAppKey(bytes({0xC1,0xA9,0xF5,0x40,0x6F,0x2D,0x70,0x06,0xA5,0xC6,0xE9,0x01,0xAA,0x13,0x4E,0x0B}));
    if(tester.testDoSRequest(paramsDoSRequest) != Lorawan_result::Success)
    {
        std::cout << "testing DoS of JoinRequest packet failed" <<std::endl;
    }


    return 0;

}

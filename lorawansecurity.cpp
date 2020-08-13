#include "lorawansecurity.h"
#include <iostream>
#include "Utils/logger.h"
#include "packetstorage.h"

Lorawan_result LorawanSecurity::addTest(std::shared_ptr<LorawanTest> test)
{
    if(test->isValid() == Lorawan_result::Success)
    {
        _test.emplace_back(test);
    }
    else
    {
        writeLog(Logger::LorawanTest,"Error addTest");
        return Lorawan_result::Error;
    }

    return Lorawan_result::Success;
}


Lorawan_result LorawanSecurity::startPrerequisites()
{
    Lorawan_result result = Lorawan_result::Success;

    std::cout << std::endl;
    std::cout <<"----------------- Starting test prerequisites ----------------"<< std::endl;
    for(auto &test : _test)
    {
        writeLog(Logger::LorawanTest, "Test describtion : " + test->getDescription());

        auto &prerequisite = test->getPrerequisite();

        if(prerequisite.size() == 0) // vector size
        {
            writeLog(Logger::LorawanTest,"No prerequisite required");
        }
        for(auto &prereq : prerequisite)
        {
            writeLog(Logger::LorawanTest,"Prerequisite : " + prereq->getName());

            result = prereq->start();
            if(result != Lorawan_result::Success)
            {
                writeLog(Logger::LorawanTest, "Error starting condition");
                PacketStorage::deleteInstance();
                return result;
            }
            else
                continue;
        }
    }

    return result;
}

Lorawan_result LorawanSecurity::launchTest()
{
    std::cout << std::endl;
    std::cout <<"----------------- Starting tests -----------------------------" << std::endl;
    Lorawan_result result = Lorawan_result::Success;
    for(auto &test : _test)
    {
        writeLog(Logger::LorawanTest,"Test describtion : " + test->getDescription());
        result = test->launch();
        if(result != Lorawan_result::Success)
        {
            writeLog(Logger::LorawanTest,"starting test failed");
            PacketStorage::deleteInstance();
            return result;
        }
    }
    PacketStorage::deleteInstance();
    return result;
}

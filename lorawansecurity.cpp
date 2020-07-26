#include "lorawansecurity.h"
#include <iostream>
#include "Utils/logger.h"

LorawanSecurity::LorawanSecurity()
{

}

Lorawan_result LorawanSecurity::addTest(std::shared_ptr<LorawanTest> test)
{
    if(test->isValid() == Lorawan_result::Success)
    {
        _test.emplace_back(test);
    }
    else
    {
        writeLog(Logger::Common,"Error addTest");
        return Lorawan_result::Error;
    }

    return Lorawan_result::Success;
}

Lorawan_result LorawanSecurity::setUpTestParams(TestParams &params)
{
    _testParams = params;

    return Lorawan_result::Success;
}

Lorawan_result LorawanSecurity::startPrerequisites()
{
    Lorawan_result result = Lorawan_result::Success;

    std::cout << std::endl;
    std::cout <<"----------------- Starting test prerequisites ----------------"<< std::endl;
    for(auto &test : _test)
    {
        writeLog(Logger::Common, "Test describtion : " + test->getDescription());

        auto &prerequisite = test->getPrerequisite();

        if(prerequisite.size() == 0) // vector size
        {
            writeLog(Logger::Common,"No prerequisite required");
        }
        for(auto &prereq : prerequisite)
        {
            writeLog(Logger::Common,"Prerequisite : " + prereq->getName());

            result = prereq->start();
            if(result != Lorawan_result::Success)
            {
                writeLog(Logger::Common, "Error starting condition");
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
        writeLog(Logger::Common,"Test describtion : " + test->getDescription());
        result = test->launch();
        if(result != Lorawan_result::Success)
        {
            writeLog(Logger::Common,"starting test failed");
            return result;
        }
    }
    return result;
}
